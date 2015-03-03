#include "croom.h"
#include "cserver.h"
#include "cplayersocket.h"

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
    sendbuf.resize(3);
    sendbuf[2]=players.size();
    foreach (player, players) {
        sendbuf.append(player->info());
    }
    handle->send(sendbuf);
}
