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

#define CARDTYPE_ZHUGELIANNU 0
#define CARDTYPE_CIXIONGSHUANGGUJIAN 1
#define CARDTYPE_QINGGANGJIAN 2
#define CARDTYPE_QINGLONGYANYUEDAO 3
#define CARDTYPE_ZHANGBASHEMAO 4
#define CARDTYPE_GUANSHIFU 5
#define CARDTYPE_FANGTIANHUAJI 6
#define CARDTYPE_QILINGONG 7
#define CARDTYPE_BAGUAZHEN 8
#define CARDTYPE_HANBINGJIAN 9
#define CARDTYPE_RENWANGDUN 10
#define CARDTYPE_GUDINGDAO 11
#define CARDTYPE_TENGJIA 12
#define CARDTYPE_BAIYINSHIZI 13
#define CARDTYPE_ZHUQUEYUSHAN 14
#define CARDTYPE_MUNIULIUMA 15

#define SUIT_HONGTAO 0
#define SUIT_FANGKUAI 1
#define SUIT_HEITAO 2
#define SUIT_MEIHUA 3
#define SUIT_WU 4

#define CARDPACKAGE_BIAO 0
#define CARDPACKAGE_EX 1
#define CARDPACKAGE_JUNZHENG 2
#define CARDPACKAGE_JIEXIANTUPO 3

class CPlayer;

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
    bool active;
    virtual void wearEvent(CPlayer *);
    virtual void takeOffEvent(CPlayer *);
    virtual void moveEvent(CPlayer *,CPlayer *);
    virtual void callback(CPlayer *,int phase);
};

class CCardWuqi: public CCardZhuangbei
{
public:
    int distance;
};

class CCardZhugeliannu: public CCardWuqi
{

};

class CCardCixiongshuanggujian :public CCardWuqi
{

};

class CCardQinggangjian:public CCardWuqi
{

};

class CCardQinglongyanyuedao:public CCardWuqi
{

};

class CCardZhangbashemao:public CCardWuqi
{

};

class CCardGuanshifu:public CCardWuqi
{

};

class CCardFangtianhuaji:public CCardWuqi
{

};

class CCardQilingong:public CCardWuqi
{

};

class CCardBaguazhen:public CCardZhuangbei
{

};

class CCardHanbingjian:public CCardZhuangbei
{

};

class CCardRenwangdun :public CCardZhuangbei
{

};

class CCardGudingdao:public CCardWuqi
{

};

class CCardTengjia:public CCardZhuangbei
{

};

class CCardBaiyinshizi:public CCardZhuangbei
{

};

class CCardZhuqueyushan:public CCardWuqi
{

};

class CCardMuniuliuma:public CCardZhuangbei
{

};


#endif // CCARD_H
