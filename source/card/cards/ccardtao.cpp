#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"


QList<CPlayer*> CCardTao::availableTargets(CPlayer* player,CCard* )
{
    QList<CPlayer*> list;
    if(player->maxHP>0&&player->HP<player->maxHP)
    {
        list.append(player);
    }
    return list;
}
