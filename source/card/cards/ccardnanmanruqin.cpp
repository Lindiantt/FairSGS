#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

QList<CPlayer*> CCardNanManRuQin::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer *p;
    foreach (p, player->game->players) {
        if(p!=player&&p->isAlive)
        {
            bool b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            list.append(p);
        }
    }
    return list;
}

CCardNanManRuQin::CCardNanManRuQin()
{
    maxTargets=99;
}
