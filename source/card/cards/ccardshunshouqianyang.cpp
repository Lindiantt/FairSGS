#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "game/cevent.h"

QList<CPlayer*> CCardShunShouQianYang::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> list;
    CPlayer* p;
    int jnd=1;
    player->phaseCallback(PHASE_JINNANGDISTANCE,card,&jnd);
    foreach (p, player->game->players) {
        if(p!=player&&p->isAlive)
        {
            if(jnd!=-1&&player->distanceTo(p)>jnd) continue;
            bool b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            if(p->hasCard())
                list.append(p);
        }
    }
    return list;
}

CCardShunShouQianYang::CCardShunShouQianYang()
{
    canSelectTarget=true;
}

void CCardShunShouQianYang::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
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
            auto f=std::bind([&](CPlayer *player, CCard *,CPlayer *target){
                CEvent* ev2=new CEvent(player->game);
                std::function<void()> f;
                f=std::bind(wuxiePlay,this,ev2);
                ev2->addFunc(f);
                QVector<QVariant> values;
                values.resize(target->cardList().size());
                values[0]=target->position;
                QList<QVariant> dv;
                dv.append((int)0);
                f=std::bind(player->needSelect,player,"",SELECTTYPE_PLAYERCARD,QList<QVariant>::fromVector(values),dv,1,1);
                ev2->addFunc(f);
                f=std::bind([&](CPlayer *player,CPlayer *target){
                    CEvent *ev=new CEvent(player->game);
                    int i=player->selectionList[0].toUInt();
                    target->cardShown=target->cardList()[i];
                    if(!target->cardShown)
                    {
                        QList<CPlayer*> kl;
                        kl<<target<<player;
                        auto f=std::bind(target->getRandomHand,target,kl);
                        ev->addFunc(f);
                    }
                    auto f=std::bind([&](CPlayer *player,CPlayer *target){
                        target->loseCard(target->cardShown);
                        QList<CCard*> cards;
                        cards.append(target->cardShown);
                        player->getCard(cards,target);
                        target->phaseCallback(PHASE_LOSECARD,&cards,(void*)LOSECARDREASON_STEAL);
                        emit player->game->newData();
                    },player,target);
                    ev->addFunc(f);
                    emit player->game->newData();
                },player,target);
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
