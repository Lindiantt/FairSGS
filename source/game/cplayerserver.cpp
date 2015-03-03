#include "cplayerserver.h"
#include "network/cplayersocket.h"
#include "cgameserver.h"
#include "general/cgeneral.h"
#include "network/define.h"

CPlayerServer::CPlayerServer(CGameServer *game, CPlayerSocket *socket)
{
    this->socketHandle=socket;
    this->game=game;
    socket->player=this;
    socket->state=SOCKETSTATE_PLAYING;
    connect(this,this->generalChosen,game,game->handleGeneralChosen);
    state=PLAYERSTATE_INIT;
    bytesRemain=0;
}

CPlayerServer::~CPlayerServer()
{

}

void CPlayerServer::chooseGeneral(QList<CGeneral *> &list)
{
    QList<quint8> ids;
    foreach (CGeneral* general, list) {
        ids.append(general->id);
    }
    //发送武将信息
}

void CPlayerServer::sendGameStart()
{

}

void CPlayerServer::handleRead()
{
    QByteArray ba;
    if(bytesRemain)
    {
        if(socketHandle->socket->bytesAvailable()<bytesRemain)
            return;
    }
    else
    {
        if(socketHandle->socket->bytesAvailable()>=2)
        {
            ba=socketHandle->socket->read(2);
            memcpy(&bytesRemain,ba.constData(),2);
            bytesRemain=qFromLittleEndian(bytesRemain);
            if(bytesRemain<2)
            {
                socketDisconnected();
                return;
            }
        }
    }
    ba=socketHandle->socket->read(bytesRemain);

}

void CPlayerServer::socketDisconnected()
{

}

void CPlayerServer::deliverCard(QList<CCard *> &cards)
{
    hands=cards;
    //发送信息
}

int CPlayerServer::needSelect(const QString question,int selectionTyp,const QList<QString> &option)
{

}

void CPlayerServer::networkSend()
{

}
