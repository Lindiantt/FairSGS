#ifndef CGAME_H
#define CGAME_H

#include "pch.h"

class CCard;
class CSkill;
class CPlayer;
class CEvent;

class CGame:public QObject
{
    Q_OBJECT
public:
    CGame();
    ~CGame();
    virtual void s1needRole()=0;//分配身份和主公的选将
    virtual void s2needChooseGeneral()=0;//通知主公武将，分配其他武将的选将
    virtual void s3needAllGeneralAndCards()=0;//通知所有人选将并派发初始卡牌
    void s4skillSetup();//所有技能的GETEVENT调用
    void s5gameStart();//执行游戏开始时注册的事件，如七星和化身等
    virtual QList<CCard*> drawCard(int)=0;//从牌堆取牌
    void regGameStart(CSkill*,CPlayer*);//注册“游戏开始”事件
    virtual void needWuXieKeJi()=0;//需要无懈可击
    virtual void startNextRound()=0;
    CPlayer* nextAlivePlayer(int);

    QList<CCard*> deck,deadwood,onDesk;
    uint round,currentPosition,nextPosition;
    int currentPhase;
    QVector<CPlayer*> players;
    CEvent *currentEvent;
    QHash<CSkill*,CPlayer*> gameStartRegister;
signals:
    void signalNextRound();
public slots:
    void handleNextRound();
};

#endif // CGAME_H
