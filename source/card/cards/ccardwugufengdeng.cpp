﻿#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

QList<CPlayer*> CCardWuGuFengDeng::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer *p;
    foreach (p, player->game->players) {
        if(p->isAlive)
        {
            bool b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            list.append(p);
        }
    }
    return list;
}

CCardWuGuFengDeng::CCardWuGuFengDeng()
{
    maxTargets=99;
}

void CCardWuGuFengDeng::useCard(CPlayer *, CCard *, QList<CPlayer *> &)
{

}
