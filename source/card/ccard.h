#ifndef CCARD_H
#define CCARD_H

#define CARDTYPE_JIBEN 1
#define CARDTYPE_SHA 2
#define CARDTYPE_SHAN 4
#define CARDTYPE_TAO 8
#define CARDTYPE_JINNANG 16
#define CARDTYPE_FEIYANSHIJINNANG 32
#define CARDTYPE_YANSHIJINNANG 64
#define CARDTYPE_ZHUANGBEI 128
#define CARDTYPE_WUQI 256
#define CARDTYPE_FANGJU 512
#define CARDTYPE_JIAYIMA 1024
#define CARDTYPE_JIANYIMA 2048
#define CARDTYPE_JIU 4096
#define CARDTYPE_BAOWU 8192

#define CARDTYPE_PUTONGSHA 0
#define CARDTYPE_LEISHA 1
#define CARDTYPE_HUOSHA 2

#define CARDTYPE_GUOHECAIQIAO 0
#define CARDTYPE_SHUNSHOUQIANYANG 1
#define CARDTYPE_WUZHONGSHENGYOU 2
#define CARDTYPE_JIEDAOSHAREN 3
#define CARDTYPE_WUGUFENGDENG 4
#define CARDTYPE_TAOYUANJIEYI 5
#define CARDTYPE_WUXIEKEJI 6
#define CARDTYPE_HUOGONG 7
#define CARDTYPE_NANMANRUQIN 8
#define CARDTYPE_WANJIANQIFA 9
#define CARDTYPE_JUEDOU 10
#define CARDTYPE_TIESUOLIANHUAN 11
#define CARDTYPE_SHANDIAN 12
#define CARDTYPE_LEBUSISHU 13
#define CARDTYPE_BINGLIANGCUNDUAN 14


#define SUIT_HONGTAO 0
#define SUIT_FANGKUAI 1
#define SUIT_HEITAO 2
#define SUIT_MEIHUA 3
#define SUIT_WU 4

#define CARDPACKAGE_BIAO 0
#define CARDPACKAGE_EX 1
#define CARDPACKAGE_JUNZHENG 2
#define CARDPACKAGE_JIEXIANTUPO 3

class CCard
{
public:
    CCard();
    ~CCard();

    int id;
    int cardType,cardType2;
    int suit;
    int number;
    int package;
};

class CCardZhuangbei: public CCard
{
public:
    virtual void wearEvent();
    virtual void takeOffEvent();
    virtual void moveEvent();
    virtual void callback(int phase);
};


#endif // CCARD_H
