#include "cplayer.h"
#include "network/cplayersocket.h"
#include "general/cskill.h"
#include "cevent.h"
#include "cgame.h"
#include "cinjured.h"

#define CHECKEVENT(a) if(!(a).available){return;}

CPlayer::CPlayer()
{
    for(int i=0;i<5;i++)
    {
        zhuangBei[i]=nullptr;
    }
}

CPlayer::~CPlayer()
{

}

void CPlayer::roundInit()
{
    roundMaxSha=1;
    roundUsedSha=0;
    notDrawCard=false;
    for(int i=0;i<18;i++)
    {
        abandonPhase[i]=false;
    }
    CEvent roundEvent(game);
    roundEvent.funcDelete=[=](){emit game->signalNextRound();};
    for(int i=PHASE_BEFOREBEGIN;i<=PHASE_BEFOREJUDGE;i++)
    {
        if(!abandonPhase[i])
        {
            phaseCallback(i);
            CHECKEVENT(roundEvent);
        }
    }
    if(!abandonPhase[PHASE_JUDGEPHASE])
    {
        phaseJudge();
        CHECKEVENT(roundEvent);
    }
    for(int i=PHASE_AFTERJUDGE;i<=PHASE_BEFOREDRAW;i++)
    {
        if(!abandonPhase[i])
        {
            phaseCallback(i);
            CHECKEVENT(roundEvent);
        }
    }
    if(!abandonPhase[PHASE_DRAWPHASE])
    {
        phaseDraw();
        CHECKEVENT(roundEvent);
    }
    for(int i=PHASE_AFTERDRAW;i<=PHASE_BEFOREPLAY;i++)
    {
        if(!abandonPhase[i])
        {
            phaseCallback(i);
            CHECKEVENT(roundEvent);
        }
    }
    phaseCallback(PHASE_PLAYPHASE);
    CHECKEVENT(roundEvent);
    phaseCallback(PHASE_COUNTMAXSHA);
    CHECKEVENT(roundEvent);
    phasePlay();
}

void CPlayer::phasePlay()
{

}

void CPlayer::phaseDraw()
{
    CEvent drawEvent(game);
    int drawCount=2;
    phaseCallback(PHASE_DRAWPHASE,&drawCount);
    CHECKEVENT(drawEvent);
    if(!notDrawCard)
    {
        auto cards=game->drawCard(drawCount);
        this->getCard(cards);
    }
}

void CPlayer::phaseJudge()
{
    CEvent judgePhaseEvent(game);
    CCard* judgeCard;
    auto it=panDingQu.begin();
    while(it!=panDingQu.end())
    {
        CEvent judgeEvent(game);
        CCard* card=*it;
        //结束时如果判定牌和延时锦囊没有被人收掉，就放进弃牌堆
        judgeEvent.funcDelete=[&](){
            QList<CCard*> clist;
            if(panDingQu.contains(card))
            {
                clist.clear();
                clist.append(card);
                panDingQu.erase(it);
                discardCard(clist,DISCARDREASON_YANSHIJINNANG);
            }
            if(judgeCardArea.contains(judgeCard))
            {
                clist.clear();
                clist.append(judgeCard);
                judgeCardArea.removeOne(judgeCard);
                discardCard(clist,DISCARDREASON_JUDGE);
            }
            it=panDingQu.begin();
        };
        judgeTip(card->type->name);
        judgeCard=needJudge();
        CHECKEVENT(judgeEvent);
        phaseCallback(PHASE_CARDTRANSFORM,&judgeCard);
        CHECKEVENT(judgeEvent);
        switch (card->type->type2) {
        case CARD_LEBUSISHU:
            if(judgeCard->suit!=SUIT_HONGTAO)
            {
                for(int i=PHASE_BEFOREPLAY;i<=PHASE_AFTERPLAY;i++)
                    abandonPhase[i]=true;
            }
            break;
        case CARD_BINGLIANGCUNDUAN:
            if(judgeCard->suit!=SUIT_MEIHUA)
            {
                for(int i=PHASE_BEFOREDRAW;i<=PHASE_AFTERDRAW;i++)
                    abandonPhase[i]=true;
            }
            break;
        case CARD_SHANDIAN:
            if(judgeCard->suit==SUIT_HEITAO&&card->number>=2&&card->number<=9)
            {
                CInjured *inj=new CInjured(3,card,nullptr,ATTRIBUTE_LEI,false);
                injured(inj);
            }
            else
            {
                CPlayer *nap=this;
                do
                {
                    nap=game->nextAlivePlayer(nap->position);
                }while(!nap->canBeTarget(card)&&nap!=this);//寻找下一个能成为闪电目标的人
                panDingQu.removeOne(card);
                if(nap==this)
                {
                    judgePhaseEvent.funcDelete=[=](){
                        panDingQu.append(card);
                    };
                }
                else
                {
                    nap->panDingQu.append(card);
                }
            }
        default:
            break;
        }
    }
}

void CPlayer::judgeTip(const QString &tip)
{

}

bool CPlayer::canBeTarget(CCard *)
{
    return true;
}

