#include "cskill.h"
#include "game/cplayer.h"

CSkill::CSkill()
{
    priority=0;
    defaultOn=false;
    lock=false;
    active=false;
    onlyOnce=false;
    zhugong=false;
}

CSkill::~CSkill()
{

}

void CSkill::activeSkill(CPlayerSkill* )
{

}

void CSkill::getEvent(CPlayerSkill *)
{

}

void CSkill::loseEvent(CPlayerSkill *)
{

}

void CSkill::phaseCallback(CPlayerSkill* , int , CPlayer* , void* , void* , void *, void *)
{

}
