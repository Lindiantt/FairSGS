#include "cplayerserver.h"
#include "network/cplayersocket.h"
#include "cgameserver.h"
#include "general/cgeneral.h"
#include "network/define.h"
#include "network/cserver.h"
#include "coperation.h"
#include "general/cskill.h"
#include "card/ccard.h"
#include "game/doondelete.h"
#include "game/cevent.h"
#include "mainwindow.h"
extern MainWindow *w;

#define COMPARE(a) if(a){socketHandle->socket->disconnectFromHost();return false;}
#define GAME ((CGameServer*)game)
#define PLAYER ((CPlayerServer*)player)

CPlayerServer::CPlayerServer(CGameServer *game, CPlayerSocket *socket)
{
    this->socketHandle=socket;
    this->game=game;
    extreTime=game->server->extreTime*1000;
    selecting=false;
    general=nullptr;
    socket->player=this;
    socket->state=SOCKETSTATE_PLAYING;
    connect(this,this->generalChosen,game,game->handleGeneralChosen);
    state=PLAYERSTATE_INIT;
    bytesRemain=0;
    connect(&timerDA,timerDA.timeout,this,this->handleDefaultTimeout);
    timerDA.setSingleShot(true);
    canUseCard=false;
}

CPlayerServer::~CPlayerServer()
{

}

void CPlayerServer::handleDefaultTimeout()
{
    if(defaultAction) defaultAction();
}

void CPlayerServer::setDefaultAction(bool useExtreTime, int time, std::function<void ()> func)
{
    defaultAction=func;
    int rt=(time+1)*1000;
    if(useExtreTime&&extreTime)
    {
        this->useExtreTime=true;
        lastOpTime=QDateTime::currentDateTime();
        regularTime=rt;
        rt+=extreTime;
    }
    else
    {
        this->useExtreTime=false;
    }
    timerDA.start(rt);
}

void CPlayerServer::clearDefaultAction()
{
    timerDA.stop();
    defaultAction=nullptr;
}

void CPlayerServer::chooseGeneral(QList<QList<CGeneral*>> &list)
{
    QByteArray sendbuf;
    sendbuf.resize(4);
    sendbuf[2]=GAME_CHOOSEGENERAL;
    sendbuf[3]=list.length();
    quint16 id;
    quint8 l;
    foreach (auto glist, list) {
        availabelGIDs.insert(glist[0]->id);
        id=qToLittleEndian(glist[0]->id);
        sendbuf.append((char*)&id,2);
        if(glist.size()>1)
        {
            l=glist.size()-1;
            sendbuf.append(l);
            for(int i=1;i<glist.size();i++)
            {
                availabelGIDs.insert(glist[i]->id);
                id=qToLittleEndian(glist[i]->id);
                sendbuf.append((char*)&id,2);
            }
        }
        else
            sendbuf.append((char)0);
    }
    auto f=std::bind(generalOK,this,*availabelGIDs.begin());
    setDefaultAction(false,GAME->server->choiceTimeout,f);
    networkSend(sendbuf);
}

void CPlayerServer::sendGameStart()
{
    QByteArray sendbuf;
    int l=GAME->server->numberOfPlayer+4;
    sendbuf.resize(l);
    sendbuf[2]=GAME_INIT;
    sendbuf[3]=role;
    for(int i=4;i<l;i++)
    {
        auto player=game->players[i-4];
        sendbuf[i]=PLAYER->socketHandle->roomPosition;
    }
    networkSend(sendbuf);
}

