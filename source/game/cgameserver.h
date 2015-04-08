#ifndef CGAMESERVER_H
#define CGAMESERVER_H

class CServer;
class CRoom;
class CPlayerServer;
class CGeneral;

#include "game/cgame.h"

class CGameServer : public CGame
{
    Q_OBJECT
public:
    CGameServer(CServer*,CRoom*);
    ~CGameServer();
    void init();
    void cardDeliver();
    void s1needRole();
    void s2needChooseGeneral();
    void s3needAllGeneralAndCards();
    void drawCard(int);
    void needCard(int,const QList<CPlayer*> &);
    void needWuXieKeJi();
    void startNextRound();
    CServer* server;
    CRoom* room;
    QHash<int,QList<CGeneral*>> availableGenerals;
    COperation* newOperation(quint8 type);
    COperation* currentOperation;
public slots:
    void handleGeneralChosen();
private:
    int chosen;
};

#endif // CGAMESERVER_H
