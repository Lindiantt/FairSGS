#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

QList<CPlayer*> CCardJiu::availableTargets(CPlayer *player, CCard *)
{
    QList<CPlayer*> list;
    if(!player->roundJiuUsed)
        list.append(player);
    return list;
}
