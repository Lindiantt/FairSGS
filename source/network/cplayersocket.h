#ifndef CPLAYERSOCKET_H
#define CPLAYERSOCKET_H

#include "pch.h"


class CServer;
class CPlayerServer;
class CRoom;

class CPlayerSocket : public QObject
{
    Q_OBJECT
public:
    explicit CPlayerSocket(QTcpSocket* socket,QObject *parent = 0);
    ~CPlayerSocket();
    void send(QByteArray &);
    void setUdpPort(quint16 port);
    QByteArray info();
    QTcpSocket* socket;
    int state;
    QTimer timer;
    QString nick;
    QHostAddress hostAddress;
    CPlayerServer *player;
    CRoom *currentRoom;
    quint16 bytesRemain;
    quint16 udpPort;
    quint16 favorite;
    bool udpAvailable;
    int udpCode;
    bool admin,roomHost,ready;
    quint8 roomPosition;
signals:

public slots:
private:
    CServer* server;

    bool rhInit();
    bool rhVerified();
    bool rhConnected();
    bool rhChooseRoom();
    void joinRoom(CRoom*);
    void onlookRoom(CRoom*);
    void chooseRoom();
    QByteArray roomPage(uint page);
private slots:
    void handleRead();
    void handleDisconnected();
    void handleTimeout();
    void handleWrite();
};

#endif // CPLAYERSOCKET_H
