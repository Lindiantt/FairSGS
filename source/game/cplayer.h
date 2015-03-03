#ifndef CPLAYER_H
#define CPLAYER_H

#include "pch.h"
#include "card/ccard.h"

class CGeneral;
class CCardZhuangbei;
class CGame;
class CSkill;
class CInjured;

#define SELECTTYPE_YESNO 0
#define SELECTTYPE_WUXIEKEJI 1
#define SELECTTYPE_CUSTOM 2

class CPlayer:public QObject
{
    Q_OBJECT
public:
    CPlayer();
    ~CPlayer();
    void phaseCallback(int phase, void* extre=nullptr, void* extre2=nullptr,void* extre3=nullptr,
                       void* extre4=nullptr);//技能回调函数
    virtual void chooseGeneral(QList<CGeneral*> &)=0;//选将
    virtual void deliverCard(QList<CCard*> &)=0;//初始派牌
    void roundInit();//执行回合
    void regPhase(int phase,CSkill* skill,CPlayer* player);//技能对特定的时机进行注册
    void unregPhase(int phase,CSkill* skill,CPlayer* player);//技能对特定的时机解除注册
    virtual CCard* needJudge();//需要进行判定
    void discardCard(QList<CCard*> &,int reason);//把牌放入弃牌堆
    virtual void judgeTip(const QString &);//判定提示（CPlayerCLient专用）
    void injured(CInjured*);//受伤
    void loseHP(int);//体力流失
    bool canBeTarget(CCard*);//是否能成为牌的目标
    void getCard(QList<CCard*> &list,CPlayer* player=nullptr);//获得牌
    virtual void goingToDie();//濒死状态
    virtual int needSelect(const QString question,int selectionType=SELECTTYPE_YESNO,
                           const QList<QString> &option=QList<QString>())=0;//需要玩家做选择
    virtual void loseCard(QList<CCard*> &,int reason);//失去牌
    virtual void networkSend()=0;//发送信息

    quint8 position;
    CGeneral* general;
    quint8 kingdom;
    bool gender;
    QList<CCard*> panDingQu,judgeCardArea;
    quint8 role;
    CCardZhuangbei *zhuangBei[5];
    QList<CCard*> hands;
    QHash<QPair<CSkill*,CPlayer*>,QList<CCard*>> cardBlind,cardSeen;
    quint8 jiaYi,jianYi;
    quint8 shaDistance;
    bool isAlive;
    bool isTieSuoLianHuan,isFanMian;
    quint8 HP,maxHP;
    QHash<int,int> marks;
    QHash<int,QVariant> skillVar;
    int roundMaxSha,roundUsedSha;
    bool notDrawCard;
    bool abandonPhase[18];
    QHash<int,QMultiHash<CSkill*,CPlayer*>> phaseRegister;
    QHash<CSkill*,bool> skills;
    CGame *game;
signals:
    void generalChosen();//信号：选好将了
protected:
    void phaseJudge();//判定阶段
    void phaseDraw();//摸牌阶段
    virtual void phasePlay();//出牌阶段
    void phaseDiscard();//弃牌阶段
};

#endif // CPLAYER_H
