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
#include "card/ccard.h"
#include "game/cevent.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "ui/widget/widgetbottom.h"
#include "game/cplayerclientme.h"

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

void CPlayerClient::needSelect(const QString &question,int ,const QList<QVariant> values,
                               const QList<QVariant> &,int min,int max)
{
    CEvent *ev=new CEvent(game);
    auto f=std::bind([&](QString question){
        GAME->widgetGame->displayTip(general->name+" "+question);
        startTimeCount(w->client->operationTimeout,true);
    },question);
    ev->addFunc(f);
    auto f2=std::bind([&](uint limit,int min,int max){
        QByteArray ba=GAME->serverData.takeFirst();
        stopTimeCount();
        COMPARE(ba[0]!=GAME_OPERATIONRESULT);
        COperation *op=COperation::newOperation(game,ba);
        COMPARE(!op);
        COMPARE(op->type!=OPERATION_SELECT);
        auto it=op->reply.find(this->position);
        COMPARE(it==op->reply.end());
        selectionList=it.value().second;
        COMPARE(selectionList.size()<min||selectionList.size()>max);
        QVariant var;
        foreach (var, selectionList) {
            COMPARE(var.toUInt()>=limit);
        }
        emit GAME->newData();
    },values.size(),min,max);
    ev->addFunc(f2);
    emit game->newData();
}

void CPlayerClient::getCard(const QList<CCard *> &list, CPlayer *player)
{
    wgp->setHands(hands.size());
    CPlayer::getCard(list,player);
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
    game->smartAddFunc([](){});
    game->smartAddFunc([&](){
        QByteArray ba=GAME->serverData.takeFirst();
        COMPARE(ba[0]!=GAME_OPERATIONRESULT);
        COperation *op=COperation::newOperation(game,ba);
        COMPARE(!op||op->type!=OPERATION_STARTPHASE||op->parameter.size()!=1||op->parameter[0].toInt()!=PHASE_PLAYPHASE);
        playPhaseInit();
    });
    auto f=std::bind(playPhaseResponse,this);
    game->smartAddFunc(f);
}

void CPlayerClient::playPhaseResponse()
{
    QByteArray ba=GAME->serverData.takeFirst();
    COMPARE(ba[0]!=GAME_OPERATIONRESULT);
    COperation *op=COperation::newOperation(game,ba);
    COMPARE(!op);
    switch (op->type) {
    case OPERATION_ENDPHASE:
        emit game->newData();
        return;
    case OPERATION_USECARD:
    {
        COMPARE(op->parameter.size()<2);
        CPlayer *player;
        quint8 id=op->parameter[0].toUInt();
        COMPARE(id>=game->players.size());
        player=game->players[id];
        COMPARE(player!=this);
        id=op->parameter[1].toUInt();
        CCard* card=CCard::find(id);
        COMPARE(!card);
        QList<CPlayer*> list;
        for(int i=2;i<op->parameter.size();i++)
        {
            id=op->parameter[i].toUInt();
            COMPARE(id>=game->players.size());
            list.append(game->players[id]);
        }
        COMPARE(!card->type->cardUseCheck(player,card,list));
        useCard(card,list);
    }
        break;
    default:
        COMPARE(true);
    }
    CEvent *ev=game->currentEvent->findParent(EVENT_PHASE);
    ev->addFunc([&](){
        auto f=std::bind(playPhaseResponse,this);
        playPhaseInit();
        game->smartInsertFunc(f);
    });
}

void CPlayerClient::playPhaseInit()
{

}

void CPlayerClient::roundInit()
{
    w->imageLoad(w->wLoadSource->framePlayerPlaying,wgp->labelFrame);
    wgp->labelFrame->show();
    CPlayer::roundInit();
}

void CPlayerClient::phaseCallback(int phase, void *extre, void *extre2, void *extre3, void *extre4)
{
    CPlayer::phaseCallback(phase,extre,extre2,extre3,extre4);
    if(phase<=15&&phase%3==0)
    {
        showPhase(w->wLoadSource->phase[phase/3]);
    }
}

void CPlayerClient::showPhase(CImage *img)
{
    w->imageLoad(img,wgp->labelPhase,wgp);
    wgp->labelPhase->show();
}

