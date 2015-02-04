#include "cserverlist.h"
#include <qnetworkrequest.h>
#include <QtEndian>
#include "network/define.h"

#include "mainwindow.h"
extern MainWindow *w;

CServerList::CServerList()
{
    firstload=true;
    stop=false;
    network=new QNetworkAccessManager(this);
    connect(network,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
}

CServerList::~CServerList()
{
    network->deleteLater();
}

CServerList::replyFinished(QNetworkReply *nr)
{
    char buf[10];
    quint32 addr;
    quint16 port;
    nr->read(buf,2);
    port=qFromLittleEndian(*(quint16*)buf);
    if(port!=VERSION_SERVERLIST)
    {
        w->mwServerList->statusBar()->showMessage("列表服务器返回的数据与本程序不兼容，如果您使用的是最新的版本，那么可能是服务器出现了故障。");
        stop=true;
        return;
    }
    while(nr->bytesAvailable()>=10)
    {
        nr->read(buf,10);
        addr=qFromLittleEndian(*(quint32*)buf);
        port=qFromLittleEndian(*(quint16*)(buf+4));
        addAddress(addr,port);
    }
}

CServerList::requestList()
{
    if(stop)
    {
        w->mwServerList->statusBar()->showMessage("无法获得更多服务器。");
        return;
    }
    QString s=w->settings->value("slconfig/geturl","http://fairsgs.tk/").toString();
    if(firstload)
    {
        s+="servers";
        firstload=false;
    }
    else
    {
        int l=leftServers.size();
        if(l)
        {
            QPair<quint32,quint16> pair;
            for(int i=0;i<20&&i<l;i++)
            {
                pair=leftServers.takeFirst();
                addAddress(pair.first,pair.second);
            }
        }
        s+="full";
    }
    network->get(QNetworkRequest(QUrl(s)));
}

void CServerList::addAddress(quint32 add, quint16 port)
{
    QHostAddress ha;
    ha.setAddress(add);
    w->mwServerList->addAddress(ha,port);
}
