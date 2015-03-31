#include "cimage.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"

CImage::CImage(const QString &s)
{
    filename=s;
    loaded=false;
}

CImage::~CImage()
{

}

CImage* CImage::avatarById(quint16 id)
{
    if(id>=10000)
        return w->wLoadSource->shenAvatars[id-10000];
    else
        return w->wLoadSource->generalAvatars[id];
}
