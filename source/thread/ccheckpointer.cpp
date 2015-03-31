#include "ccheckpointer.h"
#include "mainwindow.h"
extern MainWindow *w;

CCheckPointer::CCheckPointer(void *obj, void *par)
{
    this->obj=obj;
    this->par=par;
}

CCheckPointer::~CCheckPointer()
{
}

QMutex CCheckPointer::mutex;
QSet<void*> CCheckPointer::parList;

void CCheckPointer::deleteObject(void *ob)
{
    mutex.lock();
    parList.remove(ob);
    mutex.unlock();
    QObject *o=(QObject*)ob;
    o->deleteLater();
}

void CCheckPointer::addParent(void *ob)
{
    mutex.lock();
    parList.insert(ob);
    mutex.unlock();
}

bool CCheckPointer::findParent(void *ob)
{
    mutex.lock();
    return parList.find(ob)!=parList.end();
}

void CCheckPointer::endFind()
{
    mutex.unlock();
}
