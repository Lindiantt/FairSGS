#include "card/ccardtype.h"

QString CCardWuqi::distanceChinese()
{
    switch (distance) {
    case 1:
        return "一";
    case 2:
        return "二";
    case 3:
        return "三";
    case 4:
        return "四";
    case 5:
        return "五";
    default:
        break;
    }
    return "";
}