void CPlayerClient::needPlay(quint8 suit, quint16 type, const QList<quint8> type2, int number, CPlayer *from, CCard *card,
                             int playMode, int cardMode, const QList<CPlayer *> targets)
{
    CPlayer::needPlay(suit,type,type2,number,from,card,playMode,cardMode,targets);
    startTimeCount(w->client->operationTimeout,true);
    game->smartInsertFunc(std::bind(needPlayCallback,this,suit,type,type2,number,from,card,playMode,cardMode,targets));
}

void CPlayerClient::needPlayCallback(quint8 suit,quint16 type,const QList<quint8> type2,int number,CPlayer* from,
        CCard* card,int playMode,int cardMode,const QList<CPlayer*> targets)
{
    this->stopTimeCount();
    if(this==GAME->me)
    {
        GAME->me->opSelect->deleteLater();
    }
    w->mwCLient->ui->widgetBottom->endSelect();
    QByteArray ba=GAME->serverData.takeFirst();
    COMPARE(ba[0]!=GAME_OPERATIONRESULT);
    COperation* op=COperation::newOperation(game,ba);
    COMPARE(!op||op->type!=OPERATION_NEEDPLAY||op->reply[this->position].second.isEmpty());
    QList<QVariant> &list=op->reply[this->position].second;
    int n=list[0].toInt();
    COMPARE(n>=list.size());
    if(!n)
    {
        emit game->newData();
        return;
    }
    if(n==10000)
    {
        //使用技能
        emit game->newData();
        return;
    }
    CCard *cd;
    CEvent *ev=new CEvent(game);
    playedCards.clear();
    for(int i=0;i<n;i++)
    {
        cd=CCard::find(list[i+1].toUInt());
        COMPARE(!cd);
        playedCards.append(cd);
    }
    playedTargets.clear();
    for(int i=n+1;i<list.size();i++)
    {
        int pos=list[i].toUInt();
        COMPARE(pos>=game->players.size());
        playedTargets.append(game->players[pos]);
    }
    this->phaseCallback(PHASE_LOSECARD,&playedCards,(void*)playMode);
    if(playMode==PLAYMODE_USE&&number==1)
    {
        if(type==CARDTYPE_SHA)
        {
            COMPARE(playedTargets.isEmpty());
            COMPARE(!playedCards[0]->type->cardUseCheck(this,playedCards[0],playedTargets));
            auto f=std::bind(playedCards[0]->type->useCard,playedCards[0]->type,this,playedCards[0],playedTargets);
            ev->addFunc(f);
        }
    }
    foreach (cd, playedCards) {
        loseCard(cd);
        ev->addCard(cd);
    }
    auto f=std::bind([&](){
        cardPlayed=true;
        //todo:各种phasecallback
        emit game->newData();
    });
    ev->addFunc(f);
    emit game->newData();
}

void CPlayerClient::removeHand(CCard *card)
{
    hands.removeFirst();
    GAME->widgetGame->displayCard(card,this);
    wgp->setHands(hands.size());
}

void CPlayerClient::needShow()
{
    startTimeCount(w->client->operationTimeout,true);
    game->smartInsertFunc(std::bind(needShowCallback,this));
}

void CPlayerClient::needShowCallback()
{
    stopTimeCount();
    if(this==GAME->me)
    {
        GAME->me->opSelect->deleteLater();
    }
    QByteArray ba=GAME->serverData.takeFirst();
    COMPARE(ba[0]!=GAME_OPERATIONRESULT);
    COperation* op=COperation::newOperation(game,ba);
    COMPARE(!op||op->type!=OPERATION_NEEDSHOW);
    QList<QVariant> &list=op->reply[position].second;
    COMPARE(list.size()!=1);
    int i=list[0].toUInt();
    CCard* card=CCard::find(i);
    COMPARE(!card);
    cardShown=card;
    GAME->widgetGame->displayCard(card,this);
}

void CPlayerClient::getRandomHand(const QList<CPlayer *> &knownList)
{
    if(knownList.contains(GAME->me))
    {
        game->smartInsertFunc([&](){
            QByteArray ba=GAME->serverData.takeFirst();
            COMPARE(ba[0]!=GAME_OPERATIONRESULT);
            COperation* op=COperation::newOperation(game,ba);
            COMPARE(!op||op->type!=OPERATION_RANDOMHAND);
            QList<QVariant> &list=op->parameter;
            COMPARE(list.size()!=1);
            cardShown=CCard::find(list[0].toUInt());
            COMPARE(!cardShown);
            emit game->newData();
        });
    }
    else
    {
        cardShown=nullptr;
        emit game->newData();
    }
}
