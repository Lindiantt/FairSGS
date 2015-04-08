#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "card/ccard.h"

CCardBingLiangCunDuan::CCardBingLiangCunDuan()
{
    canSelectTarget=true;
}

QList<CPlayer*> CCardBingLiangCunDuan::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer* p;
    int jnd=1;
    player->phaseCallback(PHASE_JINNANGDISTANCE,card,&jnd);
    foreach (p, player->game->players) {
        if(p!=player&&p->isAlive)
        {
            if(jnd!=-1&&player->distanceTo(p)>jnd) continue;
            bool b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            if(!p->panDingQu[2])
                list.append(p);
        }
    }
    return list;
}

void CCardBingLiangCunDuan::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    list[0]->panDingQuAdd(card);
    emit player->game->newData();
}
