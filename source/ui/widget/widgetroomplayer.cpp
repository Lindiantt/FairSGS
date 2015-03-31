#include "widgetroomplayer.h"
#include "ui_widgetroomplayer.h"
#include "thread/workerloadsource.h"
#include "mainwindow.h"
extern MainWindow *w;
#include <qmenu.h>
#include "widgetroom.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "network/cclient.h"
#include <qmessagebox.h>
#include <qclipboard.h>
#include <qpainter.h>

WidgetRoomPlayer::WidgetRoomPlayer(quint8 pos, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetRoomPlayer)
{
    ui->setupUi(this);
    this->pos=pos;
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(handleContextMenu(QPoint)));
}

WidgetRoomPlayer::~WidgetRoomPlayer()
{
    delete ui;
}

void WidgetRoomPlayer::handleContextMenu(QPoint)
{
    WidgetRoom *room=w->mwCLient->ui->widgetRoom;
    if(room->me!=-1&&room->me==this->pos) return;
    if(this->empty) return;
    QMenu menu(w->mwCLient);
    menu.addAction("查看资料");
    menu.addAction("复制昵称");
    if(room->me!=-1&&!this->admin)
    {
        if(room->players[room->me]->roomHost||room->players[room->me]->admin)
            menu.addAction("踢人");
    }
    QAction *action=menu.exec(QCursor::pos());
    if(room->me!=-1&&room->me==this->pos) return;
    if(this->empty) return;
    if(action)
    {
        QString s=action->text();
        QByteArray ba;
        if(s=="查看资料")
        {
            if(!this->udpAvailable)
            {
                QMessageBox::about(w->mwCLient,"错误","无法查看资料。");
                return;
            }
            //
        }
        else if(s=="复制昵称")
        {
            QClipboard *cb=QApplication::clipboard();
            cb->setText(this->nick);
        }
        else
        {
            if(room->me==-1||this->admin||(!room->players[room->me]->roomHost&&!room->players[room->me]->admin))
            {
                return;
            }
            ba.resize(4);
            ba[2]=INROOM_KICK;
            ba[3]=this->pos;
            w->client->send(ba);
        }
    }
}

void WidgetRoomPlayer::setFavorite(quint16 favor)
{
    if(favor>=MAX_GENERAL) favor=0;
    this->favorite=favor;
    w->imageLoad(w->wLoadSource->generalCards[favor],ui->labelPicture);
}

void WidgetRoomPlayer::init()
{
    empty=true;
    this->roomHost=false;
    this->admin=false;
    this->ready=false;
    this->udpAvailable=false;
    ui->labelAdmin->setVisible(false);
    ui->labelRoomHost->setVisible(false);
    ui->labelReady->setVisible(false);
    ui->labelPicture->clear();
    ui->labelNick->clear();
}

void WidgetRoomPlayer::setRoomHost()
{
    roomHost=true;
    ui->labelRoomHost->setVisible(true);
}

void WidgetRoomPlayer::setAdmin()
{
    admin=true;
    ui->labelAdmin->setVisible(true);
}

void WidgetRoomPlayer::setReady(bool b)
{
    ready=b;
    ui->labelReady->setVisible(b);
}

void WidgetRoomPlayer::setNick(const QString &n)
{
    this->nick=n;
    ui->labelNick->setText(n);
}

void WidgetRoomPlayer::moveTo(quint8 pos)
{
    WidgetRoomPlayer *player=w->mwCLient->ui->widgetRoom->players[pos];
    player->empty=false;
    player->setNick(nick);
    player->setReady(ready);
    if(admin) player->setAdmin();
    if(roomHost) player->setRoomHost();
    player->setFavorite(favorite);
    this->init();
}

void WidgetRoomPlayer::mouseDoubleClickEvent(QMouseEvent *)
{
    int me=w->mwCLient->ui->widgetRoom->me;
    if(!empty||pos==me||me==-1) return;
    QByteArray ba;
    ba.resize(4);
    ba[2]=INROOM_MOVE;
    ba[3]=pos;
    w->client->send(ba);
}

void WidgetRoomPlayer::paintEvent(QPaintEvent *ev)
{
    QPainter pt(this);
    pt.fillRect(0,0,width(),this->height(),QColor(136,68,0));
    QWidget::paintEvent(ev);
}
