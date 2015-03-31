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
    int needSelect(const QString &question,int selectionType=SELECTTYPE_YESNO,
                               const QStringList option=QStringList());
    void setSelected(bool);
    void setSelectable(bool);
    void setNoSelect();
    void useCard(CCard*,QList<CPlayer*> &);
signals:
    void quitLoop();
protected:
    COperation *opSelect;
    void startPlayPhase();
protected slots:
    void handleSelected(int);
};

#endif // CPLAYERCLIENTME_H