bool CPlayerServer::rhPlaying(QByteArray &ba)
{
    switch (ba[0]) {
    case GAME_CHOOSEGENERAL:
        COMPARE(ba.length()!=3);
        if(!general)
        {
            quint16 id;
            memcpy(&id,ba.data()+1,2);
            id=qFromLittleEndian(id);
            COMPARE(!generalOK(id));
        }
        break;
    case GAME_OPERATIONRESULT:
        COMPARE(ba.length()<6);
    {
        uint u;
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        auto it=game->operations.find(u);
        if(it!=game->operations.end())
        {
            COperation *op=it.value();
            if(op->reply.find(this->position)==op->reply.end()) return true;
            if(!op->finished)
            {
                QList<QVariant> list;
                QDataStream ds(&ba,QIODevice::ReadWrite);
                ds.setVersion(SERIALIZATION_VERSION);
                ds.skipRawData(5);
                ds>>list;
                if(ds.status()==ds.Ok&&!list.isEmpty())
                    op->replied(this->position,list);
                else
                {
                    COMPARE(true);//数据损坏
                }
            }
        }
    }
        break;
    case GAME_OPERATION:
    {
        COMPARE(ba.length()<6);
        uint id;
        quint8 type;
        QList<QVariant> list;
        QDataStream ds(&ba,QIODevice::ReadWrite);
        ds.skipRawData(1);
        ds>>id>>type>>list;
        COMPARE(ds.status()!=ds.Ok);
        checkActiveOperation(type,list);
    }
        break;
    default:
        break;
    }
    return true;
}

bool CPlayerServer::checkActiveOperation(quint8 type, QList<QVariant> &list)
{
    qDebug("receive active operation");
    switch (type) {
    case OPERATION_USECARD:
        if(!canUseCard)
        {
            qDebug("can not use card");
            return false;
        }
        canUseCard=false;
        COMPARE(list.isEmpty());
    {
        bool b;
        CCard *card=CCard::find(list[0].toUInt(&b));
        COMPARE(!b||!card);
        if(!hands.contains(card)) return false;
        list.removeFirst();
        QList<CPlayer*> players;
        QVariant var;
        foreach (var, list) {
            int i=var.toUInt(&b);
            COMPARE(!b||i>=game->players.size());
            players.append(game->players[i]);
        }
        if(!card->type->cardUseCheck(this,card,players)) return false;
        useCard(card,players);
        qDebug("usecard");
    }
        break;
    default:
        qDebug("undefined active operation");
        return false;
    }
    return true;
}

bool CPlayerServer::generalOK(quint16 id)
{
    clearDefaultAction();
    if(!availabelGIDs.contains(id)) return false;
    this->setGeneral(CGeneral::find(id));
    emit generalChosen();
    return true;
}

void CPlayerServer::socketDisconnected()
{

}

void CPlayerServer::deliverCard(const QList<CCard *> &cards)
{
    CPlayer::deliverCard(cards);
    QByteArray sendbuf;
    sendbuf.resize(3+cards.size());
    sendbuf[2]=GAME_CARDINIT;
    for(int i=0;i<cards.size();i++)
    {
        sendbuf[i+3]=cards[i]->id;
    }
    networkSend(sendbuf);
}

void CPlayerServer::needSelect(const QString &, int , const QList<QVariant> values,
                               const QList<QVariant> &defaultResult, int min, int max)
{
    CEvent *ev=new CEvent(game);
    GAME->currentOperation=GAME->newOperation(OPERATION_SELECT);
    GAME->currentOperation->needReply(this->position);
    setDefaultAction(true,GAME->server->operationTimeout,std::bind(standardDefaultAction,this,GAME->currentOperation));
    connect(GAME->currentOperation,GAME->currentOperation->signalReplied,this,this->handleOperationReplied);
    GAME->currentOperation->send();
    auto f=std::bind([&](const QList<QVariant> values,const QList<QVariant> &defaultResult,int min,int max){
        QList<QVariant> &list=GAME->currentOperation->reply[position].second;
        DoOnDelete d([&](){
            list=defaultResult;
            GAME->currentOperation->deliver();
            selectionList=list;
            emit game->newData();
        });
        QVariant var;
        if(list.size()<min||list.size()>max) return;
        foreach (var, defaultResult) {
            int i=var.toUInt();
            if(i>=values.size()) return;
        }
        d.on=false;
        GAME->currentOperation->deliver();
        selectionList=list;
        emit game->newData();
    },values,defaultResult,min,max);
    ev->addFunc(f);
    if(GAME->currentOperation->finished)
        emit game->newData();
    else
        connect(GAME->currentOperation,GAME->currentOperation->signalFinished,game,game->doFuncs);
}

