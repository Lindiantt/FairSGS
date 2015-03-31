#include "croom.h"
#include "cserver.h"
#include "cplayersocket.h"
#include "game/cgameserver.h"

#define COMPARE(a,b) if((a)!=(b)){handle->deleteLater();return false;}

CRoom::CRoom(CServer* server)
{
    this->server=server;
    game=nullptr;
}

CRoom::~CRoom()
{

}

int CRoom::emptySeat()
{
    for(int i=0;i<server->numberOfPlayer;i++)
    {
        if(!players.contains(i))
            return i;
    }
    return -1;
}

void CRoom::join(CPlayerSocket *handle)
{
    int seat=emptySeat();
    CPlayerSocket *player;
    QByteArray sendbuf;
    handle->ready=false;
    if(handle->hostAddress.toIPv4Address()==0x7f000001)
        handle->admin=true;
    else
        handle->admin=false;
    handle->roomPosition=seat;
    if(!players.isEmpty())
    {
        handle->roomHost=false;
        sendbuf.resize(3);
        sendbuf[2]=INROOM_NEWPLAYER;
        sendbuf.append(handle->info());
        foreach (player, players) {
            player->send(sendbuf);
        }
    }
    else
        handle->roomHost=true;
    handle->state=SOCKETSTATE_PLAYER;
    players.insert(seat,handle);
    sendbuf.resize(4);
    sendbuf[2]=seat;
    sendbuf[3]=players.size();
    foreach (player, players) {
        sendbuf.append(player->info());
    }
    handle->currentRoom=this;
    handle->send(sendbuf);
}

bool CRoom::rhInRoomPlayer(QByteArray &ba, CPlayerSocket *handle)
{
    switch (ba[0]) {
    case INROOM_READY:
        COMPARE(ba.length(),1);
        setReady(true,handle);
        break;
    case INROOM_UNREADY:
        COMPARE(ba.length(),1);
        setReady(false,handle);
        break;
    case INROOM_CLIMB:
        COMPARE(ba.length(),1);
        if(!handle->roomHost&&onlooker.size()<server->maxOnlooker)
        {
            QByteArray ba;
            ba.resize(3);
            ba[2]=INROOM_CLIMB_OK;
            handle->send(ba);
            leave(handle);
            onlooker.append(handle);
            handle->state=SOCKETSTATE_ONLOOKER;
        }
        break;
    case INROOM_RETURN:
        COMPARE(ba.length(),1);
        if(server->maxRoom!=1)
        {
            leave(handle);
            if(handle->roomHost&&!players.isEmpty())
            {
                auto it=players.begin();
                CPlayerSocket *player=it.value();
                player->roomHost=true;
            }
            QByteArray sendbuf;
            sendbuf.resize(3);
            sendbuf[2]=INROOM_RETURN_OK;
            handle->send(sendbuf);
            handle->chooseRoom();
        }
        break;
    case INROOM_STARTGAME:
        COMPARE(ba.length(),1);
        if(players.size()==server->numberOfPlayer)
        {
            CPlayerSocket *player;
            foreach (player, players) {
                if(!player->ready&&!player->roomHost)
                    return true;
            }
            startGame();
        }
        break;
    case INROOM_KICK:
        COMPARE(ba.length(),2);
        if(handle->roomHost||handle->admin)
        {
            auto it=players.find(ba[1]);
            if(it!=players.end())
            {
                CPlayerSocket *player=it.value();
                leave(player);
                if(server->maxRoom==1)
                {
                    player->deleteLater();
                }
                else
                {
                    QByteArray sendbuf;
                    sendbuf.resize(3);
                    sendbuf[2]=INROOM_KICK;
                    player->send(sendbuf);
                    player->chooseRoom();
                }
            }
        }
        break;
    case INROOM_MOVE:
        COMPARE(ba.length(),2);
        if(ba[1]>=(char)0&&ba[1]<server->numberOfPlayer)
        {
            auto it=players.find(ba[1]);
            if(it==players.end())
            {
                QByteArray sendbuf;
                sendbuf.resize(5);
                sendbuf[2]=INROOM_MOVE;
                sendbuf[3]=handle->roomPosition;
                sendbuf[4]=ba[1];
                players.remove(handle->roomPosition);
                handle->roomPosition=ba[1];
                players.insert(ba[1],handle);
                CPlayerSocket *player;
                foreach (player, players) {
                    player->send(sendbuf);
                }
            }
        }
        break;
    default:
        break;
    }
    return true;
}

void CRoom::setReady(bool b, CPlayerSocket *handle)
{
    if(handle->ready==b||handle->roomHost) return;
    CPlayerSocket *player;
    QByteArray ba;
    handle->ready=b;
    ba.resize(4);
    ba[2]=b?INROOM_READY:INROOM_UNREADY;
    ba[3]=handle->roomPosition;
    foreach (player, players) {
        player->send(ba);
    }
}

void CRoom::leave(CPlayerSocket *handle)
{
    players.remove(handle->roomPosition);
    QByteArray ba;
    if(players.isEmpty())
    {
        destroy();
    }
    else
    {
        CPlayerSocket *player;
        ba.resize(4);
        ba[2]=INROOM_PLAYERLEAVE;
        ba[3]=handle->roomPosition;
        foreach (player, players) {
            player->send(ba);
        }
    }
}

void CRoom::destroy()
{
    if(server->maxRoom>1)
    {
        server->rooms.remove(this->id);
        this->deleteLater();
    }
    if(!onlooker.isEmpty())
    {
        QByteArray sendbuf;
        sendbuf.resize(3);
        sendbuf[2]=INROOM_DESTROY;
        CPlayerSocket *player;
        foreach (player, onlooker) {
            player->send(sendbuf);
            if(server->maxRoom==1)
            {
                player->joinRoom(this);
            }
            else
            {
                player->chooseRoom();
            }
        }
    }
}

void CRoom::startGame()
{
    CPlayerSocket *player;
    QByteArray ba,bp;
    ba.resize(3);
    ba[2]=INROOM_STARTGAME;
    bp.resize(2);
    uint ip;
    quint16 port;
    foreach (player, players) {
        ip=qToLittleEndian(player->hostAddress.toIPv4Address());
        bp.append((char*)&ip,4);
        if(player->udpAvailable)
        {
            bp.append((char)1);
            port=qToLittleEndian(player->udpPort);
            bp.append((char*)&port,2);
            ip=qToLittleEndian(player->udpCode);
            bp.append((char*)&ip,4);
        }
        else
            bp.append((char)0);
    }
    foreach (player, players) {
        player->state=CLIENTSTATE_PLAYING;
        player->send(ba);
        player->send(bp);
    }
    foreach (player, onlooker) {
        player->state=CLIENTSTATE_ONLOOKING;
        player->send(ba);
        player->send(bp);
    }
    game=new CGameServer(server,this);
    game->init();
}
