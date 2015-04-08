#include "cplayer.h"
#include "network/cplayersocket.h"
#include "general/cskill.h"
#include "cevent.h"
#include "cgame.h"
#include "cinjured.h"
#include "general/cgeneral.h"
#include "card/ccard.h"

CPlayer::CPlayer()
{
    offline=false;
    tuoGuan=false;
    isAlive=true;
    isTieSuoLianHuan=false;
    isFanMian=false;
    for(int i=0;i<5;i++)
    {
        zhuangBei[i]=nullptr;
    }
    general=nullptr;
    shaDistance=1;
    jianYi=0;
    jiaYi=0;
    for(int i=0;i<3;i++)
        panDingQu[i]=nullptr;
    stateJiu=false;
    playCount=0;
}

CPlayer::~CPlayer()
{

}

void CPlayer::nextPhase()
{
    do
    {
        currentPhase++;
    }while(currentPhase<=PHASE_AFTEREND&&abandonPhase[currentPhase]);
    if(currentPhase>PHASE_AFTEREND)
    {
        //game->roundEnd();
        return;
    }
    new CEvent(game,EVENT_PHASE);
    phaseCallback(currentPhase);
    switch (currentPhase) {
    case PHASE_JUDGEPHASE:
        phaseJudge();
        break;
    case PHASE_DRAWPHASE:
        phaseDraw();
        break;
    case PHASE_PLAYPHASE:
        phasePlay();
        break;
    case PHASE_DISCARDPHASE:
        phaseDiscard();
        break;
    default:
        break;
    }
    game->addFunc(std::bind(nextPhase,this));
    emit game->newData();
}

void CPlayer::roundInit()
{
    roundMaxSha=1;
    roundUsedSha=0;
    roundJiuUsed=false;
    currentPhase=-1;
    for(int i=0;i<18;i++)
    {
        abandonPhase[i]=false;
    }
    nextPhase();
}

void CPlayer::phaseDraw()
{
    drawCount=2;
    game->smartAddFunc([&](){
        if(drawCount)
        {
            QList<CPlayer*> ntk;
            ntk.append(this);
            auto f=std::bind(game->needCard,game,drawCount,ntk);
            game->insertMultiFunc(f);
            game->insertMultiFunc([&](){
                getCard(game->getDrawnCards(),nullptr);
            });
            game->smartInsertOver();
        }
        emit game->newData();
    });
}

void CPlayer::panDingQuAdd(CCard *card)
{
    panDingQu[card->type->type2-15]=card;
}

void CPlayer::panDingQuRemove(CCard *card)
{
    panDingQu[card->type->type2-15]=nullptr;
}

void CPlayer::phaseJudge()
{
    for(int i=0;i<3;i++)
    {
        CCard* card=panDingQu[i];
        if(!card) continue;
        auto f=std::bind([&](CCard* card){
            CEvent *je=new CEvent(game);
            emit game->judgeTip(card->type->type2);
            je->addFunc(std::bind(judge,this));
            auto f=std::bind([&](CCard* card){
                CTempCard tcard(judgeResult);
                phaseCallback(PHASE_CARDTRANSFORM,&tcard);
                switch (card->type->type2) {
                case CARD_LEBUSISHU:
                    if(tcard.suit!=SUIT_HONGTAO)
                    {
                        emit game->judgeSucceeded(judgeResult,true);
                        for(int i=PHASE_BEFOREPLAY;i<=PHASE_AFTERPLAY;i++)
                            abandonPhase[i]=true;
                    }
                    else
                        emit game->judgeSucceeded(judgeResult,false);
                    panDingQuRemove(card);
                    je->addCard(card);
                    break;
                case CARD_BINGLIANGCUNDUAN:
                    if(tcard.suit!=SUIT_MEIHUA)
                    {
                        emit game->judgeSucceeded(judgeResult,true);
                        for(int i=PHASE_BEFOREDRAW;i<=PHASE_AFTERDRAW;i++)
                            abandonPhase[i]=true;
                    }
                    else
                        emit game->judgeSucceeded(judgeResult,false);
                    panDingQuRemove(card);
                    je->addCard(card);
                    break;
                case CARD_SHANDIAN:
                    if(tcard.suit==SUIT_HEITAO&&tcard.number>=2&&tcard.number<=9)
                    {
                        emit game->judgeSucceeded(judgeResult,true);
                        panDingQuRemove(card);
                        je->addCard(card);
                        CInjured *inj=new CInjured(3,card,nullptr,ATTRIBUTE_LEI,false);
                        injured(inj);
                    }
                    else
                    {
                        emit game->judgeSucceeded(judgeResult,false);
                        CPlayer *nap=this;
                        do
                        {
                            nap=game->nextAlivePlayer(nap->position);
                        }while(!nap->canBeTarget(card,this)&&nap!=this);//寻找下一个能成为闪电目标的人
                        if(nap!=this)
                        {
                            panDingQuRemove(card);
                            nap->panDingQuAdd(card);
                        }
                    }
                default:
                    break;
                }
                emit game->newData();
            },card);
            je->addFunc(f);
            emit game->newData();
        },card);
        game->smartAddFunc(f);
    }
}

