#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

CCardTieSuoLianHuan::CCardTieSuoLianHuan()
{
    canSelectTarget=true;
    minTargets=0;
    maxTargets=2;
}

QList<CPlayer*> CCardTieSuoLianHuan::availableTargets(CPlayer *player, CCard *card)
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
            list.append(p);
        }
    }
    return list;
}

void CCardTieSuoLianHuan::useCard(CPlayer *, CCard *, QList<CPlayer *> &)
{

}
