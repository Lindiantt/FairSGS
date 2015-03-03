#ifndef CPLAYERSERVER_H
#define CPLAYERSERVER_H

#include "game/cplayer.h"

class CPlayerSocket;
class CGameServer;

class CPlayerServer:public CPlayer
{
    Q_OBJECT
public:
    CPlayerServer(CGameServer*,CPlayerSocket*);
    ~CPlayerServer();
    void chooseGeneral(QList<CGeneral*> &);
    void deliverCard(QList<CCard*> &);//派牌及其他武将信息
    int needSelect(const QString question,int selectionType=SELECTTYPE_YESNO,
                               const QList<QString> &option=QList<QString>());
    void sendGameStart();
    void handleRead();
    void socketDisconnected();
    void networkSend();
    int state;
    quint16 bytesRemain;
    CPlayerSocket *socketHandle;
    CGameServer* game;
};

#endif // CPLAYERSERVER_H
