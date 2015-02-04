#ifndef CGAME_H
#define CGAME_H

#include "general/cgeneral.h"
#include "card/ccard.h"
#include "general/cskill.h"
#include "game/cplayer.h"
#include "game/cruntimeevent.h"

class CGame
{
public:
    static CCard *originCards;
    static CGeneral *originGenerals,*derivedGenerals;
    static CSkill *originSkills;
    CGame();
    ~CGame();
    void shuffle();

    QList<CCard*> deck,deadwood;
    uint round,currentPosition,nextPosition;
    int currentPhase;
    QVector<CPlayer*> players;
    CRuntimeEvent *baseEvents;
};

#endif // CGAME_H
