#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"

QList<CPlayer*> CCardWuZhongShengYou::availableTargets(CPlayer *player, CCard *)
{
    QList<CPlayer*> list;
    list.append(player);
    return list;
}

void CCardWuZhongShengYou::useCard(CPlayer *, CCard *, QList<CPlayer *> &)
{

}
