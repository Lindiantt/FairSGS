#include "widgetzhuangbeismall.h"
#include <qlabel.h>
#include "card/ccard.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include <qpainter.h>
#include "qsanguoshautil/uiUtils.h"

WidgetZhuangBeiSmall::WidgetZhuangBeiSmall(QWidget *parent) : QWidget(parent)
{
    card=nullptr;
    labelIcon=new QLabel(this);
    labelIcon->move(1,1);
    labelIcon->resize(11,11);
    labelSuit=new QLabel(this);
    labelSuit->move(56,-2);
    labelSuit->resize(21,17);
    this->resize(75,14);
}

WidgetZhuangBeiSmall::~WidgetZhuangBeiSmall()
{

}

void WidgetZhuangBeiSmall::paintEvent(QPaintEvent *ev)
{
    if(card)
    {
        QPainter painter(this);
        if(card->type->type1!=CARDTYPE_FANGJU&&card->type->type1!=CARDTYPE_BAOWU)
            painter.drawPixmap(14,0,pixmapDistance);
        painter.drawPixmap(45,0,pixmapPoint);
    }
    QWidget::paintEvent(ev);
}

void WidgetZhuangBeiSmall::setCard(CCard* card)
{
    this->card=card;
    this->setVisible(true);
    w->imageLoad(w->wLoadSource->suitsSmall[card->suit],labelSuit);
    QString dc;
    if(card->type->type1==CARDTYPE_WUQI)
    {
        CCardWuqi *wuqi=static_cast<CCardWuqi *>(card->type);
        w->imageLoad(w->wLoadSource->iconWuQi[wuqi->iconType],labelIcon);
        dc=wuqi->distanceChinese();
    }
    else if(card->type->type1==CARDTYPE_FANGJU)
        w->imageLoad(w->wLoadSource->iconFangJu,labelIcon);
    else
    {
        w->imageLoad(w->wLoadSource->iconMa,labelIcon);
        if(card->type->type1==CARDTYPE_JIANYIMA)
            dc="-1";
        else if(card->type->type1==CARDTYPE_JIAYIMA)
            dc="+1";
    }
    if(!dc.isEmpty())
    {
        QImage img(30,14,QImage::Format_ARGB32);
        img.fill(QColor(0,0,0,0));
        QFont font("LiSu");
        font.setPixelSize(14);
        QPainter painter(&img);
        painter.setFont(font);
        painter.setPen(QColor(0,0,0));
        painter.drawText(0,14,dc);
        QImage imgShadow=QSanUiUtils::produceShadow(img,QColor(255,255,190,255),3,50);
        pixmapDistance=QPixmap::fromImage(imgShadow);
        QPainter pt(&pixmapDistance);
        pt.drawImage(0,0,img);
    }
    QImage img(14,14,QImage::Format_ARGB32);
    img.fill(QColor(0,0,0,0));
    QFont font("Arial");
    font.setPixelSize(12);
    QPainter painter(&img);
    painter.setFont(font);
    painter.setPen(QColor(0,0,0));
    painter.drawText(0,14,card->numberString());
    QImage imgShadow=QSanUiUtils::produceShadow(img,QColor(255,255,190,255),1,0);
    pixmapPoint=QPixmap::fromImage(imgShadow);
    QPainter pt(&pixmapPoint);
    pt.drawImage(0,0,img);
}
