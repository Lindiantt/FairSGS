#include "cclient.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
extern MainWindow *w;

#include "ui/dialog/dialogconnect.h"
#include <qmessagebox.h>
#include "define.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "ui/widget/widgetroom.h"
#include <qmovie.h>
#include "ui/dialog/dialogchooseroom.h"
#include "ui/mainwindow/mainwindowserver.h"
#include "game/cgameclient.h"

#define COMPARE(a,b) if((a)!=(b)){end2("与服务器断开连接。");return false;}

CClient::CClient(QString ip,quint16 port,quint8 authmode,QByteArray password,QString user,QObject *parent) : QObject(parent)
{
    game=nullptr;
    this->ip=ip;
    this->port=port;
    this->authmode=authmode;
    this->password=password;
    this->user=user;
    alreadyEnd=false;
    if(!w->dialogConnect)
    {
        w->dialogConnect=new DialogConnect(w);
    }
    w->dialogConnect->init();
    w->dialogConnect->show();
    timer.setSingleShot(true);
    connect(&timer,timer.timeout,this,this->handleTimeout);
    connect(&socket,socket.connected,this,this->handleConnected);
    connect(&socket,socket.disconnected,this,this->handleDisconnected);
    connect(&socket,socket.readyRead,this,this->handleRead);
    connect(&socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
    state=CLIENTSTATE_INIT;
    socket.connectToHost(ip,port);
    timer.start(10000);
}

CClient::~CClient()
{
    w->client=nullptr;
    if(game)
    {
        game->deleteLater();
    }
}

void CClient::handleTimeout()
{
    end();
    w->dialogConnect->end();
    QMessageBox::about(w,"错误","连接服务器失败。");
}

void CClient::handleConnected()
{
    timer.stop();
    w->dialogConnect->end();
    w->hide();
    if(!w->mwCLient)
    {
        w->mwCLient=new MainWindowClient();
    }
    w->mwCLient->init();
    w->mwCLient->showMaximized();
    QByteArray sendbuf;
    quint16 vn=VALIDATIONNUMBER;
    vn=qToLittleEndian(vn);
    sendbuf.resize(6);
    memcpy(sendbuf.data(),&vn,2);
    vn=VERSION_COMMUNICATION;
    vn=qToLittleEndian(vn);
    memcpy(sendbuf.data()+2,&vn,2);
    sendbuf[4]=ACTION_CONNECT;
    sendbuf[5]=authmode;
    QByteArray ba;
    quint8 len;
    if(authmode==2)
    {
        ba=user.toUtf8();
        len=ba.length();
        sendbuf.append((char)len);
        sendbuf.append(ba);
        sendbuf.append(password);
    }
    else
    {
        if(authmode==1)
        {
            sendbuf.append(password);
        }
        user=w->settings->value("personal/nick","玩家").toString();
        ba=user.toUtf8();
        len=ba.length();
        if(!len||len>30)
        {
            end2("请重设昵称。");
            return;
        }
        sendbuf.append((char)len);
        sendbuf.append(ba);
    }
    vn=w->settings->value("personal/favorite",0).toInt();
    vn=qToLittleEndian(vn);
    sendbuf.append((char*)&vn,2);
    state=CLIENTSTATE_CONNECTED;
    socket.write(sendbuf);
}

void CClient::handleDisconnected()
{
    end2("服务器主动断开连接！");
}

void CClient::handleRead()
{
aa:
    if(state==CLIENTSTATE_CONNECTED)
    {
        if(!rhConnected()) return;
    }
    else
    {
        QByteArray ba;
        if(!bytesRemain)
        {
            if(socket.bytesAvailable()>=2)
            {
                ba=socket.read(2);
                memcpy(&bytesRemain,ba.data(),2);
                bytesRemain=qFromLittleEndian(bytesRemain);
                if(!bytesRemain)
                {
                    if(!udpConfirmed)
                    {
                        udpConfirmed=true;
                        connect(&timerUdpLive,timerUdpLive.timeout,this,this->udpLive);
                        timerUdpLive.start(30000);
                        goto bb;
                    }
                    else
                    {
                        end2("与服务器断开连接。");
                        return;
                    }
                }
            }
            else
                return;
        }
        if(socket.bytesAvailable()<bytesRemain)
            return;
        ba=socket.read(bytesRemain);
        bytesRemain=0;
        switch(state)
        {
        case CLIENTSTATE_INSERVER:
            if(!rhInserver(ba)) return;
            break;
        case CLIENTSTATE_CHOOSEROOM:
            if(!w->mwCLient->dialogChooseRoom->rhChooseRoom(ba)) return;
            break;
        case CLIENTSTATE_ENTERROOM:
            if(!w->mwCLient->ui->widgetRoom->rhEnterRoom(ba))
            {
                end2("与服务器断开连接。");
                return;
            }
            break;
        case CLIENTSTATE_INROOM:
            if(!w->mwCLient->ui->widgetRoom->rhInRoom(ba)) return;
            break;
        case CLIENTSTATE_STARTGAME:
            if(!w->mwCLient->ui->widgetRoom->rhStartGame(ba)) return;
            break;
        case CLIENTSTATE_PLAYING:
            if(!w->mwCLient->ui->widgetGame->rhPlaying(ba)) return;
            break;
        default:
            break;
        }
    }
bb:
    if(socket.bytesAvailable()) goto aa;
}

void CClient::handleError(QAbstractSocket::SocketError)
{
    if(state==CLIENTSTATE_INIT)
    {
        handleTimeout();
    }
}

void CClient::end()
{
    timer.stop();
    socket.disconnectFromHost();
    this->deleteLater();
    w->client=nullptr;
}

void CClient::end2(const QString &err)
{
    if(!this||alreadyEnd) return;
    alreadyEnd=true;
    w->mwCLient->hide();
    if(w->mwCLient->dialogChooseRoom) w->mwCLient->dialogChooseRoom->hide();
    end();
    QWidget *wgt;
    if(w->server)
        wgt=w->mwServer;
    else
    {
        wgt=w;
        if(w->mwServer) w->mwServer->hideAll();
    }
    wgt->show();
    if(!err.isEmpty()) QMessageBox::about(wgt,"错误",err);
}

bool CClient::rhConnected()
{
    QByteArray ba;
    ba=socket.read(1);
    COMPARE(ba.length(),1);
    if(ba[0]==NETWORK_ERROR_VERSION)
    {
        ba=socket.read(1);
        COMPARE(ba.length(),1);
        quint8 len=ba[0];
        ba=socket.read(len);
        COMPARE(ba.length(),len);
        end2("与服务器的版本不兼容。\n服务器的版本为："+QString::fromUtf8(ba));
        return false;
    }
    COMPARE(ba[0],NETWORK_OK);
    ba=socket.read(1);
    COMPARE(ba.length(),1);
    if(ba[0]==CONNECT_ERROR_AUTHMODE)
    {
        ba=socket.read(1);
        COMPARE(ba.length(),1);
        QString s;
        if(ba[0]==(char)0)
            s="无";
        else if(ba[0]==(char)1)
            s="密码";
        else
            s="用户名+密码";
        end2("使用了错误的验证方式。\n服务器的验证方式为："+s);
        return false;
    }
    else if(ba[0]==CONNECT_ERROR_PASSWORD)
    {
        QString s;
        if(authmode==1)
            s="密码错误。";
        else
            s="用户名或密码错误。";
        end2(s);
        return false;
    }
    COMPARE(ba[0],(char)CONNECT_OK);
    ba=socket.read(15);
    COMPARE(ba.length(),15);
    memcpy(&maxRoom,ba.data(),4);
    maxRoom=qFromLittleEndian(maxRoom);
    numberOfPlayer=ba[4];
    if(numberOfPlayer<2||numberOfPlayer>10)
    {
        end2("服务器错误！");
        return false;
    }
    shuangNei=ba[5]?true:false;
    operationTimeout=ba[6];
    wuXieTimeout=ba[7];
    extreTime=ba[8];
    choiceTimeout=ba[9];
    allowChat=ba[10]?true:false;
    memcpy(&udpCode,ba.data()+11,4);
    udpConfirmed=false;
    if(udp.bind())
    {
        connect(&udp,udp.readyRead,this,this->handleUdpRead);
        udpWaitTime=5000;
        udpConfirm();
    }
    bytesRemain=0;
    state=CLIENTSTATE_INSERVER;
    w->mwCLient->ui->widgetRoom->init();
    return true;
}

void CClient::handleUdpRead()
{

}

void CClient::udpConfirm()
{
    if(!udpConfirmed)
    {
        udp.writeDatagram((char*)&udpCode,4,socket.peerAddress(),port);
        if(udpWaitTime<5000<<8)
            QTimer::singleShot(udpWaitTime,this,this->udpConfirm);
        udpWaitTime<<=1;
    }
}

bool CClient::rhInserver(QByteArray &ba)
{
    if(ba[0]==SERVER_CHOOSEROOM)
    {
        state=CLIENTSTATE_CHOOSEROOM;
        if(!w->mwCLient->dialogChooseRoom)
        {
            w->mwCLient->dialogChooseRoom=new DialogChooseRoom(w->mwCLient);
        }
        ba.remove(0,1);
        bool b=w->mwCLient->dialogChooseRoom->init(1,ba);
        COMPARE(b,true);
        w->mwCLient->dialogChooseRoom->show();
    }
    else if(ba[0]==ROOM_JOIN_OK)
    {
        COMPARE(ba.length(),5);
        state=CLIENTSTATE_ENTERROOM;
        uint u;
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        w->mwCLient->roomID=u;
    }
    else if(ba[0]==ROOM_JOIN_ERROR_FULL)
    {
        end2("服务器已满！");
        return false;
    }
    else if(ba[0]==ROOM_ONLOOK_OK)
    {
        COMPARE(ba.length(),5);
        state=CLIENTSTATE_ONLOOKROOM;
        uint u;
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        w->mwCLient->roomID=u;
    }
    else if(ba[0]==SERVER_RECONNECT)
    {

    }
    else
        COMPARE(1,0);
    return true;
}

void CClient::send(QByteArray &ba)
{
    quint16 len=ba.length()-2;
    len=qToLittleEndian(len);
    memcpy(ba.data(),&len,2);
    socket.write(ba);
}

void CClient::udpLive()
{
    quint32 ip=0x12345678;
    QHostAddress ha(ip);
    char buf[1];
    udp.writeDatagram(buf,1,ha,79);
}
