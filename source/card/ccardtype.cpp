#include "ccardtype.h"
#include "game/cplayer.h"
#include "general/cskill.h"

CCardType::CCardType()
{
    canSelectTarget=false;
    minTargets=1;
    maxTargets=1;
}

CCardType::~CCardType()
{

}

bool CCardType::canUse(CPlayer *player,CCard* card)
{
    bool b=true;
    player->phaseCallback(PHASE_CANUSECARD,card,&b);
    if(!b) return false;
    if(availableTargets(player,card).isEmpty())
        return false;
    else return true;
}

bool CCardType::canPlay(CPlayer *player, CCard *card)
{
    bool b=true;
    player->phaseCallback(PHASE_CANPLAYCARD,card,&b);
    return b;
}

void CCardType::useCard(CPlayer *, CCard* , QList<CPlayer *> &)
{

}

/*int CCardType::getMaxTargets(CPlayer *player, CCard *card)
{
    int min=minTargets;
    int max=maxTargets;
    player->phaseCallback(PHASE_CARDMINMAXTARGETS,card,&min,&max);
    return max;
}*/

bool CCardType::cardUseCheck(CPlayer *player, CCard *card, QList<CPlayer *> &list)
{
    int min=minTargets;
    int max=maxTargets;
    player->phaseCallback(PHASE_CARDMINMAXTARGETS,card,&min,&max);
    if(list.size()<min||list.size()>max) return false;
    auto alist=availableTargets(player,card);
    if(!canSelectTarget)
    {
        if(alist==list)
            return true;
        else
            return false;
    }
    else
    {
        CPlayer *pl;
        foreach (pl, list) {
            if(!alist.contains(pl))
                return false;
        }
        return true;
    }
}

QList<CPlayer*> CCardType::availableTargets(CPlayer*,CCard*)
{
    QList<CPlayer*> list;
    return list;
}

