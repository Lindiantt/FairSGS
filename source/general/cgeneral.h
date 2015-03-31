#ifndef CGENERAL_H
#define CGENERAL_H

#include "pch.h"

class CSkill;

#define PACKAGE_BIAO 0
#define PACKAGE_FENG 1
#define PACKAGE_HUO 2
#define PACKAGE_LING 3
#define PACKAGE_SHAN 4
#define PACKAGE_YIJIANG 5
#define PACKAGE_YIJIANG2012 6
#define PACKAGE_YIJIANG2013 7
#define PACKAGE_SP 8
#define PACKAGE_XINGSP 9
#define PACKAGE_OL 10
#define PACKAGE_JIEXIANTUPO 11

#define KINGDOM_WEI 0
#define KINGDOM_SHU 1
#define KINGDOM_WU 2
#define KINGDOM_QUN 3
#define KINGDOM_SHEN 4

class CGeneral
{
public:
    CGeneral();
    ~CGeneral();
    static CGeneral** createAll();
    static CGeneral** createShen();
    static CGeneral* find(quint16);
    static QString kingdomName(quint8);
    static QString packageName(quint8);
    static QString roleName(quint8);
    quint16 id;
    QString name,displayName;
    QString nick;
    bool gender,zhuGong;
    quint8 kingdom;
    quint8 maxHP;
    quint8 package;
    QList<CSkill*> skill;
    QList<CGeneral*> derived;//可派生的武将
    int derivedFrom;//是哪个武将的派生将
    //派生将：SP孙尚香是孙尚香的派生将，诸如此类
};


#endif // CGENERAL_H
