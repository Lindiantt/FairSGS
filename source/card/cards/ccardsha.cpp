#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"
#include "mainwindow.h"
extern MainWindow *w;
#include "game/cinjured.h"
#include "card/ccard.h"

bool CCardSha::canUse(CPlayer *player,CCard* card)
{
    if(player->roundMaxSha!=-1&&player->roundUsedSha>=player->roundMaxSha) return false;
    return CCardType::canUse(player,card);
}

QList<CPlayer*> CCardSha::availableTargets(CPlayer* player,CCard* card)
{
    QList<CPlayer*> list;
    CPlayer *p;
    int distance;
    foreach (p, player->game->players) {
        if(p!=player&&p->isAlive)
        {
            distance=player->game->countPositionDistance(p->position,player->position);
            distance-=player->jianYi+player->shaDistance;
            distance+=p->jiaYi;
            player->phaseCallback(PHASE_SHADISTANCE,p,&distance);
            if(distance<=0)
            {
                bool b=true;
                p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
                if(b)
                    list.append(p);
            }
        }
    }
    return list;
}

CCardSha::CCardSha()
{
    canSelectTarget=true;
    attribute=ATTRIBUTE_NONE;
}

CCardHuoSha::CCardHuoSha()
{
    attribute=ATTRIBUTE_HUO;
}

CCardLeiSha::CCardLeiSha()
{
    attribute=ATTRIBUTE_LEI;
}

void CCardSha::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    CEvent* ev=new CEvent(player->game);
    ev->addCard(card);
    player->roundUsedSha++;
    int point=1;
    if(player->stateJiu)
    {
        point++;
        player->setStateJiu(false);
    }
    player->phaseCallback(PHASE_SHAINJURYPOINT,&point);
    QList<CPlayer*> *targets=new QList<CPlayer*>();
	*targets=list;
	player->phaseCallback(PHASE_TARGETCHANGE,card,targets);
	auto f=std::bind([&](CPlayer *player, CCard *card,QList<CPlayer*> *targets,int point){
		CPlayer *target;
		CEvent *ev2=new CEvent(player->game);
		foreach (target, *targets) {
			auto f2=std::bind(target->needPlay,target,0xff,CARDTYPE_SHAN,QList<quint8>(),1,player,card,PLAYMODE_USE,
                              CARDMODE_HANDS,QList<CPlayer*>());
            ev2->addFunc(f2);
            auto f=std::bind([&](CPlayer* player,CPlayer* target,CCard* card,int point){
                CEvent *ev3=new CEvent(player->game);
                if(target->cardPlayed)
                {
                    bool b=false;
                    player->phaseCallback(PHASE_CARDPLAYAGAIN,&b,target,card);
                    if(b)
                    {
                        auto f=std::bind(target->needPlay,target,0xff,CARDTYPE_SHAN,QList<quint8>(),1,player,card,
                                PLAYMODE_USE,CARDMODE_HANDS,QList<CPlayer*>());
                        ev3->addFunc(f);
                    }
                }
                auto f=std::bind(useCardCallback,this,player,target,card,point);
                ev3->addFunc(f);
                emit player->game->newData();
            },player,target,card,point);
            ev2->addFunc(f);
		}
		delete targets;
		emit player->game->newData();
	},player,card,targets,point);
	ev->addFunc(f);
    emit player->game->newData();
}

void CCardSha::useCardCallback(CPlayer *player, CPlayer *target, CCard *card, int point)
{
    if(!target->cardPlayed)
    {
        CCardSha *sha=static_cast<CCardSha*>(card->type);
        CInjured *inj=new CInjured(point,card,player,sha->attribute);
        auto f=std::bind(target->injured,target,inj);
        player->game->smartInsertFunc(f);
    }
    emit player->game->newData();
}
