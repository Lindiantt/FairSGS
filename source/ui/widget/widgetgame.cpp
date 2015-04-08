#include "widgetgame.h"
#include "ui_widgetgame.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include <QPainter>
#include "network/cclient.h"
#include "ui/widget/widgetgameplayer.h"
#include "game/cgameclient.h"
#include "game/cplayerclientme.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "ui/widget/widgetroom.h"
#include "ui/dialog/dialogchoosegeneral.h"
#include "general/cgeneral.h"
#include "thread/cicon.h"
#include "thread/ccheckpointer.h"
#include "game/coperation.h"
#include "card/ccard.h"

#define COMPARE(a) if(a){w->client->end2("与服务器断开连接。");return false;}
#define PLAYER ((CPlayerClient*)player)

WidgetGame::WidgetGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetGame)
{
    game=nullptr;
    w->imageLoad(w->wLoadSource->bgRoom,this);
    ui->setupUi(this);
    w->iconLoad(w->wLoadSource->buttonWuXie,ui->pushButtonWuXie);
}

WidgetGame::~WidgetGame()
{
    delete ui;
}

void WidgetGame::paintEvent(QPaintEvent *ev)
{
    if(w->wLoadSource->ready&&w->wLoadSource->bgRoom->loaded)
    {
        QPainter painter(this);
        painter.drawPixmap(0,0,w->wLoadSource->bgRoom->pixmap.scaled(size()));
    }
    QWidget::paintEvent(ev);
}

void WidgetGame::init()
{
    game=new CGameClient(this);
    w->client->game=game;
    WidgetGamePlayer *player;
    foreach (player, players) {
        CCheckPointer::deleteObject(player);
    }
    players.resize(w->client->numberOfPlayer-1);
    int box[3]={0};
    switch (players.size()) {
    case 1:
        box[0]=0;
        box[1]=1;
        box[2]=0;
        break;
    case 2:
        box[0]=1;
        box[1]=0;
        box[2]=1;
        break;
    case 3:
        box[0]=1;
        box[1]=1;
        box[2]=1;
        break;
    case 4:
        box[0]=1;
        box[1]=2;
        box[2]=1;
        break;
    case 5:
        box[0]=1;
        box[1]=3;
        box[2]=1;
        break;
    case 6:
        box[0]=2;
        box[1]=2;
        box[2]=2;
        break;
    case 7:
        box[0]=2;
        box[1]=3;
        box[2]=2;
        break;
    case 8:
        box[0]=3;
        box[1]=2;
        box[2]=3;
        break;
    case 9:
        box[0]=3;
        box[1]=3;
        box[2]=3;
        break;
    default:
        break;
    }
    int j;
    for(int i=0;i<players.size();i++)
    {
        if(i<box[0])
        {
            player=new WidgetGamePlayer(ui->frameLeft);
            ui->frameLeft->layout()->addWidget(player);
        }
        else
        {
            j=i-box[0];
            if(j<box[1])
            {
                player=new WidgetGamePlayer(ui->frameTop);
                ui->frameTop->layout()->addWidget(player);
            }
            else
            {
                player=new WidgetGamePlayer(ui->frameRight);
                ui->frameRight->layout()->addWidget(player);
            }
        }
        players[i]=player;
    }
    ui->pushButtonWuXie->setVisible(false);
    ui->pushButtonCancel->setEnabled(false);
    ui->pushButtonOK->setEnabled(false);
    ui->pushButtonEnd->setEnabled(false);
    emit game->startGame();
}

bool WidgetGame::rhPlaying(QByteArray &ba)
{
    switch (ba[0]) {
    case GAME_EXTRETIME:
        COMPARE(ba.length()!=6);
    {
        quint8 c=ba[1];
        COMPARE(c>=game->players.size());
        uint u;
        memcpy(&u,ba.data()+2,4);
        game->players[c]->extreTime=qFromLittleEndian(u);
    }
        break;
    case GAME_CARDINIT:
    case GAME_CHOOSEGENERAL:
    case GAME_GENERALCHOSEN:
    case GAME_INIT:
    case GAME_OPERATION:
    case GAME_OPERATIONRESULT:
    //case GAME_NEEDCARD:
    //case GAME_RANDOMHAND:
        game->serverData.append(ba);
        emit game->newData();
        break;
    default:
        break;
    }
    return true;
}

void WidgetGame::displayTip(const QString &)
{

}

void WidgetGame::hideTip()
{

}

void WidgetGame::playerSelectionInit()
{
    CPlayer* player;
    foreach (player, game->players) {
        PLAYER->setSelected(false);
        PLAYER->setSelectable(false);
    }
}

void WidgetGame::playerSelectionNormal()
{
    CPlayer* player;
    foreach (player, game->players) {
        PLAYER->setNoSelect();
    }
}

void WidgetGame::resizeEvent(QResizeEvent *)
{
    WidgetGamePlayer *player;
    foreach (player, players) {
        player->moveWidgets();
    }
}

QList<CPlayer *> WidgetGame::currentSelectedTargets()
{
    QList<CPlayer *> list;
    CPlayer* player;
    foreach (player, game->players) {
        if(PLAYER->selected) list.append(player);
    }
    return list;
}

void WidgetGame::on_pushButtonCancel_clicked()
{
    if(game->mode==UIMODE_USECARD)
    {
        w->mwCLient->ui->widgetBottom->handsAllOff();
    }
    else if(game->mode==UIMODE_PLAYCARD)
    {
        QList<QVariant> list;
        list.append((int)0);
        auto buf=game->me->opSelect->replyBuf(list);
        w->client->send(buf);
        w->mwCLient->ui->widgetBottom->endSelect();
    }
    else if(game->mode=UIMODE_SHOWCARD)
    {
        QList<QVariant> list;
        list.append(game->me->hands[0]->id);
        auto buf=game->me->opSelect->replyBuf(list);
        w->client->send(buf);
        w->mwCLient->ui->widgetBottom->endSelect();
    }
}

void WidgetGame::on_pushButtonOK_clicked()
{
    if(game->mode==UIMODE_USECARD)
    {
        COperation op(game);
        op.type=OPERATION_USECARD;
        op.parameter.append(game->currentCard->id);
        auto list=currentSelectedTargets();
        CPlayer *player;
        foreach (player, list) {
            op.parameter.append(player->position);
        }
        auto ba=op.requestBuf();
        w->client->send(ba);
    }
    else if(game->mode==UIMODE_PLAYCARD)
    {
        auto list=w->mwCLient->ui->widgetBottom->playCardsList();
        auto buf=game->me->opSelect->replyBuf(list);
        w->client->send(buf);
    }
    else if(game->mode==UIMODE_SHOWCARD)
    {
        auto list=w->mwCLient->ui->widgetBottom->playCardsList();
        auto buf=game->me->opSelect->replyBuf(list);
        w->client->send(buf);
    }
    w->mwCLient->ui->widgetBottom->endSelect();
}

void WidgetGame::displayCard(CCard *card, CPlayer *from)
{

}
