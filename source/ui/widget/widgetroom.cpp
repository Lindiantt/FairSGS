#include "widgetroom.h"
#include "ui_widgetroom.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "network/cclient.h"
#include "widgetroomplayer.h"
#include "network/define.h"
#include <qlabel.h>
#include <qmenu.h>
#include <qmessagebox.h>
#include "ui/mainwindow/mainwindowclient.h"
#include "ui/widget/widgetgame.h"
#include "ui_mainwindowclient.h"

#define COMPARE(a) if(a){w->client->end2("与服务器断开连接。");return false;}

WidgetRoom::WidgetRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetRoom)
{
    ui->setupUi(this);
    this->setVisible(false);
}

WidgetRoom::~WidgetRoom()
{
    delete ui;
}

bool WidgetRoom::rhEnterRoom(QByteArray &ba)
{
    me=ba[0];
    if(me<0||me>=w->client->numberOfPlayer) return false;
    int pos=2;
    for(int i=0;i<ba[1];i++)
    {
        pos=readPlayerInfo(ba,pos);
        if(pos==-1) return false;
    }
    w->client->state=CLIENTSTATE_INROOM;
    this->setVisible(true);
    arrangePlayers();
    if(w->client->maxRoom==1)
        ui->pushButtonReturn->setVisible(false);
    else
        ui->pushButtonReturn->setVisible(true);
    return true;
}

void WidgetRoom::init()
{
    clear();
    disable=false;
    players.resize(w->client->numberOfPlayer);
    for(int i=0;i<w->client->numberOfPlayer;i++)
    {
        players[i]=new WidgetRoomPlayer(i,ui->scrollAreaWidgetContents);
        players[i]->init();
    }
}

void WidgetRoom::clear()
{
    if(!players.isEmpty())
    {
        qDeleteAll(players);
        players.clear();
    }
}

void WidgetRoom::arrangePlayers()
{
    QGridLayout *lo=reinterpret_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
    foreach (auto player, players) {
        lo->removeWidget(player);
    }
    int w=ui->scrollAreaWidgetContents->width();
    w=(w+30)/250;
    if(w)
    {
        int x,y;
        for(int i=0;i<players.length();i++)
        {
            x=i%w;
            y=i/w;
            lo->addWidget(players[i],y,x);
        }
    }
    ui->scrollAreaWidgetContents->setMinimumHeight(((players.length()-1)/w+1)*331);
}

int WidgetRoom::readPlayerInfo(QByteArray &ba, int pos)
{
    quint8 l;
    int p;
    QHostAddress ha;
    quint32 ip;
    quint16 favor;
    QString s;
    int j;
    if(ba.length()<pos+11) return -1;
    p=ba[pos+6];
    memcpy(&ip,ba.data()+pos,4);
    ip=qFromLittleEndian(ip);
    ha.setAddress(ip);
    players[p]->hostAddress=ha;
    memcpy(&favor,ba.data()+pos+4,2);
    favor=qFromLittleEndian(favor);
    players[p]->setFavorite(favor);
    players[p]->pos=p;
    l=ba[pos+7];
    if(l&ROOMPLAYER_ADMIN) players[p]->setAdmin();
    if(l&ROOMPLAYER_HOST) players[p]->setRoomHost();
    if(ba[pos+8]) players[p]->setReady(true);
    pos+=9;
    l=ba[pos];
    if(ba.length()<pos+l+2) return -1;
    s=QString::fromUtf8(ba.data()+pos+1,l);
    players[p]->setNick(s);
    pos+=l+1;
    if(ba[pos])
    {
        if(ba.length()<pos+7) return -1;
        players[p]->udpAvailable=true;
        memcpy(&favor,ba.data()+pos+1,2);
        players[p]->udpPort=qFromLittleEndian(favor);
        memcpy(&j,ba.data()+pos+3,4);
        players[p]->udpCode=j;
    }
    pos+=7;
    players[p]->empty=false;
    if(p==me)
    {
        buttonInit(true,players[p]->roomHost);
    }
    return pos;
}

