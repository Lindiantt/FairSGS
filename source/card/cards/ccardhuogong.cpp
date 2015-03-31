﻿#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

CCardHuoGong::CCardHuoGong()
{
    canSelectTarget=true;
}

QList<CPlayer*> CCardHuoGong::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer* p;
    bool b;
    foreach (p, player->game->players) {
        if(p->isAlive)
        {
            b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            if(!p->hands.isEmpty())
                list.append(p);
        }
    }
    return list;
}