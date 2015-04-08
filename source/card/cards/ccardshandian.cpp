#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

QList<CPlayer*> CCardShanDian::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    if(player->panDingQu[0]) return list;
    bool b=true;
    player->phaseCallback(PHASE_CANBETARGET,card,player,&b);
    if(b)
        list.append(player);
    return list;
}

void CCardShanDian::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    list[0]->panDingQuAdd(card);
    emit player->game->newData();
}
