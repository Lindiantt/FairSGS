#include "widgetroom.h"
#include "ui_widgetroom.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "network/cclient.h"
#include "widgetroomplayer.h"
#include "network/define.h"
#include <qlabel.h>

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

bool WidgetRoom::rhEnterRoom()
{
    QByteArray ba;
    ba=w->client->socket.read(w->client->bytesRemain);
    w->client->bytesRemain=0;
    int pos=1;
    quint8 l;
    int p;
    QHostAddress ha;
    quint32 ip;
    quint16 favor;
    QString s;
    int j;
    for(int i=0;i<ba[0];i++)
    {
        if(ba.length()<pos+11) return false;
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
        if(ba.length()<pos+l+2) return false;
        s=QString::fromUtf8(ba.data()+pos+1,l);
        players[p]->setNick(s);
        pos+=l+1;
        if(ba[pos])
        {
            if(ba.length()<pos+7) return false;
            players[p]->udpAvailable=true;
            memcpy(&favor,ba.data()+pos+1,2);
            players[p]->udpPort=qFromLittleEndian(favor);
            memcpy(&j,ba.data()+pos+3,4);
            players[p]->udpCode=j;
        }
        pos+=7;
    }
    w->client->state=CLIENTSTATE_INSERVER;
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
