#ifndef CSLSOCKETHANDLE_H
#define CSLSOCKETHANDLE_H

#include <QTcpSocket>
#include <QTableWidgetItem>
#include <qtimer.h>
#include <qdatetime.h>
#include <qhostaddress.h>

class CSLSocketHandle:public QObject
{
    Q_OBJECT
public:
    CSLSocketHandle(QHostAddress &address,quint16 port,QTableWidgetItem*);
    ~CSLSocketHandle();

    void getInfo();
private slots:
    void handleConnected();
    void handleRead();
    void handleDisconnected();
    void handleTimeout();
    void handleError(QAbstractSocket::SocketError);
private:
    QTcpSocket* socket;
    QDateTime lastTime;
    QHostAddress hostaddress;
    quint16 port;
    QTableWidgetItem *item;
    QTimer timer;
    void infoError(QTableWidgetItem*);
};

#endif // CSLSOCKETHANDLE_H
