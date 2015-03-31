#include "card/ccardtype.h"
#include "game/cplayer.h"
#include "game/cgame.h"
#include "general/cskill.h"
#include "mainwindow.h"
extern MainWindow *w;

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
