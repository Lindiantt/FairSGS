#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "mainwindow.h"
extern MainWindow *w;
#include "card/ccard.h"
#include "game/cevent.h"

QList<CPlayer*> CCardJieDaoShaRen::availableTargets(CPlayer *player, CCard *card)
{
    QList<CPlayer*> l;
    CPlayer* p;
    foreach (p, player->game->players) {
        if(p!=player&&p->isAlive)
        {
            bool b=true;
            p->phaseCallback(PHASE_CANBETARGET,card,player,&b);
            if(!b) continue;
            if(!p->zhuangBei[0]) continue;
            CTempCard tcard(w->cardType[CARD_PUTONGSHA]);
            auto list=w->cardType[CARD_PUTONGSHA]->availableTargets(p,static_cast<CCard*>(&tcard));
            if(!list.isEmpty())
                l.append(p);
        }
    }
    return l;
}

CCardJieDaoShaRen::CCardJieDaoShaRen()
{
    canSelectTarget=true;
}

bool CCardJieDaoShaRen::cardUseCheck(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    int min=minTargets;
    int max=maxTargets;
    player->phaseCallback(PHASE_CARDMINMAXTARGETS,card,&min,&max);
    if(list.size()<min*2||list.size()>max*2||list.size()%2!=0) return false;
    auto alist=availableTargets(player,card);
    int group=list.size()/2;
    for(int i=0;i<group;i++)
    {
        if(!alist.contains(list[i*2])) return false;
        CTempCard tcard(w->cardType[CARD_PUTONGSHA]);
        auto tal=w->cardType[CARD_PUTONGSHA]->availableTargets(player,static_cast<CCard*>(card));
        if(!tal.contains(list[i*2+1])) return false;
    }
    return true;
}

void CCardJieDaoShaRen::useCard(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    CEvent *ev=new CEvent(player->game);
    ev->addCard(card);
    QList<CPlayer*> *targets=new QList<CPlayer*>();
    *targets=list;
    player->phaseCallback(PHASE_TARGETCHANGE,card,targets);
    auto f=std::bind([&](CPlayer *player, CCard *card,QList<CPlayer*> *targets){
        CEvent *ev=new CEvent(player->game);
        for(int i=0;i<(*targets).size();i+=2)
        {
            auto f=std::bind([&](CPlayer *player, CCard *card,CPlayer* target,CPlayer* target2){
                CEvent* ev2=new CEvent(player->game);
                std::function<void()> f;
                f=std::bind(wuxiePlay,this,ev2);
                ev2->addFunc(f);
                QList<CPlayer*> shaTarget;
                shaTarget.append(target2);
                f=std::bind(CPlayer::needPlay,target,0xff,CARDTYPE_SHA,QList<quint8>(),1,nullptr,nullptr,PLAYMODE_USE,
                            CARDMODE_HANDS,shaTarget);
                ev2->addFunc(f);
                f=std::bind([&](CPlayer *player, CPlayer* target){
                    if(!target->cardPlayed)
                    {
                        CEvent *ev2=new CEvent(player->game);
                        CCard *zb=target->zhuangBei[0];
                        target->setZhuangBei(0,nullptr);
                        QList<CCard*> zbs;
                        zbs.append(zb);
                        target->phaseCallback(PHASE_LOSECARD,&zbs,(void*)LOSECARDREASON_STEAL);
                        auto f=std::bind(player->getCard,player,zbs,target);
                        ev2->addFunc(f);
                    }
                    emit player->game->newData();
                },player,target);
                ev2->addFunc(f);
                emit player->game->newData();
            },player,card,(*targets)[i],(*targets)[i+1]);
            ev->addFunc(f);
        }
        delete targets;
        emit player->game->newData();
    },player,card,targets);
    ev->addFunc(f);
    emit player->game->newData();
}
