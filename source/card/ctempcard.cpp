#include "ccard.h"

CTempCard::CTempCard(CCardType *source)
{
    this->temp=true;
    this->type=source;
}
