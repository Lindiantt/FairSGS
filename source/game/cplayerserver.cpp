#include "cplayerserver.h"
#include "network/cplayersocket.h"
#include "cgameserver.h"
#include "general/cgeneral.h"
#include "network/define.h"
#include "network/cserver.h"
#include "coperation.h"
#include "general/cskill.h"

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
    switch (type) {
    case OPERATION_USECARD:
        if(currentPhase!=PHASE_PLAYPHASE||game->currentPosition==this->position)
            return false;
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
        card->type->useCard(this,card,players);
    }
        break;
    default:
        break;
    }
    return false;
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

void CPlayerServer::needSelect(COperation *op)
{
    op->needReply(this->position);
    setDefaultAction(true,GAME->server->operationTimeout,std::bind([=](COperation *op){
        QList<QVariant> list;
        list.append((int)0);
        op->replied(this->position,list);
    },op));
    connect(op,op->signalReplied,this,this->handleOperationReplied);
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
        COperation *op=GAME->newOperation(OPERATION_STARTPHASE);
        op->parameter.append((int)PHASE_PLAYPHASE);
        op->deliver();
    });
    auto f=std::bind(endPlayPhase,this);
    setDefaultAction(true,GAME->server->operationTimeout,f);
}

void CPlayerServer::needPlay(quint16 type, QList<quint8> *type2, int number, CPlayer *from, CCard *card, int playMode,
    int cardMode)
{
    CPlayer::needPlay(type,type2,number,from,card,playMode,cardMode);
    COperation *op=GAME->newOperation(OPERATION_NEEDPLAY);
    op->needReply(this->position);
    setDefaultAction(true,GAME->server->operationTimeout,[&](){

    });
    //networkSend(ba);
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
}
