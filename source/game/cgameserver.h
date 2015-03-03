#ifndef CGAMESERVER_H
#define CGAMESERVER_H

class CServer;
class CRoom;
class CPlayerServer;

#include "game/cgame.h"

class CGameServer : public CGame
{
    Q_OBJECT
public:
    CGameServer(CServer*,CRoom*);
    ~CGameServer();
    void cardDeliver();
    void s1needRole();
    void s2needChooseGeneral();
    void s3needAllGeneralAndCards();
    QList<CCard*> drawCard(int);
    CServer* server;
    CRoom* room;
    QVector<CPlayerServer*> players;
public slots:
    void handleGeneralChosen();
};

#endif // CGAMESERVER_H
