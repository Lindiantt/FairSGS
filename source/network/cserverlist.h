#ifndef CSERVERLIST_H
#define CSERVERLIST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>


class CServerList:public QObject
{
    Q_OBJECT
public:
    CServerList();
    ~CServerList();
    requestList();
    QNetworkAccessManager *network;
    QLinkedList<QPair<quint32,quint16> > leftServers;

private slots:
    replyFinished(QNetworkReply*);
private:
    bool firstload,stop;
    void addAddress(quint32,quint16);
};

#endif // CSERVERLIST_H
