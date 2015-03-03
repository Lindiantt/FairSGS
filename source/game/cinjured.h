#ifndef CINJURED_H
#define CINJURED_H

#include "pch.h"

class CCard;
class CPlayer;
#define ATTRIBUTE_NONE 0
#define ATTRIBUTE_LEI 1
#define ATTRIBUTE_HUO 2

class CInjured : public QObject
{
    Q_OBJECT
public:
    CInjured(CInjured*);
    CInjured(int point,CCard* sourceCard=nullptr,CPlayer* sourcePlayer=nullptr,quint8 attribute=ATTRIBUTE_NONE,bool fromTieSuo=false);
    ~CInjured();
    int point;
    quint8 attribute;
    bool fromTieSuo;
    CCard* sourceCard;
    CPlayer* sourcePlayer;
signals:

public slots:
};

#endif // CINJURED_H
