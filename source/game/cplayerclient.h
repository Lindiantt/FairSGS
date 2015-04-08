#ifndef CPLAYERCLIENT_H
#define CPLAYERCLIENT_H

#include "game/cplayer.h"

class WidgetGamePlayer;
class WidgetRoomPlayer;
class WidgetGame;
class CImage;

class CPlayerClient : public CPlayer
{
    Q_OBJECT
public:
    CPlayerClient();
    ~CPlayerClient();
    void chooseGeneral(QList<QList<CGeneral*>> &);
    void deliverCard(const QList<CCard*> &);
    virtual void startTimeCount(int time,bool);
    virtual void stopTimeCount();
    void setGeneral(CGeneral *);
    void setKingdom(quint8);
    void setRole(quint8);
    void setMaxHP(quint8);
    void setHP(char);
    void needSelect(const QString &question,int selectionType,const QList<QVariant> values,
                    const QList<QVariant> &defaultResult,int min,int max);
    void getCard(const QList<CCard*> &list,CPlayer* player=nullptr);
    virtual void setSelected(bool);
    virtual void setSelectable(bool);
    virtual void setNoSelect();
    void phaseCallback(int phase, void* extre=nullptr, void* extre2=nullptr,void* extre3=nullptr,
                           void* extre4=nullptr);
    virtual void showPhase(CImage*);
    void needPlay(quint8 suit,quint16 type,const QList<quint8> type2,int number,CPlayer* from,CCard* card,
        int playMode,int cardMode,const QList<CPlayer*> targets);
    void needPlayCallback(quint8 suit,quint16 type,const QList<quint8> type2,int number,CPlayer* from,CCard* card,
        int playMode,int cardMode,const QList<CPlayer*> targets);
    void roundInit();
    void removeHand(CCard*);
    void needShow();
    void getRandomHand(const QList<CPlayer*> &knownList);
    bool selected,selectable;
    WidgetGamePlayer *wgp;
    WidgetRoomPlayer *wrp;
protected:
    void phasePlay();
    virtual void playPhaseInit();
    void playPhaseResponse();
    void needShowCallback();
};

#endif // CPLAYERCLIENT_H
