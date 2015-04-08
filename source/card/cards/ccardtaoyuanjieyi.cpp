#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"

QList<CPlayer*> CCardTaoYuanJieYi::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer *p;
    foreach (p, player->game->players) {
        if(p->isAlive)
        {
            bool b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            list.append(p);
        }
    }
    return list;
}

CCardTaoYuanJieYi::CCardTaoYuanJieYi()
{
    maxTargets=99;
}

void CCardTaoYuanJieYi::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    CEvent *ev=new CEvent(player->game);
    ev->addCard(card);
    QList<CPlayer*> *targets=new QList<CPlayer*>();
	*targets=list;
	player->phaseCallback(PHASE_TARGETCHANGE,card,targets);
	auto f=std::bind([&](CPlayer *player, CCard *card,QList<CPlayer*> *targets){
        CEvent *ev=new CEvent(player->game);
        CPlayer *target;
        foreach (target, *targets) {
            auto f=std::bind([&](){
                CEvent* ev2=new CEvent(player->game);
                std::function<void()> f;
                f=std::bind(wuxiePlay,this,ev2);
                ev2->addFunc(f);
            });
            ev->addFunc(f);
        }
        delete targets;
        emit player->game->newData();
	},player,card,targets);
	ev->addFunc(f);
    emit player->game->newData();
}
