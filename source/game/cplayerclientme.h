#ifndef CPLAYERCLIENTME_H
#define CPLAYERCLIENTME_H

#include "game/cplayerclient.h"
class COperation;

class CPlayerClientMe : public CPlayerClient
{
    Q_OBJECT
public:
    CPlayerClientMe();
    ~CPlayerClientMe();
    void startTimeCount(int time,bool);
    void timeoutTimeCount();
    void stopTimeCount();
    void setGeneral(CGeneral *);
    void setKingdom(quint8);
    void setRole(quint8);
    void setMaxHP(quint8);
    void setHP(char);
    void deliverCard(const QList<CCard*> &);
    void getCard(const QList<CCard*> &list,CPlayer* player=nullptr);
    void needSelect(const QString &question,int selectionType,const QList<QVariant> values,
                    const QList<QVariant> &defaultResult,int min,int max);
    void setSelected(bool);
    void setSelectable(bool);
    void setNoSelect();
    void needPlay(quint8 suit,quint16 type,const QList<quint8> type2,int number,CPlayer* from,CCard* card,
        int playMode,int cardMode,const QList<CPlayer*> targets);
    void showPhase(CImage*);
    void roundInit();
    void removeHand(CCard*);
    void needShow();
    COperation *opSelect;
signals:
    void quitLoop();
protected:
    void playPhaseInit();
protected slots:
    void handleSelected(int);
};

#endif // CPLAYERCLIENTME_H
