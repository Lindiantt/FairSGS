#include "network/cserver.h"
#include "network/cplayersocket.h"
#include "game/cplayerserver.h"
#include "croom.h"

#define COMPARE(a,b) if((a)!=(b)){this->deleteLater();return false;}

CPlayerSocket::CPlayerSocket(QTcpSocket *socket, QObject *parent) : QObject(parent)
{
    this->socket=socket;
    this->server=(CServer*)parent;
    currentRoom=nullptr;
    bytesRemain=0;
    state=SOCKETSTATE_INIT;
    connect(socket,socket->readyRead,this,this->handleRead);
    connect(socket,socket->disconnected,this,this->handleDisconnected);
    connect(socket,socket->bytesWritten,this,this->handleWrite);
    /*timer.setSingleShot(true);
    connect(&timer,timer.timeout,this,this->handleTimeout);
    timer.start(30000);*/
}

CPlayerSocket::~CPlayerSocket()
{
    socket->deleteLater();
    if(state>=SOCKETSTATE_CONNECTED) server->removeSocket(this);
    server->log(socket->peerAddress().toString()+"断开连接");
}

void CPlayerSocket::handleRead()
{
aa:
    switch (state) {
    case SOCKETSTATE_INIT:
        if(!rhInit()) return;
        qDebug("init");
        break;
    case SOCKETSTATE_VARIFIED:
        if(!rhVerified()) return;
        qDebug("varified");
        break;
    case SOCKETSTATE_CONNECTED:
        if(!rhConnected()) return;
        qDebug("connected");
        break;
    case SOCKETSTATE_PLAYING:
        //player;
        break;
    default:
    {
        QByteArray ba;
        if(!bytesRemain)
        {
            if(socket->bytesAvailable()>=2)
            {
                ba=socket->read(2);
                memcpy(&bytesRemain,ba.data(),2);
                bytesRemain=qFromLittleEndian(bytesRemain);
                if(!bytesRemain)
                {
                    this->deleteLater();
                    return;
                }
            }
            else
                return;
        }
        if(socket->bytesAvailable()<bytesRemain)
            return;
        switch (state) {
        case SOCKETSTATE_CHOOSEROOM:
            if(!rhChooseRoom()) return;
            qDebug("choose room");
            break;
        default:
            return;
        }
    }
        break;
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
    if(state==SOCKETSTATE_PLAYING)
    {

    }
    else
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
        server->log(socket->peerAddress().toString()+"获取简略信息");
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
            i=socket->read(buf,1);
            COMPARE(i,1);
            if(buf[0]>30)
            {
                this->deleteLater();
                return false;
            }
            ba=socket->read(buf[0]);
            COMPARE(ba.length(),buf[0]);
        }
        else if(buf[0]==1)
        {
            i=socket->read(buf,20);
            COMPARE(i,20);
            if(memcmp(server->password,buf,20))
            {
                sendbuf+=CONNECT_ERROR_PASSWORD;
                socket->write(sendbuf);
                state=SOCKETSTATE_GOINGTODIE;
                return false;
            }
            i=socket->read(buf,1);
            COMPARE(i,1);
            if(buf[0]>30)
            {
                this->deleteLater();
                return false;
            }
            ba=socket->read(buf[0]);
            COMPARE(ba.length(),buf[0]);
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
            if(!server->querySelect.next()||server->querySelect.value(0).toInt()!=1)
            {
                sendbuf+=CONNECT_ERROR_PASSWORD;
                socket->write(sendbuf);
                state=SOCKETSTATE_GOINGTODIE;
                return false;
            }
        }
        nick=QString::fromUtf8(ba);
        ba=socket->read(2);
        COMPARE(ba.length(),2);
        memcpy(&favorite,ba.data(),2);
        favorite=qFromLittleEndian(favorite);
        state=SOCKETSTATE_CONNECTED;
        sendbuf+=(char)CONNECT_OK;
        sendbuf+=server->importantInfo;
        udpCode=qrand();
        sendbuf.append((char*)&udpCode,4);
        server->numberOfSockets++;
        server->sockets.append(this);
        this->hostAddress=socket->peerAddress();
        socket->write(sendbuf);
        return rhConnected();
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
    bool reconnect=false;
    if(server->auth<2)//检测是否断线重连
    {
        foreach (CPlayerSocket *handle,server->sockets) {
            if(handle!=this&&handle->nick==this->nick&&handle->hostAddress==this->hostAddress)
            {
                if(handle->state==SOCKETSTATE_PLAYING)
                    reconnect=true;
                handle->socket->disconnectFromHost();
                break;
            }
        }
    }
    else
    {
        foreach (CPlayerSocket *handle, server->sockets) {
            if(handle!=this&&handle->nick==this->nick)
            {
                if(handle->state==SOCKETSTATE_PLAYING)
                    reconnect=true;
                handle->socket->disconnectFromHost();
                break;
            }
        }
    }
    if(reconnect)
    {
        //断线重连
        return true;
    }
    if(server->maxRoom==1)
    {
        if(server->rooms.isEmpty())
            server->createRoom();
        auto it=server->rooms.begin();
        CRoom* room=it.value();
        joinRoom(room);
    }
    else
    {
        chooseRoom();
    }
    return true;
}

