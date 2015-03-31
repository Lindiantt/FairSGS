#include "widgetzhuangbeifull.h"
#include <qlabel.h>
#include "card/ccard.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include <qpainter.h>
#include "qsanguoshautil/uiUtils.h"

WidgetZhuangBeiFull::WidgetZhuangBeiFull(QWidget *parent) : WidgetZhuangBeiSmall(parent)
{
    this->resize(150,14);
    labelSuit->move(131,-2);
}

WidgetZhuangBeiFull::~WidgetZhuangBeiFull()
{

}

void WidgetZhuangBeiFull::paintEvent(QPaintEvent *ev)
{
    if(card)
    {
        QPainter painter(this);
        if(card->type->type1!=CARDTYPE_FANGJU&&card->type->type1!=CARDTYPE_BAOWU)
            painter.drawPixmap(14,0,pixmapDistance);
        painter.drawPixmap(120,0,pixmapPoint);
        painter.drawPixmap(35,0,pixmapName);
    }
    QWidget::paintEvent(ev);
}

void WidgetZhuangBeiFull::setCard(CCard* card)
{
    WidgetZhuangBeiSmall::setCard(card);

    QImage img(85,14,QImage::Format_ARGB32);
    img.fill(QColor(0,0,0,0));
    QFont font("LiSu");
    font.setPixelSize(14);
    QPainter painter(&img);
    painter.setFont(font);
    painter.setPen(QColor(0,0,0));
    painter.drawText(0,14,card->type->name);
    QImage imgShadow=QSanUiUtils::produceShadow(img,QColor(255,255,190,255),3,50);
    pixmapName=QPixmap::fromImage(imgShadow);
    QPainter pt(&pixmapName);
    pt.drawImage(0,0,img);
}
