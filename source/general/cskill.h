#ifndef CSKILL_H
#define CSKILL_H

#include "pch.h"

class CGeneral;
class CPlayer;
class CCard;

#define PHASE_BEFOREBEGIN 0
#define PHASE_BEGINPHASE 1
#define PHASE_AFTERBEGIN 2
#define PHASE_BEFOREJUDGE 3
#define PHASE_JUDGEPHASE 4
#define PHASE_AFTERJUDGE 5
#define PHASE_BEFOREDRAW 6
#define PHASE_DRAWPHASE 7
#define PHASE_AFTERDRAW 8
#define PHASE_BEFOREPLAY 9
#define PHASE_PLAYPHASE 10
#define PHASE_AFTERPLAY 11
#define PHASE_BEFOREDISCARD 12
#define PHASE_DISCARDPHASE 13
#define PHASE_AFTERDISCARD 14
#define PHASE_BEFOREEND 15
#define PHASE_ENDPHASE 16
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
#define PHASE_AFTERDIED 29
#define PHASE_PLAYCARD 30
#define PHASE_CALCDISTANCE 31
#define PHASE_COUNTMAXSHA 32
#define PHASE_LOSEZHUANGBEI 33
#define PHASE_DISCARD 37
#define PHASE_GETCARD 35
#define PHASE_ABOUTTODISCARD 36
#define PHASE_DISCARDOVER 38
#define PHASE_CANBETARGET 34
#define PHASE_JUDGED 39
#define PHASE_CARDTRANSFORM 40
#define PHASE_BEFOREINJURED 41
#define PHASE_SUITTRANSFORM 42
#define PHASE_SHADISTANCE 43
#define PHASE_DISTANCE 44
#define PHASE_CANUSECARD 45
#define PHASE_CANPLAYCARD 46
#define PHASE_JINNANGDISTANCE 47
#define PHASE_CARDMINMAXTARGETS 48
#define PHASE_TARGETCHANGE 49
#define PHASE_SHAINJURYPOINT 50
#define PHASE_CARDPLAYAGAIN 51
#define PHASE_HASWUXIEKEJI 52
#define PHASE_GAMESTART 100

#define DISCARDREASON_DISCARD 0
#define DISCARDREASON_USE 1
#define DISCARDREASON_PLAY 2
#define DISCARDREASON_JUDGE 3
#define DISCARDREASON_TRANSFORM 4
#define DISCARDREASON_YANSHIJINNANG 5

#define LOSECARDREASON_USE 1
#define LOSECARDREASON_PLAY 0
#define LOSECARDREASON_STEAL 3
#define LOSECARDREASON_DISCARD 2

#define MARKTYPE_KUANGFENG 0

#include "general/cplayerskill.h"

class CSkill:public QObject
{
    Q_OBJECT
public:
    CSkill();
    ~CSkill();
    virtual void activeSkill(CPlayerSkill*);
    virtual void getEvent(CPlayerSkill*);
    virtual void loseEvent(CPlayerSkill*);
    virtual void phaseCallback(CPlayerSkill* skillOwner,int phase,CPlayer* player=nullptr, void* extre=nullptr,
                               void* extre2=nullptr, void* extre3=nullptr, void* extre4=nullptr);
    QString name;
    QString description;
    bool lock;
    bool onlyOnce;
    bool active;
    bool clickable,defaultOn;
    bool zhugong;
    int priority;
};

#endif // CSKILL_H