bool WidgetRoom::rhInRoom(QByteArray &ba)
{
    switch (ba[0]) {
    case INROOM_NEWPLAYER:
        COMPARE(readPlayerInfo(ba,1)==-1);
        break;
    case INROOM_READY:
        COMPARE(ba.length()!=2);
    {
        quint8 c=ba[1];
        COMPARE(c>=players.size());
        players[c]->setReady(true);
        if(c==me) ui->pushButtonReady->setEnabled(true);
        if(me!=-1&&players[me]->roomHost)
        {
            for(int i=0;i<players.size();i++)
            {
                if(i!=me&&!players[i]->ready)
                {
                    return true;
                }
            }
            ui->pushButtonStart->setEnabled(true);
        }
    }
        break;
    case INROOM_UNREADY:
        COMPARE(ba.length()!=2);
    {
        quint8 c=ba[1];
        COMPARE(c>=players.size());
        players[c]->setReady(false);
        if(c==me) ui->pushButtonReady->setEnabled(true);
        if(me!=-1&&players[me]->roomHost) ui->pushButtonStart->setEnabled(false);
    }
        break;
    case INROOM_PLAYERLEAVE:
        COMPARE(ba.length()!=2);
    {
        quint8 c=ba[1];
        COMPARE(c>=players.size());
        players[c]->init();
        if(me!=-1&&players[me]->roomHost) ui->pushButtonStart->setEnabled(false);
    }
        break;
    case INROOM_STARTGAME:
        w->client->state=CLIENTSTATE_STARTGAME;
        this->setVisible(false);
        w->mwCLient->ui->widgetGame->setVisible(true);
        w->mwCLient->ui->widgetGame->init();
        w->mwCLient->ui->widgetBottom->setVisible(true);
        w->mwCLient->ui->widgetBottom->init();
        break;
    case INROOM_CLIMB_OK:
        COMPARE(ba.length()!=1);
        //
        break;
    case INROOM_SITDOWN_OK:
        COMPARE(ba.length()!=1);
        w->client->state=CLIENTSTATE_ENTERROOM;
        break;
    case INROOM_MOVE:
        COMPARE(ba.length()!=3);
        COMPARE(ba[1]<(char)0||ba[1]>=(char)players.size());
        COMPARE(ba[2]<(char)0||ba[2]>=(char)players.size());
        if(!players[ba[1]]->empty&&players[ba[2]]->empty)
        {
            players[ba[1]]->moveTo(ba[2]);
            if(ba[1]==me) me=ba[2];
        }
        break;
    case INROOM_RETURN_OK:
        COMPARE(ba.length()!=1);
        w->client->state=CLIENTSTATE_INSERVER;
        this->setVisible(false);
        break;
    case INROOM_KICK:
        QMessageBox::about(reinterpret_cast<QWidget*>(w->mwCLient),"提示","您被踢出房间。");
        w->client->state=CLIENTSTATE_INSERVER;
        this->setVisible(false);
        break;
    default:
        break;
    }
    return true;
}

void WidgetRoom::buttonInit(bool isPlayer, bool isRoomHost)
{
    if(w->client->maxRoom==1)
        ui->pushButtonReturn->setVisible(false);
    else
        ui->pushButtonReturn->setVisible(true);
    if(isPlayer)
    {
        if(isRoomHost)
        {
            ui->pushButtonStart->setVisible(true);
            ui->pushButtonStart->setEnabled(false);
            ui->pushButtonReady->setVisible(false);
            ui->pushButtonSitDown->setVisible(false);
        }
        else
        {
            ui->pushButtonStart->setVisible(false);
            ui->pushButtonReady->setVisible(true);
            ui->pushButtonSitDown->setVisible(true);
            ui->pushButtonSitDown->setText("去观战");
            ui->pushButtonReady->setEnabled(true);
            ui->pushButtonReady->setChecked(false);
        }
    }
    else
    {
        ui->pushButtonSitDown->setText("坐下");
        ui->pushButtonSitDown->setVisible(true);
        ui->pushButtonReady->setVisible(false);
        ui->pushButtonStart->setVisible(false);
        me=-1;
    }
}

void WidgetRoom::on_pushButtonReady_clicked(bool checked)
{
    QByteArray ba;
    ba.resize(3);
    ui->pushButtonReady->setEnabled(false);
    disable=true;
    if(checked)
        ba[2]=INROOM_READY;
    else
        ba[2]=INROOM_UNREADY;
    w->client->send(ba);
}

void WidgetRoom::on_pushButtonReturn_clicked()
{
    QByteArray sendbuf;
    sendbuf.resize(3);
    sendbuf[2]=INROOM_RETURN;
    w->client->send(sendbuf);
}

void WidgetRoom::on_pushButtonSitDown_clicked()
{
    QByteArray sendbuf;
    sendbuf.resize(3);
    if(ui->pushButtonSitDown->text()=="坐下")
        sendbuf[2]=INROOM_SITDOWN;
    else
        sendbuf[2]=INROOM_CLIMB;
    w->client->send(sendbuf);
    ui->pushButtonSitDown->setEnabled(false);
    QTimer::singleShot(5000,[=](){
        ui->pushButtonSitDown->setEnabled(true);
    });
}

void WidgetRoom::on_pushButtonStart_clicked()
{
    QByteArray sendbuf;
    sendbuf.resize(3);
    sendbuf[2]=INROOM_STARTGAME;
    w->client->send(sendbuf);
    ui->pushButtonStart->setEnabled(false);
    QTimer::singleShot(2000,[=](){
        for(int i=0;i<players.size();i++)
        {
            if(!players[i]->ready&&i!=me)
                return;
        }
        ui->pushButtonStart->setEnabled(true);
    });
}

bool WidgetRoom::rhStartGame(QByteArray &ba)
{
    int pos=0;
    uint ip;
    quint16 us;
    for(int i=0;i<w->client->numberOfPlayer;i++)
    {
        COMPARE(ba.length()<pos+5);
        memcpy(&ip,ba.data()+pos,4);
        ip=qFromLittleEndian(ip);
        players[i]->hostAddress.setAddress(ip);
        if(ba[pos+4])
        {
            COMPARE(ba.length()<pos+11);
            memcpy(&us,ba.data()+pos+5,2);
            players[i]->udpPort=qFromLittleEndian(us);
            memcpy(&players[i]->udpCode,ba.data()+7,4);
            pos+=11;
        }
        else
            pos+=5;
    }
    w->client->state=CLIENTSTATE_PLAYING;
    return true;
}
