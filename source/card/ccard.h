#ifndef CCARD_H
#define CCARD_H

#define SUIT_HONGTAO 1
#define SUIT_FANGKUAI 2
#define SUIT_HEITAO 4
#define SUIT_MEIHUA 8
#define SUIT_NONE 16

#define COLOR_RED 0
#define COLOR_BLACK 1
#define COLOR_NONE 2


#include "card/ccardtype.h"
class CImage;

class CCard
{
public:
    CCard();
    ~CCard();
    static void init(CCardType** &cardType,QList<CCard*> &cardsBiao, QList<CCard *> &cardsEx, QList<CCard *> &cardsJunzheng, QList<CCard *> &cardsJiexiantupo);
    static CCard* find(quint8);
    quint8 id;
    quint8 suit;
    CImage *suitIcon();
    QString numberString();
    QString description();
    quint8 number;
    bool temp;
    CCardType* type;
};

class CTempCard:public CCard
{
public:
    CTempCard(CCardType*);
    CTempCard(CCard*);
    QList<CCard*> originCards;
};

#endif // CCARD_H
