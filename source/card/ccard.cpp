#include "ccard.h"
#include "mainwindow.h"
extern MainWindow *w;

#define NEWCARD {c=new CCard;c->id=i++;}

CCard::CCard()
{
    temp=false;
}

CCard::~CCard()
{

}

QString CCard::numberString()
{
    switch (number) {
    case 1:
        return "A";
    case 11:
        return "J";
    case 12:
        return "Q";
    case 13:
        return "K";
    default:
        return QString::number(number);
    }
}

void CCard::init(CCardType** &types,QList<CCard *> &cardsBiao,QList<CCard *> &cardsEx,QList<CCard *> &cardsJunzheng,QList<CCard *> &cardsJiexiantupo)
{
    CCard* c;
    int i=0;

    CCardType::createType(types);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=1;
    c->type=types[CARD_JUEDOU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=1;
    c->type=types[CARD_SHANDIAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=2;
    c->type=types[CARD_BAGUAZHEN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=2;
    c->type=types[CARD_CIXIONGSHUANGGUJIAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=3;
    c->type=types[CARD_GUOHECAIQIAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=3;
    c->type=types[CARD_SHUNSHOUQIANYANG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=4;
    c->type=types[CARD_GUOHECAIQIAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=4;
    c->type=types[CARD_SHUNSHOUQIANYANG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=5;
    c->type=types[CARD_QINGLONGYANYUEDAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=5;
    c->type=types[CARD_JUEYING];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=6;
    c->type=types[CARD_LEBUSISHU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=6;
    c->type=types[CARD_QINGGANGJIAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=7;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=7;
    c->type=types[CARD_NANMANRUQIN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=8;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=8;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=9;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=9;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=10;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=10;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=11;
    c->type=types[CARD_SHUNSHOUQIANYANG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=11;
    c->type=types[CARD_WUXIEKEJI];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=12;
    c->type=types[CARD_GUOHECAIQIAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=12;
    c->type=types[CARD_ZHANGBASHEMAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=13;
    c->type=types[CARD_NANMANRUQIN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=13;
    c->type=types[CARD_DAWAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=1;
    c->type=types[CARD_WANJIANQIFA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=1;
    c->type=types[CARD_TAOYUANJIEYI];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=2;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=2;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=3;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=3;
    c->type=types[CARD_WUGUFENGDENG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=4;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=4;
    c->type=types[CARD_WUGUFENGDENG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=5;
    c->type=types[CARD_QILINGONG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=5;
    c->type=types[CARD_CHITU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=6;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=6;
    c->type=types[CARD_LEBUSISHU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=7;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=7;
    c->type=types[CARD_WUZHONGSHENGYOU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=8;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=8;
    c->type=types[CARD_WUZHONGSHENGYOU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=9;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=9;
    c->type=types[CARD_WUZHONGSHENGYOU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=10;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=10;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=11;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=11;
    c->type=types[CARD_WUZHONGSHENGYOU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=12;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=12;
    c->type=types[CARD_GUOHECAIQIAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=13;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=13;
    c->type=types[CARD_ZHUAHUANGFEITIAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=1;
    c->type=types[CARD_JUEDOU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=1;
    c->type=types[CARD_ZHUGELIANNU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=2;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=2;
    c->type=types[CARD_BAGUAZHEN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=3;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=3;
    c->type=types[CARD_GUOHECAIQIAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=4;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=4;
    c->type=types[CARD_GUOHECAIQIAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=5;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=5;
    c->type=types[CARD_DILU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=6;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=6;
    c->type=types[CARD_LEBUSISHU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=7;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=7;
    c->type=types[CARD_NANMANRUQIN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=8;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=8;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=9;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=9;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=10;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=10;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=11;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=11;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=12;
    c->type=types[CARD_JIEDAOSHAREN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=12;
    c->type=types[CARD_WUXIEKEJI];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=13;
    c->type=types[CARD_JIEDAOSHAREN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=13;
    c->type=types[CARD_WUXIEKEJI];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=1;
    c->type=types[CARD_JUEDOU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=1;
    c->type=types[CARD_ZHUGELIANNU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=2;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=2;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=3;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=3;
    c->type=types[CARD_SHUNSHOUQIANYANG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=4;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=4;
    c->type=types[CARD_SHUNSHOUQIANYANG];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=5;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=5;
    c->type=types[CARD_GUANSHIFU];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=6;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=6;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=7;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=7;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=8;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=8;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=9;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=9;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=10;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=10;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=11;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=11;
    c->type=types[CARD_SHAN];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=12;
    c->type=types[CARD_TAO];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=12;
    c->type=types[CARD_FANGTIANHUAJI];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=13;
    c->type=types[CARD_PUTONGSHA];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=13;
    c->type=types[CARD_ZIXING];
    cardsBiao.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=2;
    c->type=types[CARD_HANBINGJIAN];
    cardsEx.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=2;
    c->type=types[CARD_RENWANGDUN];
    cardsEx.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=12;
    c->type=types[CARD_SHANDIAN];
    cardsEx.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=12;
    c->type=types[CARD_WUXIEKEJI];
    cardsEx.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=1;
    c->type=types[CARD_GUDINGDAO];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=2;
    c->type=types[CARD_TENGJIA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=3;
    c->type=types[CARD_JIU];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=4;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=5;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=6;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=7;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=8;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=9;
    c->type=types[CARD_JIU];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=10;
    c->type=types[CARD_BINGLIANGCUNDUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=11;
    c->type=types[CARD_TIESUOLIANHUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=12;
    c->type=types[CARD_TIESUOLIANHUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HEITAO;
    c->number=13;
    c->type=types[CARD_WUXIEKEJI];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=1;
    c->type=types[CARD_WUXIEKEJI];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=2;
    c->type=types[CARD_HUOGONG];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=3;
    c->type=types[CARD_HUOSHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=4;
    c->type=types[CARD_HUOGONG];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=5;
    c->type=types[CARD_TAO];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=6;
    c->type=types[CARD_TAO];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=7;
    c->type=types[CARD_HUOSHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=8;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=9;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=10;
    c->type=types[CARD_HUOSHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=11;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=12;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_HONGTAO;
    c->number=13;
    c->type=types[CARD_WUXIEKEJI];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=1;
    c->type=types[CARD_BAIYINSHIZI];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=2;
    c->type=types[CARD_TENGJIA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=3;
    c->type=types[CARD_JIU];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=4;
    c->type=types[CARD_BINGLIANGCUNDUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=5;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=6;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=7;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=8;
    c->type=types[CARD_LEISHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=9;
    c->type=types[CARD_JIEDAOSHAREN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=10;
    c->type=types[CARD_TIESUOLIANHUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=11;
    c->type=types[CARD_TIESUOLIANHUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=12;
    c->type=types[CARD_TIESUOLIANHUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_MEIHUA;
    c->number=13;
    c->type=types[CARD_TIESUOLIANHUAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=1;
    c->type=types[CARD_ZHUQUEYUSHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=2;
    c->type=types[CARD_TAO];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=3;
    c->type=types[CARD_TAO];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=4;
    c->type=types[CARD_HUOSHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=5;
    c->type=types[CARD_HUOSHA];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=6;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=7;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=8;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=9;
    c->type=types[CARD_JIU];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=10;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=11;
    c->type=types[CARD_SHAN];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=12;
    c->type=types[CARD_WUXIEKEJI];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=13;
    c->type=types[CARD_HUALIU];
    cardsJunzheng.append(c);

    NEWCARD
    c->suit=SUIT_FANGKUAI;
    c->number=5;
    c->type=types[CARD_MUNIULIUMA];
    cardsJiexiantupo.append(c);

}

CCard* CCard::find(quint8 id)
{
    if(id<104)
        return w->cardBiao[id];
    else if(id<108)
        return w->cardEx[id-104];
    else if(id<160)
        return w->cardJunzheng[id-108];
    else if(id<161)
        return w->cardJiexiantupo[0];
    else
        return nullptr;
}
