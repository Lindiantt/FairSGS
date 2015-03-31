#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

QList<CPlayer*> CCardGuoHeCaiQiao::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer* p;
    bool b;
    foreach (p, player->game->players) {
        if(p!=player&&p->isAlive)
        {
            b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            if(p->hasCard())
                list.append(p);
        }
    }
    return list;
}

CCardGuoHeCaiQiao::CCardGuoHeCaiQiao()
{
    canSelectTarget=true;
}
