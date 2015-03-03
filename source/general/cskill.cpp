#include "cskill.h"

CSkill::CSkill()
{
    priority=0;
    defaultOn=false;
    lock=false;
    active=false;
}

CSkill::~CSkill()
{

}

void CSkill::activeSkill(CPlayer* palyer)
{

}

void CSkill::getEvent(CPlayer *player)
{

}

void CSkill::loseEvent(CPlayer *player)
{

}

void CSkill::phaseCallback(CPlayer* skillOwner, int phase, CPlayer* who, void* extre, void* extre2, void *extre3, void *extre4)
{

}