void CPlayerServer::handleOperationReplied(quint8 c)
{
    if(c==this->position&&useExtreTime)
    {
        qint64 tm=lastOpTime.msecsTo(QDateTime::currentDateTime())-regularTime;
        if(tm>0)
        {
            if(extreTime<=tm)
                extreTime=0;
            else
                extreTime-=tm;
            QByteArray ba;
            ba.resize(8);
            ba[2]=GAME_EXTRETIME;
            ba[3]=this->position;
            uint u=qToLittleEndian(extreTime);
            memcpy(ba.data()+4,&u,4);
            foreach (auto player, GAME->players) {
                PLAYER->networkSend(ba);
            }
        }
    }
}

void CPlayerServer::networkSend(QByteArray &ba)
{
    if(!offline)
        socketHandle->send(ba);
    else
    {
        if(defaultAction)
        {
            std::function<void()> da=defaultAction;
            clearDefaultAction();
            da();
        }
    }
}

void CPlayerServer::setOffline(bool b)
{
    CPlayer::setOffline(b);
}

void CPlayerServer::phasePlay()
{
    game->smartAddFunc([&](){
        canUseCard=true;
        COperation *op=GAME->newOperation(OPERATION_STARTPHASE);
        op->parameter.append((int)PHASE_PLAYPHASE);
        op->deliver();
    });
    auto f=std::bind(endPlayPhase,this);
    setDefaultAction(true,GAME->server->operationTimeout,f);
}

void CPlayerServer::needPlay(quint8 suit, quint16 type, const QList<quint8> type2, int number, CPlayer *from, CCard *card, int playMode,
    int cardMode, const QList<CPlayer *> targets)
{
    CPlayer::needPlay(suit,type,type2,number,from,card,playMode,cardMode,targets);
    COperation *op=GAME->newOperation(OPERATION_NEEDPLAY);
    op->needReply(this->position);
    setDefaultAction(true,GAME->server->operationTimeout,std::bind(standardDefaultAction,this,op));
    connect(op,op->signalReplied,this,this->handleOperationReplied);
    op->send();
    auto f=std::bind([&](quint8 suit,COperation *op,quint16 type, QList<quint8> type2, int number, CPlayer *from, CCard *card,
                     int playMode,int cardMode,QList<CPlayer *> targets){
        CEvent *ev=new CEvent(game);
        DoOnDelete d([=](){
            ev->funcs.clear();
            QList<QVariant> &list=op->reply[this->position].second;
            list.clear();
            list.append((int)0);
            op->deliver();
            emit game->newData();
        });
        QList<QVariant> &list=op->reply[this->position].second;
        bool b;
        if(list.size()<1) return;
        int n=list[0].toUInt(&b);
        if(!b) return;
        if(n==10000)
        {
            //使用技能
            return;
        }
        if(n!=number||n>=list.size()) return;
        playedTargets.clear();
        if(!targets.isEmpty())
        {
            for(int i=n+1;i<list.size();i++)
            {
                int pos=list[i].toUInt();
                if(pos>=game->players.size()) return;
                if(!targets.contains(game->players[pos])) return;
                playedTargets.append(game->players[pos]);
            }
        }
        else if(n!=list.size()-1)
            return;
        QList<CCard*> cards;
        for(int i=0;i<n;i++)
        {
            quint8 id=list[i+1].toUInt(&b);
            if(!b) return;
            CCard* cd=CCard::find(id);
            if(!cd) return;
            if(!containsCard(cd,cardMode)) return;
            if(!(suit&cd->suit))
            if(!(cd->type->type1&type))
            {
                if(!type2.contains(cd->type->type2)) return;
            }
            cards.append(cd);
        }
        d.on=false;
        op->deliver();
        this->phaseCallback(PHASE_LOSECARD,&cards,(void*)playMode);
        if(playMode==PLAYMODE_USE&&number==1)
        {
            if(type==CARDTYPE_SHA)
            {
                if(playedTargets.isEmpty()) return;
                if(!cards[0]->type->cardUseCheck(this,cards[0],playedTargets)) return;
                auto f=std::bind(cards[0]->type->useCard,cards[0]->type,this,cards[0],playedTargets);
                ev->addFunc(f);
            }
        }
        CCard *cd;
        foreach (cd, cards) {
            loseCard(cd);
            ev->addCard(cd);
        }
        auto f=std::bind([&](QList<CCard*> cards){
            cardPlayed=true;
            playedCards=cards;
            //todo:各种phasecallback
            emit game->newData();
        },cards);
        ev->addFunc(f);
        emit game->newData();
    },suit,op,type,type2,number,from,card,playMode,cardMode,targets);
    game->smartInsertFunc(f);
    if(op->finished)
        emit game->newData();
    else
        connect(op,op->signalFinished,game,game->doFuncs);
}

