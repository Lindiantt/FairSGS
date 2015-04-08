#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

CCardLeBuSiShu::CCardLeBuSiShu()
{
    canSelectTarget=true;
}

QList<CPlayer*> CCardLeBuSiShu::availableTargets(CPlayer *player, CCard *card)
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
            if(!p->panDingQu[1])
                list.append(p);
        }
    }
    return list;
}

void CCardLeBuSiShu::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    list[0]->panDingQuAdd(card);
    emit player->game->newData();
}
