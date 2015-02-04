#ifndef CSERVER_H
#define CSERVER_H

#include "game/cgame.h"
#include <qtcpserver.h>
#include "network/croom.h"

class CServer
{
public:
    CServer();
    ~CServer();
    QVector<CRoom*> rooms;
    QString serverName;
    quint8 numberOfPlayer,choices,choicesZG,choicesNJ;
    uint maxRoom;
    bool shuangNei;
    bool cardEX,cardJunZheng,cardJieXianTuPo;
    quint32 generalPackage;
    quint8 generalShen[8];
    bool generalBan[100][4];
    QList<CGeneral*> generalList;
    quint8 validate;
    QString password;
    quint8 operationTimeout,wuXieTimeout,extreTime,choiceTimeout;
    uint maxUnlooker;
    bool allowChat,banSameIP;
};

#endif // CSERVER_H
