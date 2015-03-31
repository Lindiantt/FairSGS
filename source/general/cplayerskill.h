#ifndef CPLAYERSKILL_H
#define CPLAYERSKILL_H

#include <QObject>
class CSkill;
class CPlayer;

class CPlayerSkill : public QObject
{
    Q_OBJECT
public:
    explicit CPlayerSkill(CSkill *skill,CPlayer *player);
    ~CPlayerSkill();
    CSkill *skill;
    CPlayer *player;
    bool onlyOnceUsed;
    void activeSkill();
    void getEvent();
    void loseEvent();
    void phaseCallback(int phase,CPlayer* player=nullptr, void* extre=nullptr,
                               void* extre2=nullptr, void* extre3=nullptr, void* extre4=nullptr);
    QList<QVariant> vars;
signals:

public slots:
};

#endif // CPLAYERSKILL_H
