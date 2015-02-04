#include "general/init.h"

void generalInit(CGeneral *g)
{
    g=new CGeneral[100];

    g[0].nick="魏武帝";
    g[0].name="曹操";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WEI;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="狼顾之鬼";
    g[0].name="司马懿";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WEI;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="独眼的罗刹";
    g[0].name="夏侯惇";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WEI;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="前将军";
    g[0].name="张辽";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WEI;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="虎痴";
    g[0].name="许禇";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WEI;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="早终的先知";
    g[0].name="郭嘉";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WEI;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="薄幸的美人";
    g[0].name="甄姬";
    g[0].gender=false;
    g[0].kingdom=KINGDOM_WEI;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="乱世的枭雄";
    g[0].name="刘备";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_SHU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="美髯公";
    g[0].name="关羽";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_SHU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="万夫不当";
    g[0].name="张飞";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_SHU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="迟暮的丞相";
    g[0].name="诸葛亮";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_SHU;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="少年将军";
    g[0].name="赵云";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_SHU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="一骑当千";
    g[0].name="马超";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_SHU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="归隐的杰女";
    g[0].name="黄月英";
    g[0].gender=false;
    g[0].kingdom=KINGDOM_SHU;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="年轻的贤君";
    g[0].name="孙权";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="锦帆游侠";
    g[0].name="甘宁";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="白衣渡江";
    g[0].name="吕蒙";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="轻身为国";
    g[0].name="黄盖";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="大都督";
    g[0].name="周瑜";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="矜持之花";
    g[0].name="大乔";
    g[0].gender=false;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="儒生雄才";
    g[0].name="陆逊";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="弓腰姬";
    g[0].name="孙尚香";
    g[0].gender=false;
    g[0].kingdom=KINGDOM_WU;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="神医";
    g[0].name="华佗";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_QUN;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="武的化身";
    g[0].name="吕布";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_QUN;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="绝世的舞姬";
    g[0].name="貂蝉";
    g[0].gender=false;
    g[0].kingdom=KINGDOM_QUN;
    g[0].maxHP=3;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="野心渐增";
    g[0].name="袁术";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_QUN;
    g[0].maxHP=4;
    g[0].package=PACKAGE_BIAO;

    g[0].nick="飞扬跋扈";
    g[0].name="华雄";
    g[0].gender=true;
    g[0].kingdom=KINGDOM_QUN;
    g[0].maxHP=6;
    g[0].package=PACKAGE_BIAO;

}
