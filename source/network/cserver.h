#ifndef CSERVER_H
#define CSERVER_H

#include "pch.h"
#include "network/define.h"

class CPlayerSocket;
class CCard;
class CGeneral;
class QtUpnpPortMapping;
class CRoom;

class CServer:public QObject
{
    Q_OBJECT
public:
    CServer();
    ~CServer();
    void regServer();
    void removeSocket(CPlayerSocket*);
    void log(const QString &);
    CRoom* createRoom();
    uint lastRoomPage();

    QHash<uint,CRoom*> rooms;
    uint lastRoomID;

    QString serverName;
    char numberOfPlayer;
    quint8 choices,choicesZG,choicesNJ;
    int maxRoom;
    bool shuangNei;
    quint8 numberOfNei;
    bool cardEX,cardJunZheng,cardJieXianTuPo;
    bool generalPackage[12];
    quint8 generalShen[8];
    bool generalBan[MAX_GENERAL];
    quint8 auth;
    char password[20];
    quint8 operationTimeout,wuXieTimeout,extreTime,choiceTimeout;
    int maxOnlooker;
    bool allowChat,banSameIP;
    quint16 port;

    bool regSuccessed;
    QList<CPlayerSocket*> sockets;
    QByteArray getInfoBuf,versionError,authmodeError,importantInfo;
    uint numberOfSockets;
    QSqlQuery querySelect,queryInsert;

    QList<CCard*> availableCards;
    QList<CGeneral*> availableZhugong;
    QHash<int,QList<CGeneral*>> availableGenerals;
    QList<quint8> roles;

    int logCount;
    QMultiHash<int,CPlayerSocket*> udpVals;
private slots:
    void handleNewConnection();
    void handleUpnpFinished();
    void handleReplyFinished();
    void handleUdpRead();
private:
    QTcpServer *server;
    QUdpSocket udp;
    QNetworkReply *reply;
    QtUpnpPortMapping *upnp;
    QByteArray regUrl;
    QTimer timer;

    void firstReg();
};

#endif // CSERVER_H
