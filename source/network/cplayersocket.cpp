#include "network/cserver.h"
#include <QtEndian>

#define COMPARE(a,b) if((a)!=(b)){this->deleteLater();return false;}

CPlayerSocket::CPlayerSocket(QTcpSocket *socket, QObject *parent) : QObject(parent)
{
    this->socket=socket;
    this->server=(CServer*)parent;
    waitFor=0;
    state=SOCKETSTATE_INIT;
    connect(socket,socket->readyRead,this,this->handleRead);
    connect(socket,socket->disconnected,this,this->handleDisconnected);
    connect(socket,socket->bytesWritten,this,this->handleWrite);
    timer.setSingleShot(true);
    connect(&timer,timer.timeout,this,this->handleTimeout);
    timer.start(30000);
}

CPlayerSocket::~CPlayerSocket()
{
    socket->deleteLater();
    if(state<=3) server->removeSocket(this);
}

void CPlayerSocket::handleRead()
{
aa:
    switch (state) {
    case SOCKETSTATE_INIT:
        if(!rhInit()) return;
        break;
    case SOCKETSTATE_VARIFIED:
        if(!rhVerified()) return;
        break;
    case SOCKETSTATE_CONNECTED:
        if(!rhConnected()) return;
        break;
    default:
        return;
    }
    if(socket->bytesAvailable()) goto aa;
}

void CPlayerSocket::handleWrite()
{
    if(state==SOCKETSTATE_GOINGTODIE)
    {
        this->deleteLater();
    }
}

void CPlayerSocket::handleDisconnected()
{
    this->deleteLater();
}

void CPlayerSocket::handleTimeout()
{
    this->deleteLater();
}

bool CPlayerSocket::rhInit()
{
    if(socket->bytesAvailable()<4)
    {
        this->deleteLater();
        return false;
    }
    quint16 v;
    socket->read((char*)&v,2);
    v=qFromLittleEndian(v);
    if(v!=VALIDATIONNUMBER)
    {
        this->deleteLater();
        return false;
    }
    socket->read((char*)&v,2);
    v=qFromLittleEndian(v);
    if(v!=VERSION_COMMUNICATION)
    {
        socket->write(server->versionError);
        state=SOCKETSTATE_GOINGTODIE;
        return false;
    }
    state=SOCKETSTATE_VARIFIED;
    return true;
}

bool CPlayerSocket::rhVerified()
{
    char buf[20];
    int i;
    i=socket->read(buf,1);
    COMPARE(i,1);
    QByteArray ba,sendbuf;
    sendbuf.resize(1);
    sendbuf[0]=NETWORK_OK;
    switch (buf[0]) {
    case ACTION_GETINFO:
    {
        quint8 n;
        if(server->numberOfSockets>=127)
            n=127;
        else
            n=server->numberOfSockets;
        server->getInfoBuf[1]=(server->getInfoBuf[1]&0b10000000)|n;
        socket->write(sendbuf+server->getInfoBuf);
        state=SOCKETSTATE_GOINGTODIE;
    }
        break;
    case ACTION_CONNECT:
        i=socket->read(buf,1);
        COMPARE(i,1);
        if(!(server->auth==buf[0]||(server->auth==3&&buf[0]==2)))
        {
            socket->write(sendbuf+server->authmodeError);
            state=SOCKETSTATE_GOINGTODIE;
            return false;
        }
        if(buf[0]==0)
        {
            i=socket->read(machineCode,20);
            COMPARE(i,20);
            state=SOCKETSTATE_CONNECTED;
        }
        else if(buf[0]==1)
        {
            i=socket->read(buf,20);
            COMPARE(i,20);
            if(memcmp(server->password,buf,20))
            {
                buf[0]=CONNECT_ERROR_PASSWORD;
                socket->write(buf,1);
                state=SOCKETSTATE_GOINGTODIE;
                return false;
            }
            i=socket->read(machineCode,20);
            COMPARE(i,20);
            state=SOCKETSTATE_CONNECTED;
        }
        else
        {
            i=socket->read(buf,1);
            COMPARE(i,1);
            ba.resize(buf[0]);
            i=socket->read(ba.data(),buf[0]);
            COMPARE(i,buf[0]);
            QByteArray pwd;
            pwd.resize(20);
            i=socket->read(pwd.data(),20);
            COMPARE(i,20);
            server->querySelect.bindValue(0,ba);
            server->querySelect.bindValue(1,pwd);
            server->querySelect.exec();
            if(server->querySelect.next()&&server->querySelect.value(0).toInt()==1)
            {
                sendbuf+=(char)CONNECT_OK;
            }
            else
            {
                sendbuf+=CONNECT_ERROR_PASSWORD;
                socket->write(sendbuf);
                state=SOCKETSTATE_GOINGTODIE;
                return false;
            }
            state=SOCKETSTATE_CONNECTED;
        }
        break;
    case ACTION_REGISTER:
    {
        if(server->auth!=2)
        {
            sendbuf+=REGISTER_ERROR_NOTALLOW;
            socket->write(sendbuf);
        }
        i=socket->read(buf,1);
        COMPARE(i,1);
        if(buf[0]>21||buf[0]<1)
        {
            this->deleteLater();
            return false;
        }
        ba.resize(buf[0]);
        i=socket->read(ba.data(),buf[0]);
        COMPARE(i,buf[0]);
        QByteArray pwd;
        pwd=socket->read(20);
        COMPARE(pwd.length(),20);
        server->queryInsert.bindValue(0,ba);
        server->queryInsert.bindValue(1,pwd);
        if(server->queryInsert.exec())
        {
            sendbuf+=(char)REGISTER_OK;
        }
        else
        {
            sendbuf+=REGISTER_ERROR_DUPLICATION;
            socket->write(sendbuf);
            state=SOCKETSTATE_GOINGTODIE;
            return false;
        }
    }
        break;
    default:
        this->deleteLater();
        return false;
    }
    socket->write(sendbuf);
    return true;
}

bool CPlayerSocket::rhConnected()
{
    return true;
}
