#ifndef CRUNTIMEEVENT_H
#define CRUNTIMEEVENT_H

typedef int (*func_msg)(int,void*,void*);
#include <qlist.h>

class CRuntimeEvent
{
public:
    CRuntimeEvent();
    ~CRuntimeEvent();
    bool run();
    void addEvents(QList<func_msg> &);

    QList<func_msg> events;
    CRuntimeEvent* upperEvents;
    bool base;
};

#endif // CRUNTIMEEVENT_H