void CCardType::createType(CCardType **&types)
{
    types=new CCardType*[44];
    CCardType* t;

    t=new CCardSha;
    t->name="杀";
    t->type1=CARDTYPE_JIBEN|CARDTYPE_SHA;
    t->type2=CARD_PUTONGSHA;
    types[CARD_PUTONGSHA]=t;

    t=new CCardLeiSha;
    t->name="雷杀";
    t->type1=CARDTYPE_JIBEN|CARDTYPE_SHA;
    t->type2=1;
    types[1]=t;

    t=new CCardHuoSha;
    t->name="火杀";
    t->type1=CARDTYPE_JIBEN|CARDTYPE_SHA;
    t->type2=2;
    types[2]=t;

    t=new CCardGuoHeCaiQiao;
    t->name="过河拆桥";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=3;
    types[3]=t;

    t=new CCardShunShouQianYang;
    t->name="顺手牵羊";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=4;
    types[4]=t;

    t=new CCardWuZhongShengYou;
    t->name="无中生有";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=5;
    types[5]=t;

    t=new CCardJieDaoShaRen;
    t->name="借刀杀人";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=6;
    types[6]=t;

    t=new CCardWuGuFengDeng;
    t->name="五谷丰登";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=7;
    types[7]=t;

    t=new CCardTaoYuanJieYi;
    t->name="桃园结义";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=8;
    types[8]=t;

    t=new CCardType;
    t->name="无懈可击";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=9;
    types[9]=t;

    t=new CCardHuoGong;
    t->name="火攻";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=10;
    types[10]=t;

    t=new CCardNanManRuQin;
    t->name="南蛮入侵";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=11;
    types[11]=t;

    t=new CCardWanJianQiFa;
    t->name="万箭齐发";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=12;
    types[12]=t;

    t=new CCardJueDou;
    t->name="决斗";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=13;
    types[13]=t;

    t=new CCardTieSuoLianHuan;
    t->name="铁索连环";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_FEIYANSHIJINNANG;
    t->type2=14;
    types[14]=t;

    t=new CCardShanDian;
    t->name="闪电";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_YANSHIJINNANG;
    t->type2=15;
    types[15]=t;

    t=new CCardLeBuSiShu;
    t->name="乐不思蜀";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_YANSHIJINNANG;
    t->type2=16;
    types[16]=t;

    t=new CCardBingLiangCunDuan;
    t->name="兵粮寸断";
    t->type1=CARDTYPE_JINNANG|CARDTYPE_YANSHIJINNANG;
    t->type2=17;
    types[17]=t;

    CCardZhugeliannu *tln=new CCardZhugeliannu;
    tln->name="诸葛连弩";
    tln->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tln->type2=18;
    tln->distance=1;
    tln->active=false;
    types[18]=tln;

    CCardCixiongshuanggujian *tcx=new CCardCixiongshuanggujian;
    tcx->name="雌雄双股剑";
    tcx->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tcx->type2=19;
    tcx->distance=2;
    tcx->active=false;
    types[19]=tcx;

    CCardQinggangjian *tqg=new CCardQinggangjian;
    tqg->name="青釭剑";
    tqg->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tqg->type2=20;
    tqg->distance=2;
    tqg->active=false;
    types[20]=tqg;

    CCardQinglongyanyuedao *qqld=new CCardQinglongyanyuedao;
    qqld->name="青龙偃月刀";
    qqld->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    qqld->type2=21;
    qqld->distance=3;
    qqld->active=false;
    types[21]=qqld;

    CCardZhangbashemao *tzbsm=new CCardZhangbashemao;
    tzbsm->name="丈八蛇矛";
    tzbsm->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tzbsm->type2=22;
    tzbsm->distance=3;
    tzbsm->active=true;
    types[22]=tzbsm;

    CCardGuanshifu *tgsf=new CCardGuanshifu;
    tgsf->name="贯石斧";
    tgsf->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tgsf->type2=23;
    tgsf->distance=3;
    tgsf->active=false;
    types[23]=tgsf;

    CCardFangtianhuaji *tfthj=new CCardFangtianhuaji;
    tfthj->name="方天画戟";
    tfthj->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tfthj->type2=24;
    tfthj->distance=4;
    tfthj->active=false;
    types[24]=tfthj;

    CCardQilingong *tqlg=new CCardQilingong;
    tqlg->name="麒麟弓";
    tqlg->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tqlg->type2=25;
    tqlg->distance=5;
    tqlg->active=false;
    types[25]=tqlg;

    CCardBaguazhen *tbgz=new CCardBaguazhen;
    tbgz->name="八卦阵";
    tbgz->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_FANGJU;
    tbgz->type2=26;
    tbgz->active=false;
    types[26]=tbgz;

    CCardHanbingjian *thbj=new CCardHanbingjian;
    thbj->name="寒冰剑";
    thbj->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    thbj->type2=27;
    thbj->distance=2;
    thbj->active=false;
    types[27]=thbj;

    CCardRenwangdun *trwd=new CCardRenwangdun;
    trwd->name="仁王盾";
    trwd->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_FANGJU;
    trwd->type2=28;
    trwd->active=false;
    types[28]=trwd;

    CCardGudingdao *tgdd=new CCardGudingdao;
    tgdd->name="古锭刀";
    tgdd->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    tgdd->type2=29;
    tgdd->distance=2;
    tgdd->active=false;
    types[29]=tgdd;

    CCardTengjia *ttj=new CCardTengjia;
    ttj->name="藤甲";
    ttj->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_FANGJU;
    ttj->type2=30;
    ttj->active=false;
    types[30]=ttj;

    CCardBaiyinshizi *bysz=new CCardBaiyinshizi;
    bysz->name="白银狮子";
    bysz->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_FANGJU;
    bysz->type2=31;
    bysz->active=false;
    types[31]=bysz;

    CCardZhuqueyushan *zqys=new CCardZhuqueyushan;
    zqys->name="朱雀羽扇";
    zqys->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_WUQI;
    zqys->type2=32;
    zqys->distance=4;
    zqys->active=false;
    types[32]=zqys;

    CCardMuniuliuma *mnlm=new CCardMuniuliuma;
    mnlm->name="木牛流马";
    mnlm->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_BAOWU;
    mnlm->type2=33;
    mnlm->active=true;
    types[33]=mnlm;

    t=new CCardType;
    t->name="闪";
    t->type1=CARDTYPE_JIBEN|CARDTYPE_SHAN;
    t->type2=34;
    types[34]=t;

    t=new CCardTao;
    t->name="桃";
    t->type1=CARDTYPE_JIBEN|CARDTYPE_TAO;
    t->type2=35;
    types[35]=t;

    t=new CCardJiu;
    t->name="酒";
    t->type1=CARDTYPE_JIBEN|CARDTYPE_JIU;
    t->type2=36;
    types[36]=t;

    CCardJiayima *j=new CCardJiayima;
    j->name="绝影";
    j->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_JIAYIMA;
    j->type2=37;
    types[37]=j;

    j=new CCardJiayima;
    j->name="的卢";
    j->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_JIAYIMA;
    j->type2=38;
    types[38]=j;

    j=new CCardJiayima;
    j->name="爪黄飞田";
    j->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_JIAYIMA;
    j->type2=39;
    types[39]=j;

    CCardJianyima *a=new CCardJianyima;
    a->name="赤兔";
    a->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_JIANYIMA;
    a->type2=40;
    types[40]=a;

    a=new CCardJianyima;
    a->name="大宛";
    a->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_JIANYIMA;
    a->type2=41;
    types[41]=a;

    a=new CCardJianyima;
    a->name="紫骍";
    a->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_JIANYIMA;
    a->type2=42;
    types[42]=a;

    j=new CCardJiayima;
    j->name="骅骝";
    j->type1=CARDTYPE_ZHUANGBEI|CARDTYPE_JIAYIMA;
    j->type2=43;
    types[43]=j;
}
