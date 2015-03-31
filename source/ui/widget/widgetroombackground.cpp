#include "widgetroombackground.h"
#include <QPainter>
#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"

WidgetRoomBackground::WidgetRoomBackground(QWidget *parent) : QWidget(parent)
{
    w->imageLoad(w->wLoadSource->bgRoom,this);
    oldw=0;

}

WidgetRoomBackground::~WidgetRoomBackground()
{

}

void WidgetRoomBackground::paintEvent(QPaintEvent *ev)
{
    if(w->wLoadSource->ready&&w->wLoadSource->bgRoom->loaded)
    {
        QPainter painter(this);
        painter.drawPixmap(0,0,w->wLoadSource->bgRoom->pixmap.scaled(size()));
    }
    QWidget::paintEvent(ev);
}

void WidgetRoomBackground::resizeEvent(QResizeEvent *)
{
    int a=width();
    a=(a+30)/250;
    if(a!=oldw)
    {
        w->mwCLient->ui->widgetRoom->arrangePlayers();
        oldw=a;
    }
}
