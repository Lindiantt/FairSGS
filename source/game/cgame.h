#ifndef CGAME_H
#define CGAME_H

#include "pch.h"

class CCard;
class CPlayerSkill;
class CPlayer;
class CEvent;
class COperation;

class CGame:public QObject
{
    Q_OBJECT
public:
    CGame();
    ~CGame();
    virtual void s1needRole()=0;//分配身份和主公的选将
    virtual void s2needChooseGeneral()=0;//通知主公武将，分配其他武将的选将
    virtual void s3needAllGeneralAndCards()=0;//通知所有人选将并派发初始卡牌
    virtual void s4skillSetup();//所有技能的GETEVENT调用
    void s5gameStart();//执行游戏开始时注册的事件，如七星和化身等
    virtual void needCard(int,const QList<CPlayer*> &)=0;//需要从牌堆取牌
    void regGameStart(CPlayerSkill*);//注册“游戏开始”事件
    virtual void needWuXieKeJi();//需要无懈可击
    virtual void startNextRound()=0;
    CPlayer* nextAlivePlayer(int);

    bool wuxiePlayed,wuxieAvailable;

    QList<CCard*> deck,deadwood;
    QVector<CCard*> drawnCards;
    QList<CCard*> getDrawnCards();
    uint round,currentPosition;
    char nextPosition;
    bool gameOver;
    QVector<CPlayer*> players;
    CEvent *currentEvent;
    QList<CPlayerSkill*> gameStartRegister;
    QMap<uint,COperation*> operations;
    QLinkedList<std::function<void()>> funcs,insertions;
    int loopCount;
    bool isServer;
    bool boolResult;
    void sortRegister(int cp,QList<CPlayerSkill*> &);
    void addFunc(std::function<void()>);
    void insertFunc(std::function<void()>);
    void smartAddFunc(std::function<void()>);
    void smartInsertFunc(std::function<void()>);
    void insertMultiFunc(std::function<void()>);
    void insertOver();
    void smartInsertOver();
    void roundEnd();
    void checkSignalCount();
    QSet<quint8> allPlayers();
    quint8 countPositionDistance(quint8,quint8);
signals:
    void signalNextRound();
    void newData();
    void judgeSucceeded(CCard*,bool);
    void judgeTip(int);
public slots:
    void handleNextRound();
    void doFuncs();
protected:
    int signalCount;
};

#endif // CGAME_H