void CPlayer::injured(CInjured *inj)
{
    CEvent injuryEvent(game);
    injuryEvent.funcDelete=[=](){inj->deleteLater();};
    phaseCallback(PHASE_BEFOREINJURED,inj);
    CHECKEVENT(injuryEvent);
    this->HP-=inj->point;
    if(this->HP<=0)
    {
        goingToDie();
        CHECKEVENT(injuryEvent);
    }
    phaseCallback(PHASE_INJURED,inj);
    CHECKEVENT(injuryEvent);
    if(this->isTieSuoLianHuan&&inj->attribute!=ATTRIBUTE_NONE&&inj->fromTieSuo==false)
    {
        this->isTieSuoLianHuan=false;
        foreach (CPlayer* player, game->players) {
            if(player!=this&&player->isTieSuoLianHuan)
            {
                CInjured *tsinj=new CInjured(inj);
                player->injured(tsinj);
            }
            CHECKEVENT(injuryEvent);
        }
    }
}

void CPlayer::loseHP(int point)
{
    CEvent losehpEvent(game);
    this->HP-=point;
    if(HP<=0)
    {
        goingToDie();
        CHECKEVENT(losehpEvent);
    }
    phaseCallback(PHASE_LOSEHP,(void*)point);
}

void CPlayer::getCard(QList<CCard *> &list, CPlayer *player)
{
    this->hands.append(list);
    phaseCallback(PHASE_GETCARD,player);
}

void CPlayer::discardCard(QList<CCard*> &list,int reason)
{
    CEvent ev(game);
    phaseCallback(PHASE_ABOUTTODISCARD,&list,(void*)reason);
    CHECKEVENT(ev);
    phaseCallback(PHASE_DISCARD,&list,(void*)reason);
    CHECKEVENT(ev);
    foreach (CCard* card, list) {
        if(card->temp)
        {
            CTempCard* tcard=(CTempCard*)card;
            game->deadwood.append(tcard->originCards);
            phaseCallback(PHASE_DISCARDOVER,&(tcard->originCards),(void*)DISCARDREASON_TRANSFORM);
        }
        else
        {
            game->deadwood.append(card);
            phaseCallback(PHASE_DISCARDOVER,card,(void*)reason);
        }
    }
}

CCard* CPlayer::needJudge()
{
    CEvent pdevent(game);
    CCard* card;
    judgeCardArea.append(card);
    phaseCallback(PHASE_ABOUTTOJUDGE,&judgeCardArea[0]);
    if(!pdevent.available)
    {
        return judgeCardArea[0];
    }
    judgeCardArea=game->drawCard(1);
    phaseCallback(PHASE_JUDGING,&judgeCardArea[0]);
    phaseCallback(PHASE_JUDGED,&judgeCardArea[0]);
    return card;
}

void CPlayer::regPhase(int phase, CSkill *skill, CPlayer *player)
{
    auto it=phaseRegister.find(phase);
    if(it==phaseRegister.end())
    {
        it=phaseRegister.insert(phase,QMultiHash<CSkill*,CPlayer*>());
    }
    QMultiHash<CSkill*,CPlayer*> &hash=it.value();
    hash.insert(skill,player);
}

void CPlayer::unregPhase(int phase, CSkill *skill, CPlayer *player)
{
    auto it=phaseRegister.find(phase);
    if(it!=phaseRegister.end())
    {
        QMultiHash<CSkill*,CPlayer*> &hash=it.value();
        auto it2=hash.find(skill);
        while(it2!=hash.end()&&it2.key()==skill)
        {
            if(it2.value()==player)
            {
                hash.erase(it2);
                return;
            }
            it2++;
        }
    }
}

void CPlayer::phaseCallback(int phase, void *extre, void *extre2, void *extre3, void *extre4)
{
    if(phase<=PHASE_AFTEREND) game->currentPhase=phase;
    auto it=phaseRegister.find(phase);
    if(it!=phaseRegister.end())
    {
        QMultiHash<CSkill*,CPlayer*> &hash=it.value();
        QList<QPair<CSkill*,CPlayer*>> llist;
        auto ith=hash.begin();
        while(ith!=hash.end())
        {
            llist.append(QPair<CSkill*,CPlayer*>(ith.key(),ith.value()));
            ith++;
        }
        if(llist.size()>=2)
        {
            //按技能优先级排序，如果相同，则按位置排序。
            std::sort(llist.begin(),llist.end(),[=](QPair<CSkill*,CPlayer*> &a,QPair<CSkill*,CPlayer*> &b){
                if(a.first->priority<b.first->priority)
                    return false;
                else if(a.first->priority>b.first->priority)
                    return true;
                else
                {
                    if(a.second->position<this->position&&b.second->position>=this->position)
                        return false;
                    else if(a.second->position>=this->position&&b.second->position<this->position)
                        return true;
                    else if(a.second->position<b.second->position)
                        return true;
                    else
                        return false;
                }
            });
        }
        CEvent ev(game);
        foreach (auto pair, llist) {
            CSkill* skill=pair.first;
            CPlayer* player=pair.second;
            skill->phaseCallback(player,phase,this,extre,extre2,extre3,extre4);
            CHECKEVENT(ev);
        }
    }
}

void CPlayer::loseCard(QList<CCard *> &, int reason)
{

}

void CPlayer::goingToDie()
{

}
