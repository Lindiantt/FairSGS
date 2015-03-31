#ifndef CCHECKPOINTER_H
#define CCHECKPOINTER_H

#include "pch.h"

class CCheckPointer
{
public:
    CCheckPointer(void*,void*);
    ~CCheckPointer();
    static void deleteObject(void*);
    static QMutex mutex;
    static QSet<void*> parList;
    static void addParent(void*);
    static bool findParent(void*);
    static void endFind();
    void *obj,*par;
private:

};

#endif // CCHECKPOINTER_H
