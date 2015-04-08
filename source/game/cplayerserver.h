#ifndef CPLAYERSERVER_H
#define CPLAYERSERVER_H

#include "game/cplayer.h"

class CPlayerSocket;
class CGameServer;
class COperation;

class CPlayerServer:public CPlayer
{
    Q_OBJECT
public:
    CPlayerServer(CGameServer*,CPlayerSocket*);
    ~CPlayerServer();
    void chooseGeneral(QList<QList<CGeneral*>> &);
    void deliverCard(const QList<CCard*> &);//派牌及其他武将信息
    void needSelect(const QString &question,int selectionType,const QList<QVariant> values,
                    const QList<QVariant> &defaultResult,int min,int max);
    void sendGameStart();
    void socketDisconnected();
    void networkSend(QByteArray &);
    bool rhPlaying(QByteArray &);
    void setOffline(bool);
    bool checkActiveOperation(quint8 type,QList<QVariant> &);
    void needPlay(quint8 suit,quint16 type,const QList<quint8> type2,int number,CPlayer* from,CCard* card,
        int playMode,int cardMode,const QList<CPlayer*> targets);
    void useCard(CCard*,QList<CPlayer*> &);
    void endPlayPhase();
    void needShow();
    void setDefaultAction(bool,int,std::function<void()>);
    void getRandomHand(const QList<CPlayer*> &knownList);
    int state;
    quint16 bytesRemain;
    CPlayerSocket *socketHandle;
    std::function<void()> defaultAction;
    QTimer timerDA;

    bool generalOK(quint16);
    void standardDefaultAction(COperation*);
public slots:
    void handleOperationReplied(quint8);//用于处理总额外用时变更
protected:
    QSet<quint16> availabelGIDs;
    int selectReturn;
    bool selecting;
    QDateTime lastOpTime;
    bool useExtreTime;
    int regularTime;
    bool canUseCard;

    void clearDefaultAction();
    void phasePlay();
    void phaseDiscard();
signals:
    void selectionOK();
private slots:
    void handleDefaultTimeout();
};

#endif // CPLAYERSERVER_H
