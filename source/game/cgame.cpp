#include "cgame.h"
#include "cplayer.h"
#include "general/cgeneral.h"
#include "general/cskill.h"

CGame::CGame()
{
    currentEvent=nullptr;
    connect(this,this->signalNextRound,this,this->handleNextRound);
}

CGame::~CGame()
{

}

void CGame::s4skillSetup()
{
    foreach (CPlayer* player, players) {
        foreach (CSkill* skill,player->general->skill) {
            skill->getEvent(player);
        }
    }
}

void CGame::s5gameStart()
{
    auto it=gameStartRegister.begin();
    while(it!=gameStartRegister.end())
    {
        CSkill* skill=it.key();
        CPlayer* player=it.value();
        skill->phaseCallback(player,PHASE_GAMESTART);
        it++;
    }
    gameStartRegister.clear();
    round=0;
    currentPosition=0;
    nextPosition=1;
}

void CGame::regGameStart(CSkill *skill, CPlayer *player)
{
    gameStartRegister.insert(skill,player);
}

void CGame::handleNextRound()
{
    currentPosition=nextPosition;
    nextPosition++;
    if(nextPosition>=players.length())
    {

    }
}

CPlayer* CGame::nextAlivePlayer(int)
{
    return nullptr;
}
