#ifndef CROOM_H
#define CROOM_H

#include "pch.h"

class CGame;
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
    CGame* game;
    QHash<int,CPlayerSocket*> players;
    QList<CPlayerSocket*> onlooker;
    CServer *server;
    uint id;
};

#endif // CROOM_H
