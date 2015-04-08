#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"
#include "game/cinjured.h"

QList<CPlayer*> CCardNanManRuQin::availableTargets(CPlayer *player, CCard *card)
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

CCardNanManRuQin::CCardNanManRuQin()
{
    maxTargets=99;
}

void CCardNanManRuQin::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    CEvent *ev=new CEvent(player->game);
    ev->addCard(card);
    QList<CPlayer*> *targets=new QList<CPlayer*>();
    *targets=list;
    player->phaseCallback(PHASE_TARGETCHANGE,card,targets);
    auto f=std::bind([&](CPlayer *player, CCard *card,QList<CPlayer*> *targets){
        CPlayer *target;
        CEvent *ev=new CEvent(player->game);
        foreach (target, *targets) {
            auto f=std::bind([&](CPlayer *player, CCard *card,CPlayer *target){
                CEvent* ev2=new CEvent(player->game);
                std::function<void()> f;
                f=std::bind(wuxiePlay,this,ev2);
                ev2->addFunc(f);
                f=std::bind(target->needPlay,target,0xff,CARDTYPE_SHA,QList<quint8>(),1,player,card,PLAYMODE_PLAY,
                    CARDMODE_HANDS,QList<CPlayer*>());
                ev2->addFunc(f);
                f=std::bind([&](CPlayer *player, CCard *card,CPlayer *target){
                    if(!target->cardPlayed)
                    {
                        CInjured *inj=new CInjured(1,card,player);
                        auto f=std::bind(target->injured,target,inj);
                        player->game->smartInsertFunc(f);
                    }
                    emit player->game->newData();
                },player,card,target);
                ev2->addFunc(f);
                emit player->game->newData();
            },player,card,target);
            ev->addFunc(f);
        }
        delete targets;
        emit player->game->newData();
    },player,card,targets);
    ev->addFunc(f);
    emit player->game->newData();
}
