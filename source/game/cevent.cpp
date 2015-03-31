#include "cevent.h"
#include "game/cgame.h"
#include "card/ccard.h"

CEvent::CEvent(CGame* game, int type)
{
    this->parent=game->currentEvent;
    this->game=game;
    this->type=type;
    game->currentEvent=this;
}

CEvent::~CEvent()
{
    game->currentEvent=parent;
    if(!cardsToDiscard.isEmpty())
    {
        CCard *card;
        foreach (quint8 id, cardsToDiscard) {
            card=CCard::find(id);
            game->deadwood.append(card);
        }
    }
    if(!tempCards.isEmpty())
    {
        qDeleteAll(tempCards);
    }
    if(funcDelete)
        funcDelete();
}

void CEvent::addFunc(std::function<void ()> func)
{
    funcs.append(func);
    game->checkSignalCount();
}

void CEvent::insertFunc(std::function<void ()> func)
{
    funcs.prepend(func);
    game->checkSignalCount();
}

void CEvent::addCard(CCard *card)
{
    if(card->temp)
    {
        CTempCard *tc=static_cast<CTempCard*>(card);
        CCard* oc;
        foreach (oc, tc->originCards) {
            cardsToDiscard.insert(oc->id);
        }
        tempCards.append(tc);
    }
    else
        this->cardsToDiscard.insert(card->id);
}

CEvent* CEvent::findParent(int type)
{
    CEvent* ev=this;
    while(ev->parent)
    {
        ev=ev->parent;
        if(ev->type==type)
            return ev;
    }
    return nullptr;
}

void CEvent::takeCard(quint8 id)
{
    CEvent *ev=this;
    while(ev)
    {
        ev->cardsToDiscard.remove(id);
        ev=ev->parent;
    }
}

void CEvent::takeCards(QList<quint8> list)
{
    CEvent *ev=this;
    quint8 id;
    while(ev)
    {
        foreach (id, list) {
            ev->cardsToDiscard.remove(id);
        }
        ev=ev->parent;
    }
}
