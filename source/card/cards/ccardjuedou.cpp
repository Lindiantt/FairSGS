#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"
#include "game/cinjured.h"

CCardJueDou::CCardJueDou()
{
    canSelectTarget=true;
}

QList<CPlayer*> CCardJueDou::availableTargets(CPlayer *player, CCard *card)
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
            list.append(p);
        }
    }
    return list;
}

void CCardJueDou::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
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
                CEvent* ev2=new CEvent(player->game,EVENT_JUEDOU);
                std::function<void()> f;
                f=std::bind(wuxiePlay,this,ev2);
                ev2->addFunc(f);
                f=std::bind(useCardCallback,this,player,card,target,true);
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

void CCardJueDou::useCardCallback(CPlayer *player, CCard *card, CPlayer *target, bool isTargetTurn)
{
    CEvent *ev=new CEvent(player->game);
    if(isTargetTurn)
    {
        auto f=std::bind(target->needPlay,target,0xff,CARDTYPE_SHA,QList<quint8>(),1,player,card,PLAYMODE_PLAY,
                    CARDMODE_HANDS,QList<CPlayer*>());
        ev->addFunc(f);
        auto f2=std::bind([&](CPlayer *player, CCard *card, CPlayer *target){
            CEvent *ev2=new CEvent(player->game);
            if(target->cardPlayed)
            {
                bool b;
                player->phaseCallback(PHASE_CARDPLAYAGAIN,&b,target,card);
                if(b)
                {
                    auto f=std::bind(target->needPlay,target,0xff,CARDTYPE_SHA,QList<quint8>(),1,player,card,
                        PLAYMODE_PLAY,CARDMODE_HANDS,QList<CPlayer*>());
                    ev2->addFunc(f);
                }
            }
            emit player->game->newData();
        },player,card,target);
        ev->addFunc(f2);
    }
    else
    {
        auto f=std::bind(player->needPlay,player,0xff,CARDTYPE_SHA,QList<quint8>(),1,target,card,PLAYMODE_PLAY,
                    CARDMODE_HANDS,QList<CPlayer*>());
        ev->addFunc(f);
        auto f2=std::bind([&](CPlayer *player, CCard *card, CPlayer *target){
            CEvent *ev2=new CEvent(player->game);
            if(player->cardPlayed)
            {
                bool b;
                target->phaseCallback(PHASE_CARDPLAYAGAIN,&b,player,card);
                if(b)
                {
                    auto f=std::bind(player->needPlay,player,0xff,CARDTYPE_SHA,QList<quint8>(),1,target,card,
                        PLAYMODE_PLAY,CARDMODE_HANDS,QList<CPlayer*>());
                    ev2->addFunc(f);
                }
            }
            emit player->game->newData();
        },player,card,target);
        ev->addFunc(f2);
    }
    auto f=std::bind([&](CPlayer *player, CCard *card, CPlayer *target, bool isTargetTurn){
        CEvent *ev=player->game->currentEvent->findParent(EVENT_JUEDOU);
        if(target->cardPlayed)
        {
            auto f=std::bind(useCardCallback,this,player,card,target,!isTargetTurn);
            ev->addFunc(f);
        }
        else
        {
            if(isTargetTurn)
            {
                CInjured *inj=new CInjured(1,card,player);
                auto f=std::bind(target->injured,target,inj);
                ev->addFunc(f);
            }
            else
            {
                CInjured *inj=new CInjured(1,card,target);
                auto f=std::bind(player->injured,player,inj);
                ev->addFunc(f);
            }
        }
        emit player->game->newData();
    },player,card,target,isTargetTurn);
    ev->addFunc(f);
    emit player->game->newData();
}