void CPlayerSocket::joinRoom(CRoom *room)
{
    QByteArray sendbuf;
    sendbuf.resize(3);
    if(room->players.size()==server->numberOfPlayer)
    {
        if(room->onlooker.size()>=server->maxOnlooker)
        {
            sendbuf[2]=ROOM_JOIN_ERROR_FULL;
            send(sendbuf);
            if(server->maxRoom==1)
            {
                state=SOCKETSTATE_GOINGTODIE;
                this->deleteLater();
                return;
            }
        }
        else
        {
            if(server->maxRoom==1)
            {
                sendbuf[2]=ROOM_ONLOOK_OK;
                send(sendbuf);
                onlookRoom(room);
            }
            else
            {
                sendbuf[2]=ROOM_JOIN_ERROR_CANONLOOK;
                send(sendbuf);
            }
        }
    }
    else
    {
        sendbuf[2]=ROOM_JOIN_OK;
        if(server->maxRoom==1)
        {
            sendbuf.resize(7);
            uint u=qToLittleEndian(room->id);
            memcpy(sendbuf.data()+3,&u,4);
        }
        send(sendbuf);
        room->join(this);
    }
}

void CPlayerSocket::chooseRoom()
{
    state=SOCKETSTATE_CHOOSEROOM;
    QByteArray sendbuf,ba;
    sendbuf.resize(3);
    sendbuf[2]=SERVER_CHOOSEROOM;
    ba=roomPage(1);
    sendbuf.append(ba);
    send(sendbuf);
}

void CPlayerSocket::send(QByteArray &sendbuf)
{
    quint16 len=sendbuf.length()-2;
    len=qToLittleEndian(len);
    memcpy(sendbuf.data(),&len,2);
    socket->write(sendbuf);
}

void CPlayerSocket::onlookRoom(CRoom *)
{

}

bool CPlayerSocket::rhChooseRoom()
{
    QByteArray ba,sendbuf;
    ba=socket->read(bytesRemain);
    uint u;
    switch (ba[0]) {
    case ROOM_JOIN:
    {
        COMPARE(ba.length(),5);
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        auto room=server->rooms.find(u);
        if(room==server->rooms.end())
        {
            sendbuf.resize(3);
            sendbuf[2]=ROOM_JOIN_ERROR_INVALID;
            send(sendbuf);
            return true;
        }
        joinRoom(room.value());
    }
        break;
    case ROOM_CREATE:
    {
        COMPARE(ba.length(),1);
        CRoom* room;
        room=server->createRoom();
        if(!room)
        {
            sendbuf.resize(3);
            sendbuf[2]=ROOM_CREATE_ERROR_FULL;
            send(sendbuf);
            return true;
        }
        sendbuf.resize(7);
        u=qToLittleEndian(room->id);
        memcpy(sendbuf.data()+3,&u,4);
        sendbuf[2]=ROOM_CREATE_OK;
        send(sendbuf);
        joinRoom(room);
    }
        break;
    case ROOM_ONLOOK:
    {
        COMPARE(ba.length(),5);
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        auto room=server->rooms.find(u);
        if(room==server->rooms.end())
        {
            sendbuf.resize(3);
            sendbuf[2]=ROOM_ONLOOK_ERROR_INVALID;
            send(sendbuf);
            return true;
        }
        onlookRoom(room.value());
    }
        break;
    case ROOM_FIND:
    {
        COMPARE(ba.length(),5);
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        auto room=server->rooms.find(u);
        if(room==server->rooms.end())
        {
            sendbuf.resize(3);
            sendbuf[2]=ROOM_FIND_ERROR;
            send(sendbuf);
            return true;
        }
        sendbuf.resize(5);
        sendbuf[2]=ROOM_FIND_OK;
        CRoom *r=room.value();
        sendbuf[3]=r->players.size();
        sendbuf[4]=r->game?1:0;
        send(sendbuf);
    }
        break;
    case ROOM_REFRESH:
        COMPARE(ba.length(),5);
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        ba=roomPage(u);
        if(ba.isEmpty())
        {
            u=server->lastRoomPage();
            ba=roomPage(u);
        }
        sendbuf.resize(7);
        sendbuf[2]=ROOM_REFRESH_OK;
        u=qToLittleEndian(u);
        memcpy(sendbuf.data()+3,&u,4);
        sendbuf.append(ba);
        send(sendbuf);
        break;
    case ROOM_PAGE_FIRST:
        u=1;
        COMPARE(ba.length(),1);
        goto aa;
    case ROOM_PAGE_PREV:
        COMPARE(ba.length(),5);
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        u--;
        if(u<1) COMPARE(1,0);
        goto aa;
    case ROOM_PAGE_NEXT:
        COMPARE(ba.length(),5);
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        u++;
        goto aa;
    case ROOM_PAGE_LAST:
        u=server->lastRoomPage();
aa:
        ba=roomPage(u);
        if(ba.isEmpty())
        {
            sendbuf.resize(3);
            sendbuf[2]=ROOM_PAGE_ERROR;
            send(sendbuf);
            return true;
        }
        sendbuf.resize(7);
        sendbuf[2]=ROOM_PAGE_OK;
        u=qToLittleEndian(u);
        memcpy(sendbuf.data()+3,&u,4);
        sendbuf.append(ba);
        send(sendbuf);
        break;
    default:
        break;
    }
    return true;
}

