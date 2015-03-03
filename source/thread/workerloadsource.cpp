#include "workerloadsource.h"

#include "mainwindow.h"
extern MainWindow *w;
#include <qlabel.h>
#include <qmovie.h>
#include "thread/cimage.h"

workerLoadSource::workerLoadSource(QObject *parent) : QObject(parent)
{
    connect(this,this->wakeUp,this,this->init);
    connect(this,this->stopMovie,this,this->handleStopMovie);
    connect(this,this->newWork,this,this->handleNew);
}

workerLoadSource::~workerLoadSource()
{

}

void workerLoadSource::handleNew(int workType, void *pointer,void *lab)
{
    switch (workType) {
    case WORKTYPE_MOVIE:
    {
        QMovie *movie=(QMovie*)pointer;
        QLabel *label=(QLabel*)lab;
        label->setMovie(movie);
        movie->start();
    }
        break;
    case WORKTYPE_IMAGE:
    {
        CImage *image=(CImage*)pointer;
        QLabel *label=(QLabel*)lab;
        if(!image->loaded)
        {
            image->pixmap.load(image->filename);
            image->loaded=true;
        }
        if(label)
            label->setPixmap(image->pixmap);
    }
        break;
    case WORKTYPE_SOUND:
        break;
    default:
        break;
    }
}

void workerLoadSource::handleStopMovie(QMovie *movie)
{
    movie->stop();
}

void workerLoadSource::init()
{
    movieLoading=new QMovie("img/loading.gif");
    movieLoading->setCacheMode(QMovie::CacheAll);
    for(int i=0;i<MAX_GENERAL;i++)
    {
        generalCards[i]=new CImage("image/general/card/"+QString::number(i)+".jpg");
    }
    bgRoom=new CImage("image/system/tableBg.jpg");
}
