#include "cserverlist.h"
#include "define.h"

#include "ui_mainwindowserverlist.h"
#include "mainwindow.h"
extern MainWindow *w;

#include "ui/mainwindowserverlist.h"

CServerList::CServerList()
{
    initVar();
}

CServerList::~CServerList()
{

}

void CServerList::initVar()
{
    firstload=true;
    stop=false;
    loading=false;
    networkReply=NULL;
    leftServers.clear();
}

void CServerList::replyFinished()
{
    char buf[10];
    quint32 addr;
    quint16 port;
    int i,j,k;
    bool b;
    networkReply->read(buf,2);
    port=qFromLittleEndian(*(quint16*)buf);
    if(port!=VERSION_SERVERLIST)
    {
        w->mwServerList->statusBar()->showMessage("列表服务器返回的数据与本程序不兼容，如果您使用的是最新的版本，那么可能是服务器出现了故障。");
        stop=true;
    }
    else
    {
        i=networkReply->bytesAvailable();
        j=w->mwServerList->ui->tableWidgetServerList->rowCount();
        if(i>=200)
        {
            k=j+20;
            w->mwServerList->ui->tableWidgetServerList->setRowCount(k);
        }
        else if(i>=10)
        {
            k=j+i/10;
            w->mwServerList->ui->tableWidgetServerList->setRowCount(k);
        }
        i=0;
        b=false;
        while(networkReply->bytesAvailable()>=10)
        {
            networkReply->read(buf,10);
            addr=qFromLittleEndian(*(quint32*)buf);
            port=qFromLittleEndian(*(quint16*)(buf+4));
            addAddress(j+i,addr,port);
            i++;
            if(i==20)
            {
                b=true;
                break;
            }
        }
        if(!b)
        {
            stop=true;
        }
        else if(!firstload)
        {
            while(networkReply->bytesAvailable()>=10)
            {
                networkReply->read(buf,10);
                addr=qFromLittleEndian(*(quint32*)buf);
                port=qFromLittleEndian(*(quint16*)(buf+4));
                leftServers.append(QPair<quint32,quint16>(addr,port));
            }
            if(!leftServers.size())
            {
                stop=true;
            }
        }
        if(firstload) firstload=false;
    }
    loading=false;
    networkReply->deleteLater();
    networkReply=NULL;
}

void CServerList::requestList()
{
    if(loading)
    {
        w->mwServerList->statusBar()->showMessage("正在加载中");
        return;
    }
    if(stop)
    {
        w->mwServerList->statusBar()->showMessage("无法获得更多服务器。");
        return;
    }
    QString s=w->settings->value("slconfig/geturl",URL_DEFAULTGET).toString();
    if(firstload)
    {
        s+="servers";
        lastTime=QDateTime::currentDateTime();
        networkReply=w->network->get(QNetworkRequest(QUrl(s)));
        connect(networkReply,networkReply->finished,this,this->replyFinished);
        loading=true;
    }
    else
    {
        int l=leftServers.size();
        if(l)
        {
            QPair<quint32,quint16> pair;
            if(l>20) l=20;
            int j=w->mwServerList->ui->tableWidgetServerList->rowCount();
            if(l)
            {
                w->mwServerList->ui->tableWidgetServerList->setRowCount(j+l);
            }
            for(int i=0;i<l;i++)
            {
                pair=leftServers.takeFirst();
                addAddress(j+i,pair.first,pair.second);
            }
            if(!leftServers.size())
            {
                stop=true;
            }
        }
        else
        {
            s+="full";
            networkReply=w->network->get(QNetworkRequest(QUrl(s)));
            connect(networkReply,networkReply->finished,this,this->replyFinished);
            loading=true;
        }
    }
}

void CServerList::addAddress(int j, quint32 add, quint16 port)
{
    QHostAddress ha;
    ha.setAddress(add);
    w->mwServerList->addAddress(j,ha,port);
}

void CServerList::refresh()
{
    if(lastTime.secsTo(QDateTime::currentDateTime())<60)
    {
        w->mwServerList->statusBar()->showMessage("离上次获取列表的时间不足一分钟，请稍后再刷新。");
    }
    else
    {
        if(networkReply) networkReply->deleteLater();
        initVar();
        w->mwServerList->clearServers();
        requestList();
    }
}