void CPlayer::judge()
{
    CEvent *je=new CEvent(game);
    phaseCallback(PHASE_ABOUTTOJUDGE);
    je->addFunc(std::bind([&](CEvent *je){
        auto f=std::bind(game->needCard,game,1,QList<CPlayer*>::fromVector(game->players));
        game->insertMultiFunc(f);
        game->insertMultiFunc([&](){
            auto list=game->getDrawnCards();
            judgeResult=list[0];
            je->addCard(judgeResult);
            emit game->newData();
        });
        game->smartInsertOver();
        emit game->newData();
    },je));
    phaseCallback(PHASE_JUDGING);
    phaseCallback(PHASE_JUDGED);
    emit game->newData();
}

void CPlayer::phaseDiscard()
{

}

void CPlayer::injured(CInjured *inj)
{
    CEvent *ev=new CEvent(game);
    phaseCallback(PHASE_BEFOREINJURED,inj);
    auto f=std::bind([&](CInjured *inj){
        char hp=this->HP-inj->point;
        setHP(hp);
        if(hp<=0)
        {
            goingToDie();
        }
        emit game->newData();
    },inj);
    ev->addFunc(f);
    phaseCallback(PHASE_INJURED,inj);
    auto f2=std::bind([&](CInjured *inj){
        if(this->isTieSuoLianHuan&&inj->attribute!=ATTRIBUTE_NONE&&inj->fromTieSuo==false)
        {
            setTieSuoLianHuan(false);
            CEvent *tsev=new CEvent(game);
            foreach (CPlayer* player, game->players) {
                if(player!=this&&player->isTieSuoLianHuan)
                {
                    CInjured *tsinj=new CInjured(inj);
                    auto f=std::bind(player->injured,player,tsinj);
                    tsev->addFunc(f);
                }
            }
        }
        inj->deleteLater();
        emit game->newData();
    },inj);
    ev->addFunc(f2);
    emit game->newData();
}

void CPlayer::loseHP(int point)
{
    CEvent losehpEvent(game);
    this->HP-=point;
    if(HP<=0)
    {
        goingToDie();
    }
    phaseCallback(PHASE_LOSEHP,(void*)point);
}

void CPlayer::getCard(const QList<CCard *> &list, CPlayer *from)
{
    new CEvent(game);
    this->hands.append(list);
    QList<CCard*> l=list;
    phaseCallback(PHASE_GETCARD,&l,from);
    emit game->newData();
}