QByteArray CPlayerSocket::roomPage(uint page)
{
    QByteArray ba;
    uint crn=server->rooms.size();
    uint p=page*20;
    if(crn<=p-20)
    {
        if(!crn&&page==1)
        {
            ba.resize(1);
            ba[0]=0;
        }
        return ba;
    }
    quint8 x;
    CRoom *room;
    uint u;
    if(p<=crn)
        x=20;
    else
        x=20-(p-crn);
    ba.resize(6*x+1);
    ba[0]=x;
    if(crn>p&&p-20<crn-p)//从前面开始
    {
        auto rm=server->rooms.begin();
        for(uint i=0;i<p-20;i++)
        {
            rm++;
        }
        for(int i=0;i<x;i++)
        {
            room=rm.value();
            u=qToLittleEndian(rm.key());
            memcpy(ba.data()+6*i+1,&u,4);
            ba[6*i+5]=room->players.size();
            ba[6*i+6]=room->game?1:0;
            rm++;
        }
    }
    else//从后面开始
    {
        auto rm=server->rooms.end();
        for(uint i=crn;i>p;i--)
        {
            rm--;
        }
        for(;x>0;x--)
        {
            rm--;
            room=rm.value();
            u=qToLittleEndian(rm.key());
            memcpy(ba.data()+6*x-5,&u,4);
            ba[6*x-1]=room->players.size();
            ba[6*x]=room->game?1:0;
        }
    }
    return ba;
}

void CPlayerSocket::setUdpPort(quint16 port)
{
    this->udpAvailable=true;
    this->udpPort=port;
    char buf[2];
    memset(buf,0,2);
    socket->write(buf,2);
}

QByteArray CPlayerSocket::info()
{
    QByteArray ba;
    ba.resize(10);
    uint u;
    quint16 s;
    u=qToLittleEndian(this->hostAddress.toIPv4Address());
    memcpy(ba.data(),&u,4);
    s=qToLittleEndian(favorite);
    memcpy(ba.data()+4,&s,2);
    ba[6]=roomPosition;
    quint8 c=0;
    if(this->admin) c|=ROOMPLAYER_ADMIN;
    if(this->roomHost) c|=ROOMPLAYER_HOST;
    ba[7]=c;
    ba[8]=this->ready?1:0;
    QByteArray nk;
    nk=nick.toUtf8();
    ba[9]=nk.length();
    ba.append(nk);
    if(udpAvailable)
    {
        nk.resize(7);
        nk[0]=1;
        s=qToLittleEndian(udpPort);
        memcpy(nk.data()+1,&s,2);
        u=qToLittleEndian(udpCode);
        memcpy(nk.data()+3,&u,4);
        ba.append(nk);
    }
    else
        ba.append((char)0);
    return ba;
}