void CPlayerServer::standardDefaultAction(COperation *op)
{
    QList<QVariant> list;
    list.append((int)0);
    op->replied(this->position,list);
}

void CPlayerServer::useCard(CCard *card, QList<CPlayer *> &list)
{
    COperation *op=GAME->newOperation(OPERATION_USECARD);
    op->parameter.append(this->position);
    op->parameter.append(card->id);
    CPlayer *player;
    foreach (player, list) {
        op->parameter.append(player->position);
    }
    op->deliver();
    CPlayer::useCard(card,list);
    CEvent *ev=game->currentEvent->findParent(EVENT_PHASE);
    ev->addFunc([&](){
        canUseCard=true;
        qDebug("card use over");
    });
}

void CPlayerServer::endPlayPhase()
{
    canUseCard=false;
    //emit game->newData();
}

void CPlayerServer::phaseDiscard()
{
    int i;
    i=0;
}

void CPlayerServer::needShow()
{
    GAME->currentOperation=GAME->newOperation(OPERATION_NEEDSHOW);
    GAME->currentOperation->needReply(this->position);
    setDefaultAction(true,GAME->server->operationTimeout,[&](){
        QList<QVariant> list;
        list.append(hands[0]->id);
        GAME->currentOperation->replied(this->position,list);
    });
    connect(GAME->currentOperation,GAME->currentOperation->signalReplied,this,this->handleOperationReplied);
    GAME->currentOperation->send();
    auto f=[&](){
        QList<QVariant> &list=GAME->currentOperation->reply[position].second;
        DoOnDelete d([&](){
            list.clear();
            list.append(hands[0]->id);
            GAME->currentOperation->deliver();
            selectionList=list;
            emit game->newData();
        });
        if(list.size()!=1) return;
        int i=list[0].toUInt();
        CCard *card=CCard::find(i);
        if(!card||!hands.contains(card)) return;
        cardShown=card;
        d.on=false;
        GAME->currentOperation->deliver();
        emit game->newData();
    };
    GAME->smartInsertFunc(f);
    if(GAME->currentOperation->finished)
        emit game->newData();
    else
        connect(GAME->currentOperation,COperation::signalFinished,game,game->doFuncs);
}

void CPlayerServer::getRandomHand(const QList<CPlayer*> &knownList)
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    int r=qrand()%hands.size();
    cardShown=hands[r];
    COperation *op=GAME->newOperation(OPERATION_RANDOMHAND);
    op->parameter.append(cardShown->id);
    op->selectiveDeliver(knownList);
    emit game->newData();
}
