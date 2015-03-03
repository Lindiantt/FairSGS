#ifndef CCLIENT_H
#define CCLIENT_H

#include "pch.h"

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

    quint16 bytesRemain;
    int udpCode;
    bool udpConfirmed;
signals:

public slots:
    void handleTimeout();
    void handleConnected();
    void handleDisconnected();
    void handleRead();
    void handleError(QAbstractSocket::SocketError);
    void udpConfirm();
    void udpLive();
private:
    bool rhConnected();
    bool rhInserver();
    bool rhChooseRoom();
};

#endif // CCLIENT_H
