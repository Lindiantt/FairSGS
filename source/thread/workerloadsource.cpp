#include "workerloadsource.h"

#include "mainwindow.h"
extern MainWindow *w;
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qmovie.h>
#include "thread/cimage.h"
#include "thread/cicon.h"
#include <qpushbutton.h>
#include "thread/ccheckpointer.h"

workerLoadSource::workerLoadSource(QObject *parent) : QObject(parent)
{
    ready=false;
    connect(this,this->wakeUp,this,this->init,Qt::QueuedConnection);
    connect(this,this->stopMovie,this,this->handleStopMovie,Qt::QueuedConnection);
    connect(this,SIGNAL(newWork(int,void*,CCheckPointer*)),this,SLOT(handleNew(int,void*,CCheckPointer*)),
        Qt::QueuedConnection);
    connect(this,SIGNAL(newWork(int,void*,void*)),this,SLOT(handleNew(int,void*,void*)),Qt::QueuedConnection);
}

workerLoadSource::~workerLoadSource()
{

}

void workerLoadSource::handleNew(int workType, void *pointer, CCheckPointer *lab)
{
    doWork(workType,pointer,lab,true);
    delete lab;
}

void workerLoadSource::handleNew(int workType, void *pointer, void *lab)
{
    doWork(workType,pointer,lab);
}

void workerLoadSource::imageSet(CImage *image, QWidget *widget)
{
    if(QLabel *label=qobject_cast<QLabel*>(widget))
        label->setPixmap(image->pixmap);
    else if(QToolButton *button=qobject_cast<QToolButton*>(widget))
        button->setIcon(QIcon(image->pixmap));
    else
    {
        widget->update();
    }
}

void workerLoadSource::doWork(int workType, void *pointer, void * lab, bool iscp)
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
        QWidget *widget;
        if(!image->loaded)
        {
            image->pixmap.load(image->filename);
            image->loaded=true;
        }
        if(iscp)
        {
            CCheckPointer* cp=(CCheckPointer*)lab;
            if(cp->findParent(cp->par))
            {
                widget=(QWidget*)cp->obj;
                if(widget) imageSet(image,widget);
            }
            cp->endFind();
        }
        else
        {
            widget=(QWidget*)lab;
            if(widget) imageSet(image,widget);
        }
    }
        break;
    case WORKTYPE_SOUND:
        break;
    case WORKTYPE_ICON:
    {
        CIcon *icon=(CIcon*)pointer;
        icon->load();
        if(iscp)
        {
            CCheckPointer* cp=(CCheckPointer*)lab;
            if(cp->findParent(cp->par))
            {
                QPushButton *button=(QPushButton*)cp->obj;
                if(button) button->setIcon(icon->icon);
            }
            cp->endFind();
        }
        else
        {
            QPushButton *button=(QPushButton*)lab;
            button->setIcon(icon->icon);
        }
    }
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
        generalCards[i]=new CImage("image/generals/card/"+QString::number(i)+".jpg");
        generalAvatars[i]=new CImage("image/generals/avatar/"+QString::number(i)+".png");
    }
    int j;
    for(int i=0;i<MAX_SHEN;i++)
    {
        j=i+10000;
        shenCards[i]=new CImage("image/generals/card/"+QString::number(j)+".jpg");
        shenAvatars[i]=new CImage("image/generals/avatar/"+QString::number(j)+".png");
    }
    bgRoom=new CImage("image/system/tableBg.jpg");
    bgGamePlayer=new CImage("image/system/photo-back.png");
    bgBottomLeft=new CImage("image/system/dashboard-hand.png");
    bgBottomRight=new CImage("image/system/dashboard-avatar.png");
    bgBottomEquip=new CImage("image/system/dashboard-equip.png");
    handCardNumber=new CImage("image/system/handcard.png");
    for(int i=0;i<6;i++)
    {
        blood[i]=new CImage("image/system/magatamas/"+QString::number(i)+".png");
    }
    for(int i=0;i<5;i++)
    {
        bloodBack[i]=new CImage("image/system/magatamas/bg"+QString::number(i+1)+".png");
        kingdomFrame[i]=new CImage("image/kingdom/frame/"+QString::number(i)+".png");
        kingdomIcon[i]=new CImage("image/kingdom/icon/"+QString::number(i)+".png");
        roleCorner[i]=new CImage("image/system/roles/corner"+QString::number(i)+".png");
    }
    for(int i=0;i<3;i++)
    {
        iconWuQi[i]=new CImage("image/equips/icon/"+QString::number(i)+".png");
    }
    iconMa=new CImage("image/equips/icon/horse.png");
    iconFangJu=new CImage("image/equips/icon/shield.png");
    for(int i=0;i<4;i++)
    {
        suitsSmall[i]=new CImage("image/system/cardsuit/"+QString::number(i)+".png");
    }
    for(int i=0;i<13;i++)
    {
        pointRed[i]=new CImage("image/system/red/"+QString::number(i+1)+".png");
        pointBlack[i]=new CImage("image/system/black/"+QString::number(i+1)+".png");
    }
    for(int i=0;i<44;i++)
    {
        cards[i]=new CImage("image/card/"+QString::number(i)+".jpg");
    }
    buttonWuXie=new CIcon("image/system/button/handcard/nullification/");
    cardBack=new CImage("image/system/card-back.png");
    frameMeSelected=new CImage("image/system/frame/dashboardSelected.png");
    framePlayerPlaying=new CImage("image/system/frame/playing.png");
    framePlayerResponding=new CImage("image/system/frame/responding.png");
    framePlayerSelected=new CImage("image/system/frame/photoSelected.png");
    framePlayerSOS=new CImage("image/system/frame/sos.png");
    ready=true;
}