void CPlayer::discardCard(QList<CCard *> &list, int reason)
{
    CEvent ev(game);
    phaseCallback(PHASE_ABOUTTODISCARD,&list,(void*)reason);
    phaseCallback(PHASE_DISCARD,&list,(void*)reason);
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

void CPlayer::regPhase(int phase, CPlayerSkill *skill)
{
    phaseRegister[phase].append(skill);
}

void CPlayer::unregPhase(int phase, CPlayerSkill *skill)
{
    auto it=phaseRegister.find(phase);
    if(it!=phaseRegister.end())
    {
        it.value().removeAll(skill);
    }
}

void CPlayer::phaseCallback(int phase, void *extre, void *extre2, void *extre3, void *extre4)
{
    auto it=phaseRegister.find(phase);
    if(it!=phaseRegister.end())
    {
        QList<CPlayerSkill*> &list=it.value();
        if(list.length()>1) game->sortRegister(position,list);
        foreach (CPlayerSkill *ps, list) {
            ps->phaseCallback(phase,this,extre,extre2,extre3,extre4);
        }
    }
}

void CPlayer::loseCard(CCard *card)
{
    for(int i=0;i<5;i++)
    {
        if(zhuangBei[i]==card)
        {
            setZhuangBei(i,nullptr);
            return;
        }
    }
    for(int i=0;i<3;i++)
    {
        if(panDingQu[i]==card)
        {
            panDingQuRemove(card);
            return;
        }
    }
    removeHand(card);
}

void CPlayer::setZhuangBei(int pos, CCard *card)
{
    zhuangBei[pos]=card;
}

void CPlayer::goingToDie()
{

}

void CPlayer::setOffline(bool b)
{
    offline=b;
}

void CPlayer::setGeneral(CGeneral *g)
{
    this->general=g;
    setKingdom(g->kingdom);
    this->maxHP=g->maxHP;
    if(this->position==0&&game->players.size()>3)
        maxHP++;
    this->HP=maxHP;
    setMaxHP(maxHP);
    this->gender=g->gender;
    CPlayerSkill *ps;
    foreach (CSkill *skill, g->skill) {
        if(!skill->zhugong||(this->position==0&&game->players.size()>2))
        {
            ps=new CPlayerSkill(skill,this);
            this->skills.append(ps);
        }
    }
}

void CPlayer::setKingdom(quint8 kd)
{
    this->kingdom=kd;
}

void CPlayer::setMaxHP(quint8 mh)
{
    this->maxHP=mh;
}

void CPlayer::setHP(char hp)
{
    this->HP=hp;
}

void CPlayer::setRole(quint8 role)
{
    this->role=role;
}

void CPlayer::deliverCard(const QList<CCard *> &list)
{
    hands=list;
}

int CPlayer::distanceTo(CPlayer *player)
{
    int distance=game->countPositionDistance(this->position,player->position);
    distance-=this->jianYi;
    distance+=player->jiaYi;
    phaseCallback(PHASE_DISTANCE,player,&distance);
    if(distance<0) distance=0;
    return distance;
}

bool CPlayer::hasCard()
{
    if(hands.isEmpty())
    {
        for(int i=0;i<5;i++)
        {
            if(zhuangBei[i])
                return true;
        }
        for(int i=0;i<3;i++)
        {
            if(panDingQu[i])
                return true;
        }
        return false;
    }
    else
        return true;
}

void CPlayer::needPlay(quint8 , quint16 , const QList<quint8>, int number, CPlayer *, CCard *, int  , int , const QList<CPlayer *> )
{
    cardPlayed=false;
    playCount=number;
}

void CPlayer::setStateJiu(bool b)
{
    stateJiu=b;
}

void CPlayer::endPlayPhase()
{

}

void CPlayer::useCard(CCard *card, QList<CPlayer *> &list)
{
    removeHand(card);
    card->type->useCard(this,card,list);
}

bool CPlayer::containsCard(CCard *card, int mode)
{
    if(mode&CARDMODE_HANDS)
    {
        CCard *c;
        foreach (c, hands) {
            if(c==card) return true;
        }
    }
    if(mode&CARDMODE_ZHUANGBEI)
    {
        for(int i=0;i<5;i++)
        {
            if(zhuangBei[i]==card) return true;
        }
    }
    return false;
}

void CPlayer::removeHand(CCard *card)
{
    hands.removeOne(card);
}

void CPlayer::setTieSuoLianHuan(bool b)
{
    isTieSuoLianHuan=b;
}

void CPlayer::needSelectKingdom()
{
    QList<QVariant> dr;
    dr.append((int)0);
    QList<QVariant> values;
    values<<"魏"<<"蜀"<<"吴"<<"群";
    needSelect("选择武将",SELECTTYPE_CUSTOM,values,dr,1,1);
}

bool CPlayer::canBeTarget(CCard *card, CPlayer *player)
{
    return card->type->availableTargets(player,card).contains(this);
}

QList<CCard*> CPlayer::cardList(bool hand, bool zhuangbei, bool panding)
{
    QList<CCard*> list;
    if(hand&&!hands.isEmpty())
        list.append(nullptr);
    if(zhuangbei)
    {
        for(int i=0;i<5;i++)
        {
            if(zhuangBei[i]) list.append(zhuangBei[i]);
        }
    }
    if(panding)
    {
        for(int i=0;i<3;i++)
        {
            if(panDingQu[i]) list.append(panDingQu[i]);
        }
    }
    return list;
}

bool CPlayer::hasWuXieKeJi()
{
    CCard *card;
    foreach (card, hands) {
        if(card->type->type2==CARD_WUXIEKEJI)
            return true;
    }
    bool b=false;
    phaseCallback(PHASE_HASWUXIEKEJI,&b);
    return b;
}
