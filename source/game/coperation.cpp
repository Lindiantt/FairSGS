#include "coperation.h"
#include "network/define.h"
#include "cgameserver.h"
#include "cplayerserver.h"
#include "card/ccard.h"

#define PLAYER ((CPlayerServer*)player)

COperation::COperation(CGame *game) : QObject(game)
{
    this->game=game;
    finished=false;
    onlyOne=false;
}

COperation::~COperation()
{

}

bool COperation::fromNotice(QByteArray &ba)
{
    QDataStream ds(&ba,QIODevice::ReadWrite);
    ds.skipRawData(1);
    ds>>this;
    return ds.status()==ds.Ok;
}

void COperation::deliver()
{
    finished=true;
    CGameServer *game=(CGameServer*)this->game;
    QByteArray ba=noticeBuf();
    foreach (auto player, game->players) {
        PLAYER->networkSend(ba);
    }
}

void COperation::selectiveDeliver(const QList<CPlayer *> &playersSeen)
{
    finished=true;
    QByteArray ba=noticeBuf();
    foreach (auto player, playersSeen) {
        PLAYER->networkSend(ba);
    }
}

QByteArray COperation::requestBuf()
{
    QByteArray ba;
    ba.resize(3);
    ba[2]=GAME_OPERATION;
    QDataStream ds(&ba,QIODevice::ReadWrite);
    ds.setVersion(SERIALIZATION_VERSION);
    ds.skipRawData(3);
    ds<<id<<type<<parameter;
    return ba;
}

QByteArray COperation::noticeBuf()
{
    QByteArray ba;
    ba.resize(3);
    ba[2]=GAME_OPERATIONRESULT;
    QDataStream ds(&ba,QIODevice::ReadWrite);
    ds.skipRawData(3);
    ds.setVersion(SERIALIZATION_VERSION);
    ds<<this;
    return ba;
}

QByteArray COperation::noticeBuf3rd()
{
    QByteArray ba;
    ba.resize(3);
    ba[2]=GAME_OPERATIONRESULT;
    QDataStream ds(&ba,QIODevice::ReadWrite);
    ds.skipRawData(3);
    ds.setVersion(SERIALIZATION_VERSION);
    ds<<this;
    ds<<id<<type<<parameter<<thirdPartReply;
    return ba;
}

QByteArray COperation::replyBuf(QList<QVariant> &list)
{
    QByteArray ba;
    ba.resize(7);
    ba[2]=GAME_OPERATIONRESULT;
    uint id=qToLittleEndian(this->id);
    memcpy(ba.data()+3,&id,4);
    QDataStream ds(&ba,QIODevice::ReadWrite);
    ds.skipRawData(7);
    ds<<list;
    return ba;
}

void COperation::send()
{
    auto it=reply.begin();
    CGameServer *game=(CGameServer*)this->game;
    QByteArray ba=requestBuf();
    while(it!=reply.end())
    {
        auto player=game->players[it.key()];
        PLAYER->networkSend(ba);
        it++;
    }
}

void COperation::checkReply()
{
    foreach (auto it, reply) {
        if(!it.first) return;
    }
    finished=true;
    emit signalFinished();
    //deliver();
}

void COperation::needReply(quint8 c)
{
    reply[c].first=false;
}

void COperation::needReply(const QList<quint8> list)
{
    quint8 c;
    foreach (c, list) {
        reply[c].first=false;
    }
}

void COperation::replied(quint8 c, const QList<QVariant> list)
{
    if(finished) return;
    auto it=reply.find(c);
    if(it!=reply.end())
    {
        if(it.value().first) return;
        it.value().first=true;
        it.value().second=list;
        emit signalReplied(c);
        if(onlyOne&&list[0].toInt()!=-1)
        {
            finished=true;
            emit signalFinished();
        }
        else
            checkReply();
    }
}

COperation* COperation::newOperation(CGame *game, QByteArray &ba)
{
    COperation *op=new COperation(game);
    if(op->fromNotice(ba))
    {
        game->operations.insert(op->id,op);
        return op;
    }
    else
    {
        op->deleteLater();
        return nullptr;
    }
}

COperation* COperation::serverRequest(QByteArray &ba)
{
    COperation *op=new COperation(nullptr);
    QDataStream ds(&ba,QIODevice::ReadWrite);
    ds.skipRawData(1);
    ds>>op->id>>op->type>>op->parameter;
    if(ds.status()==ds.Ok)
        return op;
    else
    {
        op->deleteLater();
        return nullptr;
    }
}

QDataStream &operator<<(QDataStream &out, const COperation *op)
{
    out<<op->id<<op->type<<op->parameter<<op->reply;
    return out;
}

QDataStream &operator>>(QDataStream &in, COperation *op)
{
    in>>op->id>>op->type>>op->parameter>>op->reply;
    return in;
}
