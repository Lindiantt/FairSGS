#include "cinjured.h"

CInjured::CInjured(int point,CCard* sourceCard,CPlayer* sourcePlayer,quint8 attribute,bool fromTieSuo)
{
    this->point=point;
    this->sourceCard=sourceCard;
    this->sourcePlayer=sourcePlayer;
    this->attribute=attribute;
    this->fromTieSuo=fromTieSuo;
}

CInjured::CInjured(CInjured *inj)
{
    this->point=inj->point;
    this->sourceCard=inj->sourceCard;
    this->sourcePlayer=inj->sourcePlayer;
    this->attribute=inj->attribute;
    this->fromTieSuo=true;
}

CInjured::~CInjured()
{

}

