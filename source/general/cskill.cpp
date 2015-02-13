#include "cskill.h"

CSkill::CSkill()
{

}

CSkill::~CSkill()
{

}

bool CSkill::activeSkill(QVector<CGeneral*> &generals,QVector<CCard*> &cards)
{
    return true;
}

SSkillTip CSkill::activeTip(QVector<CGeneral*> &generals,QVector<CCard*> &cards)
{
    SSkillTip st;
    return st;
}

bool CSkill::cardTargetJudge(CGeneral* target,CGeneral* source,QVector<CGeneral*> &otherTargets,int cardType)
{
    return true;
}

void CSkill::getEvent(CGeneral* general)
{

}

void CSkill::loseEvent(CGeneral* general)
{

}

void CSkill::phaseCallback(CGeneral* skillOwner,CGeneral* general,int phase,int extre,int extre2)
{

}

int CSkill::msg(int type,void* par1,void* par2)
{
    return 0;
}
