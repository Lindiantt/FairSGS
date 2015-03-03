#ifndef CEVENT_H
#define CEVENT_H

#include "pch.h"

class CGame;

/*#define EVENTTYPE_USECARD 0
#define EVENTTYPE_NEEDCARD 1
#define EVENTTYPE_INJURED 2
*/

class CEvent : public QObject
{
    Q_OBJECT
public:
    explicit CEvent(CGame* game);
    ~CEvent();
    CEvent* parent;
    CGame* game;
    bool available;
    std::function<void()> funcDelete;

signals:

public slots:
};

#endif // CEVENT_H
