#ifndef CCLIENT_H
#define CCLIENT_H

#include "pch.h"

class CGameClient;

class CClient : public QObject
{
    Q_OBJECT
public:
    explicit CClient(const QString ip,quint16 port,quint8 authmode,const QByteArray password,const QString user,QObject *parent = 0);
    ~CClient();
    void end();
    void end2(const QString &err="");
    void send(QByteArray &);
    QTcpSocket socket;
    QUdpSocket udp;
    QTimer timer,timerUdpLive;
    QString ip;
    quint16 port;
    quint8 authmode;
    QByteArray password;
    QString user;
    int state;
    uint maxRoom;
    quint8 numberOfPlayer,operationTimeout,wuXieTimeout,extreTime,choiceTimeout;
    bool shuangNei,allowChat;
    bool alreadyEnd;

    quint16 bytesRemain;
    int udpCode;
    bool udpConfirmed;
    int udpWaitTime;

    CGameClient *game;
signals:

public slots:
    void handleTimeout();
    void handleConnected();
    void handleDisconnected();
    void handleRead();
    void handleError(QAbstractSocket::SocketError);
    void udpConfirm();
    void udpLive();
    void handleUdpRead();
private:
    bool rhConnected();
    bool rhInserver(QByteArray &);
    bool rhChooseRoom(QByteArray &);
};

#endif // CCLIENT_H
