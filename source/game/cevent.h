#ifndef CEVENT_H
#define CEVENT_H

#include "pch.h"

class CGame;
class CCard;
class CTempCard;

#define EVENT_UNKNOWN -1
#define EVENT_PHASE 0
#define EVENT_ABOUTTOJUDGE 1
#define EVENT_JUDGE 2
#define EVENT_INJURY 3
#define EVENT_JUEDOU 4

class CEvent : public QObject
{
    Q_OBJECT
public:
    explicit CEvent(CGame* game,int type=EVENT_UNKNOWN);
    ~CEvent();
    CEvent* parent;
    CGame* game;
    int type;
    QSet<quint8> cardsToDiscard;
    QList<CTempCard*> tempCards;
    std::function<void()> funcDelete;
    QLinkedList<std::function<void()>> funcs;
    void addFunc(std::function<void()>);
    void insertFunc(std::function<void()>);
    void addCard(CCard*);
    CEvent* findParent(int type);
    void takeCard(quint8);
    void takeCards(QList<quint8>);

signals:

public slots:
};

#endif // CEVENT_H
