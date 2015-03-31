#ifndef CROOM_H
#define CROOM_H

#include "pch.h"

class CGameServer;
class CPlayerSocket;
class CServer;

class CRoom:public QObject
{
    Q_OBJECT
public:
    CRoom(CServer*);
    ~CRoom();
    int emptySeat();
    void join(CPlayerSocket*);
    void sendRoomInfo();
    bool rhInRoomPlayer(QByteArray &,CPlayerSocket *);
    void destroy();
    void leave(CPlayerSocket *);
    CGameServer* game;
    QMap<int,CPlayerSocket*> players;
    QList<CPlayerSocket*> onlooker;
    CServer *server;
    uint id;
private:
    void setReady(bool,CPlayerSocket *);

    void startGame();
};

#endif // CROOM_H
