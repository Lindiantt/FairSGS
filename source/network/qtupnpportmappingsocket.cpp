#include "qtupnpportmapping.h"

QtUpnpPortMappingSocket::QtUpnpPortMappingSocket(QHostAddress address, quint16 port, QString host, QString infoURL, QtUpnpPortMapping *parent) : QObject(parent)
{
    this->address=address;
    this->port=port;
    this->host=host;
    this->parent=parent;
    this->infoURL=infoURL;
    socket=new QTcpSocket();
    connect(socket,socket->connected,this,this->handleConnected);
    connect(socket,socket->readyRead,this,this->handleRead);
    connect(socket,socket->disconnected,this,this->handleDisconnected);
    socket->connectToHost(address,port);
    connect(&timer,timer.timeout,this,this->handleTimeout);
    timer.setSingleShot(true);
    timer.start(5000);
    state=0;
    contentLength=0;
}

QtUpnpPortMappingSocket::~QtUpnpPortMappingSocket()
{
    socket->deleteLater();
}

void QtUpnpPortMappingSocket::handleDisconnected()
{
    if(!sendList.isEmpty())
    {
        socket->connectToHost(address,port);
    }
}

void QtUpnpPortMappingSocket::handleConnected()
{
    QString s;
    QByteArray ba;
    if(state==0)
    {
        timer.stop();
        s="GET "+infoURL+" HTTP/1.1\r\n"
                                 "HOST: "+host+"\r\n"
                                               "ACCEPT-LANGUAGE: en\r\n\r\n";
        ba=s.toUtf8();
        socket->write(ba);
        timer.start(5000);
        state=1;
        localIP=socket->localAddress().toString();
    }
    else
    {
        if(!sendList.isEmpty())
        {
            ba=sendList.takeFirst();
            socket->write(ba);
        }
    }
}

void QtUpnpPortMappingSocket::handleRead()
{
    int i;
    QString s;
    QByteArray ba;
    switch (state) {
    case 1:
        if(socket->bytesAvailable()<17) return;
        ba.resize(17);
        ba=socket->read(17);
        if(!ba.startsWith("HTTP/1.1 200 OK\r\n"))
        {
            parent->rootFailed(this);
            return;
        }
        state=2;
    case 2:
    case 3:
        ba.resize(1024);
aa:
        i=socket->readLine(ba.data(),1024);
        if(i<=0) return;
        remain.append(ba.left(i));
        if(remain[remain.length()-1]!='\n')//没读完整行，等下一次SIGNAL再继续读
        {
            return;
        }
        if(state==3)
        {
            if(remain.length()==2&&remain.startsWith("\r\n"))//HTTP头部分结束了
            {
                state=4;
                remain.clear();
                goto bb;
            }
            else
            {
                remain.clear();
                goto aa;
            }
        }
        if(remain.length()<=2)//行太短
        {
            parent->rootFailed(this);
            return;
        }
        if(remain.length()>18&&remain.startsWith("CONTENT-LENGTH: "))
        {
            s=s.fromLocal8Bit(remain.mid(16,remain.length()-2));
            bool ok;
            contentLength=s.toInt(&ok);
            if(!contentLength)
            {
                parent->rootFailed(this);
                return;
            }
            state=3;
        }
        remain.clear();
        goto aa;
    case 4:
bb:
        if(socket->bytesAvailable()<contentLength) return;
        ba=socket->read(contentLength);
    {
        QXmlStreamReader reader;
        reader.addData(ba);
        int sta=0;
        while(!reader.atEnd())
        {
            i=reader.readNext();
            switch(sta)
            {
            case 0:
                if(i==QXmlStreamReader::StartElement&&reader.name()=="service")
                {
                    sta=1;
                }
                break;
            case 1:
                if(i==QXmlStreamReader::StartElement&&reader.name()=="serviceType")
                {
                    s=reader.readElementText();
                    if(s.contains(":WANIPConnection:"))
                    {
                        id=s;
                        sta=2;
                    }
                }
                if(i==QXmlStreamReader::EndElement&&reader.name()=="service")
                {
                    sta=0;
                }
                break;
            case 2:
                if(i==QXmlStreamReader::StartElement&&reader.name()=="controlURL")
                {
                    controlURL=reader.readElementText();
                    parent->rootOK(this);
                    timer.stop();
                    state=5;
                    return;
                }
                if(i==QXmlStreamReader::EndElement&&reader.name()=="service")
                {
                    parent->rootFailed(this);
                    return;
                }
                break;
            }
        }
    }
        break;
    default:
        socket->close();
        parent->emitFinished();
        if(sendList.size())
            socket->connectToHost(address,port);
        break;
    }
}

void QtUpnpPortMappingSocket::handleTimeout()
{
    parent->rootFailed(this);
}

void QtUpnpPortMappingSocket::doAdd(QList<SAddPortMapping> &list)
{
    QString s;
    QString header;
    QByteArray ba;
    int l;
    foreach (SAddPortMapping sa, list) {
        s="<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                      "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>";
        s+="<u:AddPortMapping xmlns:u=\""+id+"\"><NewRemoteHost></NewRemoteHost><NewExternalPort>"
                +s.number(sa.externalPort)+"</NewExternalPort><NewProtocol>"
                +(sa.tcp?"TCP":"UDP")+"</NewProtocol><NewInternalPort>"
                +s.number(sa.internalPort)+"</NewInternalPort><NewInternalClient>"
                +localIP+"</NewInternalClient><NewEnabled>1</NewEnabled><NewPortMappingDescription>"
                +sa.description+"</NewPortMappingDescription><NewLeaseDuration>0</NewLeaseDuration></u:AddPortMapping>";
        s+="</s:Body></s:Envelope>";
        ba=s.toUtf8();
        l=ba.length();
        header="POST "+controlURL+" HTTP/1.1\r\n"
                                  "HOST: "+host+"\r\n"
                                  "CONTENT-LENGTH: "+s.number(l)+"\r\n"
                                  "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
                                  "SOAPACTION: \""+id+"#AddPortMapping\"\r\n\r\n";
        ba.prepend(header.toUtf8());
        sendList.append(ba);
    }
    if(!socket->isOpen())
        socket->connectToHost(address,port);
}

void QtUpnpPortMappingSocket::doDelete(QList<SDeletePortMapping> &list)
{
    QString s;
    QByteArray ba;
    int l;
    QString header;
    foreach (SDeletePortMapping sa, list) {
        s="<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                      "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>";
        s+="<u:DeletePortMapping xmlns:u=\""+id+"\"><NewRemoteHost></NewRemoteHost><NewExternalPort>"
                +s.number(sa.externalPort)+"</NewExternalPort><NewProtocol>"
                +(sa.tcp?"TCP":"UDP")+"</NewProtocol></u:DeletePortMapping>";
        s+="</s:Body></s:Envelope>";
        ba=s.toUtf8();
        l=ba.length();
        header="POST "+controlURL+" HTTP/1.1\r\n"
                                  "HOST: "+host+"\r\n"
                                  "CONTENT-LENGTH: "+s.number(l)+"\r\n"
                                  "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
                                  "SOAPACTION: \""+id+"#DeletePortMapping\"\r\n\r\n";
        ba.prepend(header.toUtf8());
        sendList.append(ba);
    }
    if(!socket->isOpen())
        socket->connectToHost(address,port);
}
