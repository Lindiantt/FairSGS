#include "cgame.h"
#include "cplayer.h"
#include "general/cgeneral.h"
#include "general/cskill.h"
#include "game/cevent.h"

CGame::CGame()
{
    gameOver=false;
    currentEvent=nullptr;
    signalCount=0;
    connect(this,this->signalNextRound,this,this->handleNextRound,Qt::QueuedConnection);
    connect(this,this->newData,this,this->doFuncs,Qt::QueuedConnection);
}

CGame::~CGame()
{
    while(currentEvent)
    {
        delete currentEvent;
    }
}

void CGame::s4skillSetup()
{
    CPlayer* player;
    foreach (player, players) {
        foreach (CPlayerSkill* skill,player->skills) {
            skill->getEvent();
        }
    }
    round=0;
    nextPosition=0;
    foreach (player, players) {
        if(player->kingdom==KINGDOM_SHEN)
        {
            auto f=std::bind(player->needSelectKingdom,player);
            addFunc(f);
            auto f2=std::bind([&](CPlayer* player){
                player->setKingdom(player->selectionList[0].toUInt());
                emit newData();
            },player);
            addFunc(f2);
        }
    }
    addFunc(std::bind(s5gameStart,this));
    emit newData();
}

void CGame::s5gameStart()
{
    if(gameStartRegister.isEmpty())
    {
        emit signalNextRound();
    }
    else
    {
        QList<CPlayerSkill *> list;
        list=gameStartRegister;
        gameStartRegister.clear();
        if(list.length()>1) sortRegister(0,list);
        foreach (CPlayerSkill *ps, list) {
            ps->phaseCallback(PHASE_GAMESTART);
        }
        addFunc(std::bind(s5gameStart,this));
        emit newData();
    }
}

void CGame::regGameStart(CPlayerSkill *skill)
{
    gameStartRegister.append(skill);
}

void CGame::handleNextRound()
{
    CPlayer *player=players[nextPosition];
    if(!player->isAlive)
        player=nextAlivePlayer(nextPosition);
    currentPosition=player->position;
    nextPosition=currentPosition+1;
    if(nextPosition>=players.length())
        nextPosition=0;
    player->roundInit();
}

CPlayer* CGame::nextAlivePlayer(int pos)
{
    do
    {
        pos++;
        if(pos>=players.length()) pos=0;
    }while(!players[pos]->isAlive);
    return players[pos];
}

void CGame::sortRegister(int cp, QList<CPlayerSkill *> &list)
{
    std::sort(list.begin(),list.end(),[=](CPlayerSkill* ps1,CPlayerSkill* ps2){
        if(ps1->skill->priority < ps2->skill->priority)
            return false;
        else if(ps1->skill->priority > ps2->skill->priority)
            return true;
        else
        {
            if(ps1->player->position<cp&&ps2->player->position>=cp)
                return false;
            else if(ps1->player->position>=cp&&ps2->player->position<cp)
                return true;
            else if(ps1->player->position<ps2->player->position)
                return true;
            else
                return false;
        }
    });
}

void CGame::doFuncs()
{
    signalCount++;
    while(signalCount&&(currentEvent||!funcs.isEmpty()))
    {
        while(currentEvent)
        {
            if(currentEvent->funcs.isEmpty())
            {
                delete currentEvent;
                continue;
            }
            auto func=currentEvent->funcs.takeFirst();
            signalCount--;
            func();
            goto aa;
        }
        if(!funcs.isEmpty())
        {
            auto func=funcs.takeFirst();
            signalCount--;
            func();
        }
        aa:;
    }
}

void CGame::addFunc(std::function<void ()> func)
{
    funcs.append(func);
    checkSignalCount();
}

void CGame::insertFunc(std::function<void ()> func)
{
    funcs.prepend(func);
    checkSignalCount();
}

void CGame::insertMultiFunc(std::function<void ()> func)
{
    insertions.append(func);
}

void CGame::insertOver()
{
    auto it=insertions.end();
    while(it!=insertions.begin())
    {
        it--;
        funcs.prepend(*it);
    }
    insertions.clear();
    checkSignalCount();
}

void CGame::checkSignalCount()
{
    if(signalCount)
    {
        signalCount--;
        emit newData();
    }
}

void CGame::roundEnd()
{
    if(!gameOver)
    {
        round++;
        emit signalNextRound();
    }
}

void CGame::smartAddFunc(std::function<void ()> func)
{
    if(currentEvent)
        currentEvent->addFunc(func);
    else
        addFunc(func);
}

void CGame::smartInsertFunc(std::function<void ()> func)
{
    if(currentEvent)
        currentEvent->insertFunc(func);
    else
        insertFunc(func);
}

void CGame::smartInsertOver()
{
    auto it=insertions.end();
    while(it!=insertions.begin())
    {
        it--;
        if(currentEvent)
            currentEvent->funcs.prepend(*it);
        else
            funcs.prepend(*it);
    }
    insertions.clear();
    checkSignalCount();
}

QList<CCard *> CGame::getDrawnCards()
{
    QList<CCard*> vec=vec.fromVector(drawnCards);
    drawnCards.clear();
    return vec;
}

QSet<quint8> CGame::allPlayers()
{
    QSet<quint8> s;
    for(quint8 i=0;i<players.size();i++)
    {
        s.insert(i);
    }
    return s;
}

quint8 CGame::countPositionDistance(quint8 pos1, quint8 pos2)
{
    quint8 dis1,dis2;
    quint8 skip;
    char td;
    dis1=1;
    skip=0;
    for(;;)
    {
        td=pos1+dis1+skip;
        if(td>=players.size()) td-=players.size();
        if(td==pos2) break;
        if(players[td]->isAlive)
            dis1++;
        else
            skip++;
    }
    dis2=1;
    skip=0;
    while(dis2<dis1)
    {
        td=pos1-dis2-skip;
        if(td<0) td+=players.size();
        if(td==pos2) break;
        if(players[td]->isAlive)
            dis2++;
        else
            skip++;
    }
    return dis2;
}

void CGame::needWuXieKeJi()
{
    wuxiePlayed=false;
}
