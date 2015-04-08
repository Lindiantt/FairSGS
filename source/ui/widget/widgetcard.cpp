#include "widgetcard.h"
#include "ui_widgetcard.h"
#include "card/ccard.h"
#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include <qpainter.h>
#include "thread/ccheckpointer.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "ui/widget/widgetbottom.h"
#include <QGraphicsOpacityEffect>

WidgetCard::WidgetCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetCard)
{
    card=nullptr;
    isOn=false;
    selectable=false;
    ui->setupUi(this);
    CCheckPointer::addParent(this);
    opEffect=new QGraphicsOpacityEffect();
    this->setGraphicsEffect(opEffect);
}

WidgetCard::~WidgetCard()
{
    delete ui;
}

void WidgetCard::setCard(CCard *card)
{
    this->card=card;
    if(!card)
    {
        w->imageLoad(w->wLoadSource->cardBack,this,this);
        return;
    }
    w->imageLoad(w->wLoadSource->cards[card->type->type2],this,this);
    CImage *point;
    if(card->suit==SUIT_FANGKUAI||card->suit==SUIT_HONGTAO)
        point=w->wLoadSource->pointRed[card->number-1];
    else if(card->suit==SUIT_HEITAO||card->suit==SUIT_MEIHUA)
        point=w->wLoadSource->pointBlack[card->number-1];
    else
        return;
    w->imageLoad(point,ui->labelPoint,this);
    w->imageLoad(card->suitIcon(),ui->labelSuit,this);
    this->setToolTip(card->description());
}

void WidgetCard::paintEvent(QPaintEvent *ev)
{
    if(w->wLoadSource->ready)
    {
        QPainter painter(this);
        if(card)
        {
            if(w->wLoadSource->cards[card->type->type2]->loaded)
                painter.drawPixmap(0,0,w->wLoadSource->cards[card->type->type2]->pixmap);
        }
        else
        {
            if(w->wLoadSource->cardBack->loaded)
                painter.drawPixmap(0,0,w->wLoadSource->cardBack->pixmap);
        }
    }
    QWidget::paintEvent(ev);
}

void WidgetCard::mousePressEvent(QMouseEvent *)
{
    if(selectable)
        emit clicked();
}

void WidgetCard::setSelectable(bool b)
{
    qreal opct;
    if(b)
        opct=1.0;
    else
        opct=0.7;
    selectable=b;
    opEffect->setOpacity(opct);
}
