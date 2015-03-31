#ifndef COPERATION_H
#define COPERATION_H

#include "pch.h"
class CGame;
class CPlayer;

class COperation : public QObject
{
    Q_OBJECT
public:
    explicit COperation(CGame *game);
    ~COperation();
    static COperation* newOperation(CGame*,QByteArray &);
    static COperation* serverRequest(QByteArray &);
    bool fromNotice(QByteArray &);
    CGame *game;
    bool finished;
    uint id;
    quint8 type;
    QList<QVariant> parameter;
    QMap<quint8,QPair<bool,QList<QVariant>>> reply,thirdPartReply;
    QByteArray requestBuf();
    QByteArray noticeBuf();
    QByteArray noticeBuf3rd();
    QByteArray replyBuf(QList<QVariant> &);
    void needReply(quint8);
    void needReply(const QList<quint8> );
    void replied(quint8,const QList<QVariant> );
    void deliver();
    void selectiveDeliver(QList<CPlayer*> &playersSeen);
    void checkReply();
    void send();

    friend QDataStream &operator<<(QDataStream &out, const COperation *op);
    friend QDataStream &operator>>(QDataStream &in, COperation * op);
signals:
    void signalFinished();
    void signalReplied(quint8);
public slots:
};



#endif // COPERATION_H
