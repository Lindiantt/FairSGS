#include "card/ccardtype.h"
#include "game/cplayer.h"

void CCardZhuangbei::callback(CPlayer *,int )
{

}

void CCardZhuangbei::moveEvent(CPlayer *,CPlayer *)
{

}

void CCardZhuangbei::takeOffEvent(CPlayer *)
{

}

void CCardZhuangbei::wearEvent(CPlayer *)
{

}

QList<CPlayer*> CCardZhuangbei::availableTargets(CPlayer *player, CCard *)
{
    QList<CPlayer*> list;
    list.append(player);
    return list;
}

void CCardZhuangbei::useCard(CPlayer *, CCard *, QList<CPlayer *> &)
{

}
