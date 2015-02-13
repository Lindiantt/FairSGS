#include "qtupnpportmapping.h"

QtUpnpPortMapping::QtUpnpPortMapping(QObject *parent) : QObject(parent)
{
    noNewRoot=false;
    state=UPNPPORTMAPPING_STATE_INIT;
    udpSocket=new QUdpSocket(this);
    udpSocket->bind();
    QString s;
    s="M-SEARCH * HTTP/1.1\r\n"
            "Host: 239.255.255.250:1900\r\n"
            "Man: \"ssdp:discover\"\r\n"
            "Mx: 5\r\n"
            "ST: upnp:rootdevice\r\n\r\n";
    connect(udpSocket,udpSocket->readyRead,this,this->handleUdpRead);
    QByteArray ba;
    ba=s.toUtf8();
    QHostAddress addr("239.255.255.250");
    udpSocket->writeDatagram(ba,addr,1900);
    QTimer::singleShot(6000,this,QtUpnpPortMapping::handleTimeout);
}

QtUpnpPortMapping::~QtUpnpPortMapping()
{
    qDeleteAll(pendingRootDevices);
    qDeleteAll(successedRootDevices);
}

void QtUpnpPortMapping::handleTimeout()
{
    udpSocket->deleteLater();
    udpSocket=NULL;
    if(pendingRootDevices.isEmpty())
    {
        noNewRoot=true;
        pendingAdd.clear();
        pendingDelete.clear();
        if(successedRootDevices.isEmpty())
        {
            state=UPNPPORTMAPPING_STATE_FAILED;
            emit finished();
        }
    }
}

void QtUpnpPortMapping::addPortMapping(quint16 externalPort, quint16 internalPort, const QString &description, bool tcp)
{
    if(state==UPNPPORTMAPPING_STATE_FAILED)
    {
        emit finished();
        return;
    }
    SAddPortMapping sa;
    sa.description=description;
    sa.externalPort=externalPort;
    sa.internalPort=internalPort;
    sa.tcp=tcp;
    if(!noNewRoot) pendingAdd.append(sa);
    if(!successedRootDevices.isEmpty())
    {
        QList<SAddPortMapping> list;
        list.append(sa);
        foreach (QtUpnpPortMappingSocket* handle, successedRootDevices) {
            handle->doAdd(list);
        }
    }
}

void QtUpnpPortMapping::deletePortMapping(quint16 externalPort, bool tcp)
{
    if(state==UPNPPORTMAPPING_STATE_FAILED)
    {
        emit finished();
        return;
    }
    SDeletePortMapping sd;
    sd.externalPort=externalPort;
    sd.tcp=tcp;
    if(!noNewRoot) pendingDelete.append(sd);
    if(!successedRootDevices.isEmpty())
    {
        QList<SDeletePortMapping> list;
        list.append(sd);
        foreach (QtUpnpPortMappingSocket* handle, successedRootDevices) {
            handle->doDelete(list);
        }
    }
}

void QtUpnpPortMapping::handleUdpRead()
{
    QByteArray ba;
    while(udpSocket->hasPendingDatagrams())
    {
        ba.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 port;
        udpSocket->readDatagram(ba.data(),ba.size(),&sender,&port);
        if(!ba.startsWith("HTTP/1.1 200 OK\r\n")) continue;
        int i=ba.indexOf("\r\nLOCATION: ");
        if(i==-1) continue;
        i+=12;
        int j=ba.indexOf("\r\n",i);
        if(j==-1) continue;
        QByteArray location;
        location=ba.mid(i,j-i);
        if(location.length()<10||!location.startsWith("http://")) continue;
        location=location.mid(7);
        j=location.indexOf('/');
        if(j==-1) continue;
        i=location.indexOf(':');
        QString host;
        host=host.fromUtf8(location.mid(0,j));
        if(i==-1)
        {
            port=80;
        }
        else
        {
            bool ok;
            int k=location.mid(i+1,j-i-1).toInt(&ok);
            if(!ok||k<0||k>65535) continue;
            port=k;
        }
        QString info;
        info=location.mid(j);
        addRoot(host,port,sender,info);
    }
}

void QtUpnpPortMapping::addRoot(QString host, quint16 port, QHostAddress address, QString infoURL)
{
    foreach (QtUpnpPortMappingSocket* handle, pendingRootDevices) {
        if(handle->host==host)
            return;
    }
    foreach (QtUpnpPortMappingSocket* handle, successedRootDevices) {
        if(handle->host==host)
            return;
    }
    QtUpnpPortMappingSocket *newHandle=new QtUpnpPortMappingSocket(address,port,host,infoURL,this);
    pendingRootDevices.append(newHandle);
}

void QtUpnpPortMapping::rootFailed(QtUpnpPortMappingSocket *handle)
{
    pendingRootDevices.removeOne(handle);
    handle->deleteLater();
    if(!udpSocket&&state==UPNPPORTMAPPING_STATE_INIT&&pendingRootDevices.isEmpty())
    {
        state=UPNPPORTMAPPING_STATE_FAILED;
        emit finished();
    }
}

void QtUpnpPortMapping::rootOK(QtUpnpPortMappingSocket *handle)
{
    pendingRootDevices.removeOne(handle);
    state=UPNPPORTMAPPING_STATE_READY;
    successedRootDevices.append(handle);
    if(!pendingAdd.isEmpty())
    {
        handle->doAdd(pendingAdd);
    }
    if(!pendingDelete.isEmpty())
    {
        handle->doDelete(pendingDelete);
    }
}

void QtUpnpPortMapping::emitFinished()
{
    emit finished();
}
