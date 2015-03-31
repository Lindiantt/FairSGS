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
class CCard;

class CCardType
{
public:
    CCardType();
    ~CCardType();
    static void createType(CCardType** &);
    virtual void useCard(CPlayer*,CCard*,QList<CPlayer*> &);
    virtual bool canUse(CPlayer*,CCard*);
    virtual bool canPlay(CPlayer*,CCard*);
    virtual QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    virtual bool cardUseCheck(CPlayer*,CCard*,QList<CPlayer*> &);
    quint16 type1;
    quint8 type2;
    QString name;
    int minTargets,maxTargets;
    bool canSelectTarget;
};

class CCardSha: public CCardType
{
public:
    CCardSha();
    bool canUse(CPlayer*,CCard*);
    void useCard(CPlayer*,CCard*,QList<CPlayer*> &);
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    int attribute;
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
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
};

class CCardTao: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
};

class CCardGuoHeCaiQiao: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    CCardGuoHeCaiQiao();
};

class CCardShunShouQianYang: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    CCardShunShouQianYang();
};

class CCardWuZhongShengYou: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    //CCardWuZhongShengYou();
};

class CCardJieDaoShaRen: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    bool cardUseCheck(CPlayer*,CCard*,QList<CPlayer*> &);
    CCardJieDaoShaRen();
};

class CCardWuGuFengDeng: public CCardType
{
public:
    CCardWuGuFengDeng();
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
};

class CCardTaoYuanJieYi: public CCardType
{
public:
    CCardTaoYuanJieYi();
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
};

class CCardHuoGong: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    CCardHuoGong();
};

class CCardNanManRuQin: public CCardType
{
public:
    CCardNanManRuQin();
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
};

class CCardWanJianQiFa: public CCardType
{
public:
    CCardWanJianQiFa();
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
};

class CCardJueDou: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    CCardJueDou();
};

class CCardTieSuoLianHuan: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    CCardTieSuoLianHuan();
};

class CCardShanDian: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
};

class CCardLeBuSiShu: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    CCardLeBuSiShu();
};

class CCardBingLiangCunDuan: public CCardType
{
public:
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    CCardBingLiangCunDuan();
};

class CCardZhuangbei: public CCardType
{
public:
    bool active;
    virtual void wearEvent(CPlayer *);
    virtual void takeOffEvent(CPlayer *);
    virtual void moveEvent(CPlayer *,CPlayer *);
    virtual void callback(CPlayer *,int phase);
    QList<CPlayer*> availableTargets(CPlayer*,CCard*);
    //void useCard(CPlayer*);
    //bool canUse(CPlayer*);
};

class CCardWuqi: public CCardZhuangbei
{
public:
    QString distanceChinese();
    quint8 distance;
    quint8 iconType;
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
