#ifndef CSERVER_H
#define CSERVER_H

#include "game/cgame.h"
#include <qtcpserver.h>
#include "network/croom.h"
#include <qnetworkreply.h>
#include <qtimer.h>
#include "network/qtupnpportmapping.h"
#include "network/cplayersocket.h"
#include <QLinkedList>
#include <QtSql>


class CServer:public QObject
{
    Q_OBJECT
public:
    CServer();
    ~CServer();
    void regServer();
    void removeSocket(CPlayerSocket*);

    QList<CRoom*> rooms;
    QString serverName;
    quint8 numberOfPlayer,choices,choicesZG,choicesNJ;
    uint maxRoom;
    bool shuangNei;
    bool cardEX,cardJunZheng,cardJieXianTuPo;
    bool generalPackage[12];
    quint8 generalShen[8];
    bool generalBan[100][2];
    quint8 auth;
    char password[20];
    quint8 operationTimeout,wuXieTimeout,extreTime,choiceTimeout;
    uint maxOnlooker;
    bool allowChat,banSameIP;
    quint16 port;

    bool regSuccessed;
    QLinkedList<CPlayerSocket*> sockets;
    QByteArray getInfoBuf,versionError,authmodeError;
    uint numberOfSockets;
    QSqlQuery querySelect,queryInsert;
private slots:
    void handleNewConnection();
    void handleUpnpFinished();
    void handleReplyFinished();
private:
    QTcpServer *server;
    QNetworkReply *reply;
    QtUpnpPortMapping *upnp;
    QByteArray regUrl;
    QTimer timer;

    void firstReg();
};

#endif // CSERVER_H
