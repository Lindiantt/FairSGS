#ifndef CSKILL_H
#define CSKILL_H

#include <qstring.h>
#include <qvector.h>

class CGeneral;
class CCard;

#define TIPTYPE_CARD 0
#define TIPTYPE_GENERAL 1

#define PHASE_BEFOREBEGIN 0
#define PHASE_BEGIN 1
#define PHASE_AFTERBEGIN 2
#define PHASE_BEFOREJUDGE 3
#define PHASE_JUDGE 4
#define PHASE_AFTERJUDGE 5
#define PHASE_BEFOREDRAW 6
#define PHASE_DRAW 7
#define PHASE_AFTERDRAW 8
#define PHASE_BEFOREPLAY 9
#define PHASE_PLAY 10
#define PHASE_AFTERPLAY 11
#define PHASE_BEFOREDISCARD 12
#define PHASE_DISCARD 13
#define PHASE_AFTERDISCARD 14
#define PHASE_BEFOREEND 15
#define PHASE_END 16
#define PHASE_AFTEREND 17
#define PHASE_INJURED 18
#define PHASE_LOSEHP 19
#define PHASE_TARGET 20
#define PHASE_GOINGTODIE 21
#define PHASE_DIED 22
#define PHASE_LOSECARD 23
#define PHASE_TURNOVER 24
#define PHASE_JUDGING 25
#define PHASE_ABOUTTOJUDGE 26
#define PHASE_NEEDCARD 27
#define PHASE_BECOMETARGET 28


struct SSkillTip
{
    int tipType;
    union {
        QVector<CCard*> *card;
        QVector<CGeneral*> *general;
    };
};

class CSkill
{
public:
    CSkill();
    ~CSkill();
    virtual bool activeSkill(QVector<CGeneral*> &generals,QVector<CCard*> &cards);
    virtual SSkillTip activeTip(QVector<CGeneral*> &generals,QVector<CCard*> &cards);
    virtual bool cardTargetJudge(CGeneral* target,CGeneral* source,QVector<CGeneral*> &otherTargets,int cardType);
    virtual void getEvent(CGeneral* general);
    virtual void loseEvent(CGeneral* general);
    virtual void phaseCallback(CGeneral* skillOwner,CGeneral* general,int phase,int extre=0,int extre2=0);
    virtual int msg(int type,void* par1,void* par2);
    QString name;
    QString description;
    bool active;
};

#endif // CSKILL_H
