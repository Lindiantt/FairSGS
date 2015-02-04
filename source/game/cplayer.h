#ifndef CPLAYER_H
#define CPLAYER_H

#include "general/cgeneral.h"
#include "card/ccard.h"
#include <qlist.h>

struct SPanDingPai
{
    CCard* source;
    int actAs;
};

#define IDENTITY_ZHU 0
#define IDENTITY_ZHONG 1
#define IDENTITY_FAN 2
#define IDENTITY_NEI 3
#define IDENTITY_UNKNOWN 4

class CPlayer
{
public:
    CPlayer();
    ~CPlayer();
    int position;
    CGeneral* general;
    QList<SPanDingPai> panDingQu;
    int identity;
    CCardZhuangbei *wuQi,*fangJu,*jiaYiMa,*jianYiMa,*baoWu;
    QList<CCard*> hands;
    int numberOfHands;
    int jiaYi,jianYi;
    int shaDistance;
    bool alive;
    QString nick;
};

#endif // CPLAYER_H
