#ifndef QTUPNPPORTMAPPING_H
#define QTUPNPPORTMAPPING_H

#include "qtupnpportmappingsocket.h"

#define UPNPPORTMAPPING_STATE_INIT 0
#define UPNPPORTMAPPING_STATE_READY 1
#define UPNPPORTMAPPING_STATE_FAILED 2

struct SAddPortMapping
{
    quint16 externalPort;
    quint16 internalPort;
    QString description;
    bool tcp;
};

struct SDeletePortMapping
{
    quint16 externalPort;
    bool tcp;
};

class QtUpnpPortMapping:public QObject
{
    Q_OBJECT
public:
    explicit QtUpnpPortMapping(QObject *parent = 0);
    ~QtUpnpPortMapping();
    void addPortMapping(quint16 externalPort,quint16 internalPort,const QString &description="",bool tcp=true);
    void deletePortMapping(quint16 externalPort,bool tcp=true);
    void rootFailed(QtUpnpPortMappingSocket*);
    void rootOK(QtUpnpPortMappingSocket*);
    void emitFinished();
signals:
    void finished();
private:
    QUdpSocket *udpSocket;
    QList<QtUpnpPortMappingSocket*> pendingRootDevices,successedRootDevices;
    int state;
    void addRoot(QString host,quint16 port,QHostAddress address,QString infoURL);
    QList<SAddPortMapping> pendingAdd;
    QList<SDeletePortMapping> pendingDelete;
    bool noNewRoot;
private slots:
    void handleUdpRead();
    void handleTimeout();
};

#endif // QTUPNPPORTMAPPING_H
