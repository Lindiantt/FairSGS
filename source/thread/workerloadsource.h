#ifndef WORKERLOADSOURCE_H
#define WORKERLOADSOURCE_H

#include "pch.h"
#include "network/define.h"
#define WORKTYPE_MOVIE 0
#define WORKTYPE_IMAGE 1
#define WORKTYPE_SOUND 2

class CImage;
typedef CImage* pCImage;
typedef QMovie* pQMovie;

class workerLoadSource : public QObject
{
    Q_OBJECT
public:
    explicit workerLoadSource(QObject *parent = 0);
    ~workerLoadSource();
    pQMovie movieLoading;
    pCImage generalCards[MAX_GENERAL],bgRoom;
public slots:
    void handleNew(int workType,void* pointer,void* lab);
    void handleStopMovie(QMovie*);
    void init();
signals:
    void newWork(int workType,void* pointer,void* lab);
    void stopMovie(QMovie*);
    void wakeUp();
};

#endif // WORKERLOADSOURCE_H
