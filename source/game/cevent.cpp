#include "cevent.h"
#include "game/cgame.h"

CEvent::CEvent(CGame* game)
{
    this->parent=game->currentEvent;
    this->game=game;
    game->currentEvent=this;
    this->available=true;
}

CEvent::~CEvent()
{
    game->currentEvent=parent;
    if(funcDelete)
        funcDelete();
}
