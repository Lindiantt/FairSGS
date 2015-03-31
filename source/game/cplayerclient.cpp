#include "cplayerclient.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include "general/cgeneral.h"
#include "ui/widget/widgettimecount.h"
#include "ui/widget/widgetgameplayer.h"
#include "cgameclient.h"
#include "ui/widget/widgetgame.h"
#include "game/coperation.h"
#include "network/cclient.h"
#include "ui_widgetgameplayer.h"
#include "general/cskill.h"

#define GAME ((CGameClient *)game)
#define COMPARE(a) if(a){w->client->end2("与服务器断开连接");return;}

CPlayerClient::CPlayerClient()
{
    extreTime=w->client->extreTime*1000;
    selected=false;
}

CPlayerClient::~CPlayerClient()
{

}

void CPlayerClient::chooseGeneral(QList<QList<CGeneral *> > & )
{

}

void CPlayerClient::deliverCard(const QList<CCard *> &list)
{
    CPlayer::deliverCard(list);
    wgp->setHands(list.size());
}

void CPlayerClient::startTimeCount(int time, bool useExtreTime)
{
    wgp->widgetTimeCount->startTimeCount(time,useExtreTime);
}

void CPlayerClient::stopTimeCount()
{
    wgp->widgetTimeCount->stop();
}

void CPlayerClient::setGeneral(CGeneral *gen)
{
    CPlayer::setGeneral(gen);
    CImage *img=CImage::avatarById(gen->id);
    if(!img->loaded)
        w->imageLoad(img,wgp,wgp);
}

void CPlayerClient::setKingdom(quint8 kd)
{
    CPlayer::setKingdom(kd);
    w->imageLoad(w->wLoadSource->kingdomFrame[kd],wgp,wgp);
    w->imageLoad(w->wLoadSource->kingdomIcon[kd],wgp->ui->labelKingdom,wgp);
}

void CPlayerClient::setRole(quint8 r)
{
    CPlayer::setRole(r);
    w->imageLoad(w->wLoadSource->roleCorner[r],wgp->ui->labelRole,wgp);
}

void CPlayerClient::setMaxHP(quint8 maxhp)
{
    CPlayer::setMaxHP(maxhp);
    wgp->setMaxHp(maxhp);
}

void CPlayerClient::setHP(char hp)
{
    CPlayer::setHP(hp);
    wgp->setHp(hp);
}

int CPlayerClient::needSelect(const QString &question, int , const QStringList )
{
    auto f=std::bind([&](QString question){
        GAME->widgetGame->displayTip(general->name+" "+question);
        startTimeCount(w->client->operationTimeout,true);
    },question);
    GAME->addFunc(f);
    GAME->addFunc([&](){
        QByteArray ba=GAME->serverData.takeFirst();
        stopTimeCount();
        COMPARE(ba[0]!=GAME_OPERATIONRESULT);
        COperation *op=COperation::newOperation(game,ba);
        COMPARE(!op);
        COMPARE(op->type!=OPERATION_SELECT);
        auto it=op->reply.find(this->position);
        COMPARE(it==op->reply.end());
        GAME->selection=it.value().second[0].toInt();
        emit GAME->newData();
    });
    return 0;
}

void CPlayerClient::getCard(const QList<CCard *> &list, CPlayer *player)
{
    CPlayer::getCard(list,player);
    wgp->setHands(hands.size());
}

void CPlayerClient::setSelectable(bool b)
{
    wgp->setSelectable(b);
    selectable=b;
}

void CPlayerClient::setSelected(bool b)
{
    wgp->setSelected(b);
    selected=b;
}

void CPlayerClient::setNoSelect()
{
    wgp->setNoSelect();
}

void CPlayerClient::phasePlay()
{
    game->smartAddFunc([&](){
        QByteArray ba=GAME->serverData.takeFirst();
        COMPARE(ba[0]!=GAME_OPERATIONRESULT);
        COperation *op=COperation::newOperation(game,ba);
        COMPARE(!op||op->type!=OPERATION_STARTPHASE||op->parameter.size()!=1||op->parameter[0].toInt()!=PHASE_PLAYPHASE);
        this->startPlayPhase();
    });
}

void CPlayerClient::startPlayPhase()
{

}

void CPlayerClient::useCard(CCard *card,QList<CPlayer*> &list)
{
    CPlayer::useCard(card,list);
    hands.removeFirst();
    wgp->setHands(hands.size());
    GAME->widgetGame->displayCard(card,this);
}
