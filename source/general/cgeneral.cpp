#include "cgeneral.h"
#include "cskill.h"
#include "network/define.h"

#include "mainwindow.h"
extern MainWindow *w;

#define NEWGENERAL {i++;g[i]=new CGeneral();g[i]->id=i;}

CGeneral::CGeneral()
{
    zhuGong=false;
    gender=true;
    derivedFrom=-1;
}

CGeneral::~CGeneral()
{

}

CGeneral** CGeneral::createAll()
{
    CGeneral **g;
    g=new CGeneral*[MAX_GENERAL];
    int i=-1;

    NEWGENERAL;g[i]->nick="魏武帝";
    g[i]->name="曹操";
    g[i]->kingdom=KINGDOM_WEI;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;
    g[i]->zhuGong=true;

    NEWGENERAL;g[i]->nick="狼顾之鬼";
    g[i]->name="司马懿";
    g[i]->kingdom=KINGDOM_WEI;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="独眼的罗刹";
    g[i]->name="夏侯惇";
    g[i]->kingdom=KINGDOM_WEI;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="前将军";
    g[i]->name="张辽";
    g[i]->kingdom=KINGDOM_WEI;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="虎痴";
    g[i]->name="许禇";
    g[i]->kingdom=KINGDOM_WEI;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="早终的先知";
    g[i]->name="郭嘉";
    g[i]->kingdom=KINGDOM_WEI;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="薄幸的美人";
    g[i]->name="甄姬";
    g[i]->gender=false;
    g[i]->kingdom=KINGDOM_WEI;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="乱世的枭雄";
    g[i]->name="刘备";
    g[i]->kingdom=KINGDOM_SHU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;
    g[i]->zhuGong=true;

    NEWGENERAL;g[i]->nick="美髯公";
    g[i]->name="关羽";
    g[i]->kingdom=KINGDOM_SHU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="万夫不当";
    g[i]->name="张飞";
    g[i]->kingdom=KINGDOM_SHU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="迟暮的丞相";
    g[i]->name="诸葛亮";
    g[i]->kingdom=KINGDOM_SHU;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="少年将军";
    g[i]->name="赵云";
    g[i]->kingdom=KINGDOM_SHU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="一骑当千";
    g[i]->name="马超";
    g[i]->kingdom=KINGDOM_SHU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="归隐的杰女";
    g[i]->name="黄月英";
    g[i]->gender=false;
    g[i]->kingdom=KINGDOM_SHU;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="年轻的贤君";
    g[i]->name="孙权";
    g[i]->zhuGong=true;
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="锦帆游侠";
    g[i]->name="甘宁";
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="白衣渡江";
    g[i]->name="吕蒙";
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="轻身为国";
    g[i]->name="黄盖";
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="大都督";
    g[i]->name="周瑜";
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="矜持之花";
    g[i]->name="大乔";
    g[i]->gender=false;
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="儒生雄才";
    g[i]->name="陆逊";
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="弓腰姬";
    g[i]->name="孙尚香";
    g[i]->gender=false;
    g[i]->kingdom=KINGDOM_WU;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="神医";
    g[i]->name="华佗";
    g[i]->kingdom=KINGDOM_QUN;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="武的化身";
    g[i]->name="吕布";
    g[i]->kingdom=KINGDOM_QUN;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="绝世的舞姬";
    g[i]->name="貂蝉";
    g[i]->gender=false;
    g[i]->kingdom=KINGDOM_QUN;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="野心渐增";
    g[i]->name="袁术";
    g[i]->kingdom=KINGDOM_QUN;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_BIAO;

    NEWGENERAL;g[i]->nick="飞扬跋扈";
    g[i]->name="华雄";
    g[i]->kingdom=KINGDOM_QUN;
    g[i]->maxHP=6;
    g[i]->package=PACKAGE_BIAO;

    return g;
}

CGeneral** CGeneral::createShen()
{
    CGeneral** g;
    g=new CGeneral*[MAX_SHEN];
    int i=-1;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="鬼神再临";
    g[i]->name="神关羽";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=5;
    g[i]->package=PACKAGE_FENG;
    g[i]->zhuGong=false;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="圣光之国士";
    g[i]->name="神吕蒙";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_FENG;
    g[i]->zhuGong=false;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="赤壁的火神";
    g[i]->name="神周瑜";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_HUO;
    g[i]->zhuGong=false;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="赤壁的妖术帅";
    g[i]->name="神诸葛亮";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_HUO;
    g[i]->zhuGong=false;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="神威如龙";
    g[i]->name="神赵云";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=2;
    g[i]->package=PACKAGE_SHAN;
    g[i]->zhuGong=false;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="晋国之祖";
    g[i]->name="神司马懿";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=4;
    g[i]->package=PACKAGE_SHAN;
    g[i]->zhuGong=false;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="超世之英杰";
    g[i]->name="神曹操";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=3;
    g[i]->package=PACKAGE_LING;
    g[i]->zhuGong=false;

    NEWGENERAL;
    g[i]->id=i+10000;
    g[i]->nick="修罗之道";
    g[i]->name="神吕布";
    g[i]->kingdom=KINGDOM_SHEN;
    g[i]->maxHP=5;
    g[i]->package=PACKAGE_LING;
    g[i]->zhuGong=false;

    return g;
}

CGeneral* CGeneral::find(quint16 id)
{
    if(id>=10000&&id<10000+MAX_SHEN)
    {
        return w->generalShen[id-10000];
    }
    else if(id<MAX_GENERAL)
    {
        return w->allGenerals[id];
    }
    else
        return nullptr;
}

QString CGeneral::kingdomName(quint8 k)
{
    switch (k) {
    case KINGDOM_QUN:
        return "群";
    case KINGDOM_SHEN:
        return "神";
    case KINGDOM_SHU:
        return "蜀";
    case KINGDOM_WEI:
        return "魏";
    case KINGDOM_WU:
        return "吴";
    default:
        break;
    }
    return "";
}

QString CGeneral::packageName(quint8 p)
{
    switch (p) {
    case PACKAGE_BIAO:
        return "标准";
    case PACKAGE_FENG:
        return "风";
    case PACKAGE_HUO:
        return "火";
    case PACKAGE_JIEXIANTUPO:
        return "界限突破";
    case PACKAGE_LING:
        return "林";
    case PACKAGE_OL:
        return "OL";
    case PACKAGE_SHAN:
        return "山";
    case PACKAGE_SP:
        return "SP";
    case PACKAGE_XINGSP:
        return "☆SP";
    case PACKAGE_YIJIANG:
        return "一将成名";
    case PACKAGE_YIJIANG2012:
        return "将2";
    case PACKAGE_YIJIANG2013:
        return "将3";
    default:
        break;
    }
    return "";
}

QString CGeneral::roleName(quint8 r)
{
    switch (r) {
    case ROLE_FAN:
        return "反贼";
    case ROLE_NEI:
        return "内奸";
    case ROLE_ZHONG:
        return "忠臣";
    case ROLE_ZHU:
        return "主公";
    default:
        return "未知";
    }
}
