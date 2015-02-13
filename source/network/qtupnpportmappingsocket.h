#ifndef QTUPNPPORTMAPPINGSOCKET_H
#define QTUPNPPORTMAPPINGSOCKET_H


#include <QTcpSocket>
#include <qtimer.h>
#include <qhostaddress.h>

class QtUpnpPortMapping;
struct SAddPortMapping;
struct SDeletePortMapping;

class QtUpnpPortMappingSocket : public QObject
{
    Q_OBJECT
public:
    explicit QtUpnpPortMappingSocket(QHostAddress address,quint16 port,QString host,QString infoURL,QtUpnpPortMapping *parent = 0);
    ~QtUpnpPortMappingSocket();
    void doAdd(QList<SAddPortMapping> &);
    void doDelete(QList<SDeletePortMapping> &);
    QString host;
signals:

public slots:
private:
    QTcpSocket *socket;
    QString controlURL;
    QString infoURL;
    QHostAddress address;
    quint16 port;
    QtUpnpPortMapping* parent;
    QTimer timer;
    int state;
    QByteArray remain;
    int contentLength;
    QString id;
    QString localIP;
    QList<QByteArray> sendList;
private slots:
    void handleConnected();
    void handleRead();
    void handleTimeout();
    void handleDisconnected();
};

#endif // QTUPNPPORTMAPPINGSOCKET_H
