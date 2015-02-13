#ifndef CSERVERLIST_H
#define CSERVERLIST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLinkedList>
#include <qdatetime.h>


class CServerList:public QObject
{
    Q_OBJECT
public:
    CServerList();
    ~CServerList();
    void requestList();
    void refresh();

    QLinkedList<QPair<quint32,quint16> > leftServers;

private slots:
    void replyFinished();
private:
    bool firstload,stop,loading;
    void addAddress(int,quint32,quint16);
    void initVar();
    QDateTime lastTime;
    QNetworkReply* networkReply;
};

#endif // CSERVERLIST_H
