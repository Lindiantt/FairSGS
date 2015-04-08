#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"

QList<CPlayer*> CCardJiu::availableTargets(CPlayer *player, CCard *)
{
    QList<CPlayer*> list;
    if(!player->roundJiuUsed)
        list.append(player);
    return list;
}

void CCardJiu::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    CEvent *ev=new CEvent(player->game);
    ev->addCard(card);
    QList<CPlayer*> *targets=new QList<CPlayer*>();
	*targets=list;
	player->phaseCallback(PHASE_TARGETCHANGE,card,targets);
	auto f=std::bind([&](CPlayer *player,QList<CPlayer*> *targets){
        CPlayer *target;
        foreach (target, *targets) {
            target->setStateJiu(true);
        }
        delete targets;
        emit player->game->newData();
	},player,targets);
	ev->addFunc(f);
    emit player->game->newData();
}
