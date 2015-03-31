#include "ccard.h"

CTempCard::CTempCard(CCardType *source)
{
    this->temp=true;
    this->type=source;
    suit=SUIT_NONE;
    number=0;
}

CTempCard::CTempCard(CCard *card)
{
    this->temp=true;
    this->type=card->type;
    this->number=card->number;
    this->suit=card->suit;
    if(card->temp)
    {
        CTempCard *tcard=static_cast<CTempCard*>(card);
        originCards=tcard->originCards;
    }
    else
        this->originCards.append(card);
}
