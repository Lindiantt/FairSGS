#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"
#include "mainwindow.h"
extern MainWindow *w;
#include "game/cinjured.h"

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

void CCardSha::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    CEvent* ev=new CEvent(player->game);
    ev->addCard(card);
    int point=1;
    if(player->stateJiu)
    {
        point++;
        player->setStateJiu(false);
    }
    CCardSha *sha=static_cast<CCardSha*>(card->type);
    player->phaseCallback(PHASE_SHAINJURYPOINT,&point);
    player->phaseCallback(PHASE_TARGETCHANGE,card,&list);
    auto f=std::bind([&](CPlayer *player,CCard *card, QList<CPlayer *> list,int point){
        CPlayer *target;
        CEvent *ev2=new CEvent(player->game);
        foreach (target, list) {
            auto f2=std::bind(target->needPlay,target,CARDTYPE_SHAN,nullptr,1,player,card,PLAYMODE_USE,CARDMODE_HANDS);
            ev2->addFunc(f2);
            auto f=std::bind([&](CPlayer* player,CPlayer* target,CCard* card,int point){
                if(!target->cardPlayed)
                {
                    CInjured *inj=new CInjured(point,card,player,sha->attribute);
                    target->injured(inj);
                }
                emit player->game->newData();
            },player,target,card,point);
            ev2->addFunc(f);
        }
        emit player->game->newData();
    },player,card,list,point);
    ev->addFunc(f);
    emit player->game->newData();
}
