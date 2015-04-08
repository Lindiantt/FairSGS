#ifndef CGAMECLIENT_H
#define CGAMECLIENT_H

#include "game/cgame.h"
class CPlayerClient;
class WidgetGame;
class CPlayerClientMe;

class CGameClient : public CGame
{
    Q_OBJECT
public:
    CGameClient(WidgetGame *);
    ~CGameClient();
    void s1needRole();
    void s2needChooseGeneral();
    void s3needAllGeneralAndCards();
    void needCard(int,const QList<CPlayer*> &);
    void needWuXieKeJi();
    void startNextRound();
    //QByteArray waitForData();
    QLinkedList<QByteArray> serverData;
    WidgetGame *widgetGame;
    CPlayerClientMe *me;
    COperation *opWuXie;
    void cardDirection(CCard*);
    void cardOKCheck();
    void checkSecondStage();
    void targetClicked(CPlayer*);
    bool twoStage;
    int twoStageLock;
    CCard *currentCard;
    CPlayer* firstTarget,*secondTarget;
    int targetMin,targetMax;
    bool canSelectTarget;
    int mode;
signals:
    void startGame();
protected:
    bool bhGeneralChosen(QByteArray &);
    bool bhChooseGeneral(QByteArray &);
    bool generalChosen;
    quint16 defaultId;
protected slots:
    void handleGeneralChosen(int);
};

#endif // CGAMECLIENT_H
