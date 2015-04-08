#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

QList<CPlayer*> CCardWanJianQiFa::availableTargets(CPlayer *player, CCard *card)
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

CCardWanJianQiFa::CCardWanJianQiFa()
{
    maxTargets=99;
}

void CCardWanJianQiFa::useCard(CPlayer *, CCard *, QList<CPlayer *> &)
{

}
