#include "cslsockethandle.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "ui_mainwindowserverlist.h"

#include "define.h"
#include "ui/mainwindow/mainwindowserverlist.h"

CSLSocketHandle::CSLSocketHandle(QHostAddress &address,quint16 port,QTableWidgetItem* item)
{
    socket=NULL;
    this->hostaddress=address;
    this->port=port;
    this->item=item;
    connect(&timer,timer.timeout,this,this->handleTimeout);
}

CSLSocketHandle::~CSLSocketHandle()
{
    if(socket)
    {
        socket->deleteLater();
        int i=item->row();
        QTableWidgetItem *ti;
        ti=w->mwServerList->ui->tableWidgetServerList->item(i,1);
        if(ti->text()!="")
        {
            ti->setText("无法连接");
            ti->setTextColor(QColor(255,0,0));
        }
    }
    item->setData(Qt::UserRole,0);
    w->mwServerList->socketFinished();
}

void CSLSocketHandle::getInfo()
{
    socket=new QTcpSocket;
    connect(socket,socket->connected,this,this->handleConnected);
    connect(socket,socket->readyRead,this,this->handleRead);
    connect(socket,socket->disconnected,this,this->handleDisconnected);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
    lastTime=QDateTime::currentDateTime();
    int i=item->row();
    QTableWidgetItem *ti;
    ti=w->mwServerList->ui->tableWidgetServerList->item(i,1);
    ti->setText("连接中");
    ti->setTextColor(QColor(0,0,0));
    socket->connectToHost(hostaddress,port);
    timer.start(5000);
}

void CSLSocketHandle::handleError(QAbstractSocket::SocketError)
{
    this->deleteLater();
}

void CSLSocketHandle::handleTimeout()
{
    if(lastTime.msecsTo(QDateTime::currentDateTime())>=5000)
    {
        this->deleteLater();
    }
}

void CSLSocketHandle::handleConnected()
{
    qint64 ms=lastTime.msecsTo(QDateTime::currentDateTime());
    lastTime=QDateTime::currentDateTime();
    int i=item->row();
    QTableWidgetItem *ti;
    ti=w->mwServerList->ui->tableWidgetServerList->item(i,1);
    ti->setText(QString::number(ms)+"ms");
    if(ms<100)
    {
        ti->setTextColor(QColor(0,255,0));
    }
    else if(ms<1000)
    {
        ti->setTextColor(QColor(255,255,0));
    }
    else
    {
        ti->setTextColor(QColor(255,0,0));
    }
    unsigned char buf[5];
    buf[4]=ACTION_GETINFO;
    quint16 u=VALIDATIONNUMBER;
    u=qToLittleEndian(u);
    memcpy(buf,&u,2);
    u=VERSION_COMMUNICATION;
    u=qToLittleEndian(u);
    memcpy(buf+2,&u,2);
    socket->write((char*)buf,5);
    timer.start(5000);
}

void CSLSocketHandle::handleDisconnected()
{
    this->deleteLater();
}

void CSLSocketHandle::handleRead()
{
    unsigned char buf[3],*bu;
    socket->read((char*)buf,1);
    int i=item->row();
    QTableWidgetItem *ti;
    QTableWidget* tw=w->mwServerList->ui->tableWidgetServerList;
    ti=tw->item(i,2);
    switch (buf[0]) {
    case NETWORK_ERROR_VERSION:
    {
        if(socket->bytesAvailable()<1)
        {
            infoError(ti);
            break;
        }
        socket->read((char*)buf,1);
        if(socket->bytesAvailable()<buf[0])
        {
            infoError(ti);
            break;
        }
        bu=new unsigned char[buf[0]];
        socket->read((char*)bu,buf[0]);
        QString s;
        s=s.fromUtf8((char*)bu,buf[0]);
        ti->setText("不兼容("+s+")");
        ti->setTextColor(QColor(255,0,0));
        delete[] bu;
    }
        break;
    case NETWORK_OK:
    {
        if(socket->bytesAvailable()<3)
        {
            infoError(ti);
            break;
        }
        socket->read((char*)buf,3);
        unsigned char k;
        //模式
        k=buf[0]&0xf0;
        k>>=4;
        ti=tw->item(i,3);
        QString s;
        s=s.number(k)+"人";
        if(buf[0]&0b00001000) s+="双内";
        ti->setText(s);
        //验证方式
        k=buf[0]&0b110;
        k>>=1;
        switch(k)
        {
        case 0:
            s="无";
            break;
        case 1:
            s="密码";
            break;
        case 2:
            s="用户名+密码";
            break;
        case 3:
            s="用户名+密码(不可注册)";
            break;
        }
        ti=tw->item(i,5);
        ti->setText(s);
        //神将
        k=((*(quint16*)buf)&0b110000000)>>7;
        switch (k) {
        case SHENMODE_BAN:
            s="禁止";
            break;
        case SHENMODE_DEFAULT:
            s="默认";
            break;
        case SHENMODE_CUSTOM:
            s="自定义";
            break;
        default:
            s="";
            break;
        }
        ti=tw->item(i,7);
        //人数
        k=buf[1]&0b01111111;
        s=s.number(k);
        ti=tw->item(i,4);
        ti->setText(s);
        ti=tw->item(i,2);
        ti->setText("兼容");
        ti->setTextColor(QColor(0,0,0));
        //服务器名
        if(buf[2]&&socket->bytesAvailable()>=buf[2])
        {
            bu=new unsigned char[buf[2]];
            socket->read((char*)bu,buf[2]);
            s=s.fromUtf8((char*)bu);
            ti=tw->item(i,6);
            ti->setText(s);
            delete[] bu;
        }
    }
        break;
    default:
        infoError(ti);
        break;
    }
    this->deleteLater();
}

void CSLSocketHandle::infoError(QTableWidgetItem *ti)
{
    ti->setText("不兼容(未知版本)");
    ti->setTextColor(QColor(255,0,0));
}
