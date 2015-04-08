#ifndef CPLAYER_H
#define CPLAYER_H

#include "pch.h"

class CGeneral;
class CCardZhuangbei;
class CGame;
class CPlayerSkill;
class CInjured;
class CCard;

#define SELECTTYPE_YESNO 0
#define SELECTTYPE_WUXIEKEJI 1
#define SELECTTYPE_CUSTOM 2
#define SELECTTYPE_PLAYERCARD 3
#define SELECTTYPE_PLAYER 4

#define PLAYMODE_PLAY 0
#define PLAYMODE_USE 1
#define PLAYMODE_DISCARD 2

#define CARDMODE_HANDS 1
#define CARDMODE_ZHUANGBEI 2
#define CARDMODE_BOTH 3

class CPlayer:public QObject
{
    Q_OBJECT
public:
    CPlayer();
    ~CPlayer();
    virtual void phaseCallback(int phase, void* extre=nullptr, void* extre2=nullptr,void* extre3=nullptr,
                       void* extre4=nullptr);//技能回调函数
    virtual void chooseGeneral(QList<QList<CGeneral*>> &)=0;//选将
    virtual void deliverCard(const QList<CCard*> &);//初始派牌
    virtual void roundInit();//执行回合
    void regPhase(int phase,CPlayerSkill* skill);//技能对特定的时机进行注册
    void unregPhase(int phase,CPlayerSkill* skill);//技能对特定的时机解除注册
    void discardCard(QList<CCard*> &,int reason);//把牌放入弃牌堆
    void injured(CInjured*);//受伤
    void loseHP(int);//体力流失
    //bool canBeTarget(CCard*);//是否能成为牌的目标
    virtual void getCard(const QList<CCard*> &list,CPlayer* player=nullptr);//获得牌
    virtual void goingToDie();//濒死状态
    virtual void needSelect(const QString &question,int selectionType,const QList<QVariant> values,
                            const QList<QVariant> &defaultResult,int min,int max)=0;
                           //需要玩家做选择
    void needSelectKingdom();
    void loseCard(CCard*);//失去牌
    virtual void setOffline(bool);
    virtual void judge();
    virtual void panDingQuAdd(CCard*);
    virtual void panDingQuRemove(CCard*);
    virtual void needPlay(quint8 suit,quint16 type,const QList<quint8> type2,int number,CPlayer* from,
                          CCard* card,int playMode,int cardMode,const QList<CPlayer*> targets);
    virtual void endPlayPhase();
    virtual void useCard(CCard*,QList<CPlayer*> &);
    virtual void setZhuangBei(int pos,CCard*);
    virtual void removeHand(CCard*);
    virtual void getRandomHand(const QList<CPlayer*> &knownList)=0;
    bool canBeTarget(CCard*,CPlayer*);
    QList<CCard*> cardList(bool hand=true,bool zhuangbei=true,bool panding=true);
    virtual void needShow()=0;
    bool hasWuXieKeJi();
    bool cardPlayed;
    int rvInt,playCount;
    bool stateJiu;
    CCard* cardShown;
    QList<CCard*> playedCards;
    QList<CPlayer*> playedTargets;

    quint8 position;
    CGeneral* general;
    quint8 kingdom;
    bool gender;
    CCard* panDingQu[3];
    quint8 role;
    CCard *zhuangBei[5];
    QList<CCard*> hands;
    QHash<int,QList<CCard*>> cardBlind,cardSeen;
    quint8 jiaYi,jianYi;
    quint8 shaDistance;
    bool isAlive;
    bool isTieSuoLianHuan,isFanMian;
    quint8 maxHP;
    char HP;
    QHash<int,int> marks;
    QHash<int,QVariant> skillVar;
    int roundMaxSha,roundUsedSha,currentPhase;
    bool roundJiuUsed;
    bool abandonPhase[18];
    QHash<int,QList<CPlayerSkill*>> phaseRegister;
    QList<CPlayerSkill*> skills;
    CGame *game;
    bool tuoGuan;
    bool offline;
    uint extreTime;
    CCard *judgeResult;
    quint8 drawCount;
    QList<QVariant> selectionList;
    virtual void setGeneral(CGeneral *);
    virtual void setKingdom(quint8);
    virtual void setRole(quint8);
    virtual void setMaxHP(quint8);
    virtual void setHP(char);
    virtual void setStateJiu(bool);
    virtual void setTieSuoLianHuan(bool);
    int distanceTo(CPlayer*);
    bool hasCard();
    bool containsCard(CCard*,int mode=CARDMODE_BOTH);
signals:
    void generalChosen();//信号：选好将了
protected:
    void phaseJudge();//判定阶段
    void phaseDraw();//摸牌阶段
    virtual void phasePlay()=0;//出牌阶段
    virtual void phaseDiscard();//弃牌阶段
    void nextPhase();
};

#endif // CPLAYER_H
