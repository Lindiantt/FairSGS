#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"
#include "card/ccard.h"
#include "game/cinjured.h"

CCardHuoGong::CCardHuoGong()
{
    canSelectTarget=true;
}

QList<CPlayer*> CCardHuoGong::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer* p;
    bool b;
    foreach (p, player->game->players) {
        if(p->isAlive)
        {
            b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            if(!p->hands.isEmpty())
                list.append(p);
        }
    }
    return list;
}

void CCardHuoGong::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
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
                f=std::bind(target->needShow,target);
                ev2->addFunc(f);
                CTempCard tcard(card);
                target->phaseCallback(PHASE_CARDTRANSFORM,&tcard);
                f=std::bind(player->needPlay,player,tcard.suit,0xffff,QList<quint8>(),1,nullptr,nullptr,
                                  PLAYMODE_DISCARD,CARDMODE_HANDS,QList<CPlayer*>());
                ev2->addFunc(f);
                f=std::bind([&](CPlayer *player,CCard *card,CPlayer *target){
                    CEvent *ev3=new CEvent(player->game);
                    if(player->cardPlayed)
                    {
                        CInjured *inj=new CInjured(1,card,player,ATTRIBUTE_HUO,false);
                        ev3->addFunc(std::bind(target->injured,target,inj));
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
