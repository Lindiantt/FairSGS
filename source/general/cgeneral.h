#ifndef CGENERAL_H
#define CGENERAL_H

#include <qstring.h>
#include "general/cskill.h"

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

class CGeneral
{
public:
    CGeneral();
    ~CGeneral();
    QString name;
    QString nick;
    bool gender;
    quint8 kingdom;
    quint8 maxHP,HP;
    quint8 package;
    QVector<CSkill*> skill;
    QVector<CGeneral*> derived;
};


#endif // CGENERAL_H
