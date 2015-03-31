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
    void needSelect(COperation*);
    void sendGameStart();
    void socketDisconnected();
    void networkSend(QByteArray &);
    bool rhPlaying(QByteArray &);
    void setOffline(bool);
    bool checkActiveOperation(quint8 type,QList<QVariant> &);
    void needPlay(quint16 type,QList<quint8> *type2=nullptr,int number=1,CPlayer* from=nullptr,CCard* card=nullptr,
        int playMode=0,int cardMode=0);
    void useCard(CCard*,QList<CPlayer*> &);
    int state;
    quint16 bytesRemain;
    CPlayerSocket *socketHandle;
    std::function<void()> defaultAction;
    QTimer timerDA;

    bool generalOK(quint16);
public slots:
    void handleOperationReplied(quint8);//用于处理总额外用时变更
protected:
    QSet<quint16> availabelGIDs;
    int selectReturn;
    bool selecting;
    QDateTime lastOpTime;
    bool useExtreTime;
    int regularTime;
    void setDefaultAction(bool,int,std::function<void()>);
    void clearDefaultAction();
    void phasePlay();
signals:
    void selectionOK();
private slots:
    void handleDefaultTimeout();
};

#endif // CPLAYERSERVER_H
