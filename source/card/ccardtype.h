#ifndef CCARDTYPE_H
#define CCARDTYPE_H

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

#define CARD_PUTONGSHA 0
#define CARD_LEISHA 1
#define CARD_HUOSHA 2

#define CARD_GUOHECAIQIAO 3
#define CARD_SHUNSHOUQIANYANG 4
#define CARD_WUZHONGSHENGYOU 5
#define CARD_JIEDAOSHAREN 6
#define CARD_WUGUFENGDENG 7
#define CARD_TAOYUANJIEYI 8
#define CARD_WUXIEKEJI 9
#define CARD_HUOGONG 10
#define CARD_NANMANRUQIN 11
#define CARD_WANJIANQIFA 12
#define CARD_JUEDOU 13
#define CARD_TIESUOLIANHUAN 14
#define CARD_SHANDIAN 15
#define CARD_LEBUSISHU 16
#define CARD_BINGLIANGCUNDUAN 17

#define CARD_ZHUGELIANNU 18
#define CARD_CIXIONGSHUANGGUJIAN 19
#define CARD_QINGGANGJIAN 20
#define CARD_QINGLONGYANYUEDAO 21
#define CARD_ZHANGBASHEMAO 22
#define CARD_GUANSHIFU 23
#define CARD_FANGTIANHUAJI 24
#define CARD_QILINGONG 25
#define CARD_BAGUAZHEN 26
#define CARD_HANBINGJIAN 27
#define CARD_RENWANGDUN 28
#define CARD_GUDINGDAO 29
#define CARD_TENGJIA 30
#define CARD_BAIYINSHIZI 31
#define CARD_ZHUQUEYUSHAN 32
#define CARD_MUNIULIUMA 33

#define CARD_SHAN 34
#define CARD_TAO 35
#define CARD_JIU 36
#define CARD_JUEYING 37
#define CARD_DILU 38
#define CARD_ZHUAHUANGFEITIAN 39
#define CARD_CHITU 40
#define CARD_DAWAN 41
#define CARD_ZIXING 42
#define CARD_HUALIU 43

#define CARDPACKAGE_BIAO 0
#define CARDPACKAGE_EX 1
#define CARDPACKAGE_JUNZHENG 2
#define CARDPACKAGE_JIEXIANTUPO 3

#include "pch.h"

class CPlayer;

class CCardType
{
public:
    CCardType();
    ~CCardType();
    static void createType(CCardType** &);
    virtual void useCard(CPlayer*);
    virtual bool canUse(CPlayer*);
    quint16 type1;
    quint8 type2;
    QString name;
};

class CCardSha: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardLeiSha:public CCardSha
{
    //void useCard(CPlayer*);
};

class CCardHuoSha:public CCardSha
{
    //void useCard(CPlayer*);
};

class CCardJiu: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardTao: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardGuoHeCaiQiao: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardShunShouQianYang: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardWuZhongShengYou: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardJieDaoShaRen: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardWuGuFengDeng: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardTaoYuanJieYi: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardHuoGong: public CCardType
{
  //  void useCard(CPlayer*);
//    bool canUse(CPlayer*);
};

class CCardNanManRuQin: public CCardType
{
    //void useCard(CPlayer*);
//    bool canUse(CPlayer*);
};

class CCardWanJianQiFa: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardJueDou: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardTieSuoLianHuan: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardShanDian: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardLeBuSiShu: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardBingLiangCunDuan: public CCardType
{
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardZhuangbei: public CCardType
{
public:
    bool active;
    virtual void wearEvent(CPlayer *);
    virtual void takeOffEvent(CPlayer *);
    virtual void moveEvent(CPlayer *,CPlayer *);
    virtual void callback(CPlayer *,int phase);
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardWuqi: public CCardZhuangbei
{
public:
    quint8 distance;
    //void useCard(CPlayer*);
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

class CCardHanbingjian:public CCardWuqi
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

class CCardJiayima:public CCardZhuangbei
{

};

class CCardJianyima:public CCardZhuangbei
{

};

#endif // CCARDTYPE_H
