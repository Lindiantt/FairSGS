#ifndef CPLAYERCLIENT_H
#define CPLAYERCLIENT_H

#include "game/cplayer.h"

class WidgetGamePlayer;
class WidgetRoomPlayer;
class WidgetGame;


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
    int needSelect(const QString &question,int selectionType=SELECTTYPE_YESNO,
                               const QStringList option=QStringList());
    void getCard(const QList<CCard*> &list,CPlayer* player=nullptr);
    virtual void setSelected(bool);
    virtual void setSelectable(bool);
    virtual void setNoSelect();
    void useCard(CCard*,QList<CPlayer*> &);
    bool selected,selectable;
    WidgetGamePlayer *wgp;
    WidgetRoomPlayer *wrp;
protected:
    void phasePlay();
    virtual void startPlayPhase();
};

#endif // CPLAYERCLIENT_H
