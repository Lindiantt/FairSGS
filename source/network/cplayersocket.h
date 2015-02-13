#ifndef CPLAYERSOCKET_H
#define CPLAYERSOCKET_H

#include <QTcpSocket>
#include "network/define.h"
#include <qtimer.h>

class CServer;

class CPlayerSocket : public QObject
{
    Q_OBJECT
public:
    explicit CPlayerSocket(QTcpSocket* socket,QObject *parent = 0);
    ~CPlayerSocket();
    QTcpSocket* socket;
    int state;
    QTimer timer;
    char machineCode[20];

signals:

public slots:
private:
    CServer* server;
    uint waitFor;
    bool rhInit();
    bool rhVerified();
    bool rhConnected();
private slots:
    void handleRead();
    void handleDisconnected();
    void handleTimeout();
    void handleWrite();
};

#endif // CPLAYERSOCKET_H
