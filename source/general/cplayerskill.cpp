#include "cplayerskill.h"
#include "cskill.h"

CPlayerSkill::CPlayerSkill(CSkill *skill,CPlayer *player) : QObject(nullptr)
{
    this->skill=skill;
    this->player=player;
    onlyOnceUsed=false;
}

CPlayerSkill::~CPlayerSkill()
{

}

void CPlayerSkill::activeSkill()
{
    skill->activeSkill(this);
}

void CPlayerSkill::getEvent()
{
    skill->getEvent(this);
}

void CPlayerSkill::loseEvent()
{
    skill->loseEvent(this);
}

void CPlayerSkill::phaseCallback(int phase, CPlayer *player, void *extre, void *extre2, void *extre3, void *extre4)
{
    skill->phaseCallback(this,phase,player,extre,extre2,extre3,extre4);
}
