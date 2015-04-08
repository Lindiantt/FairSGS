#include "widgetbottom.h"
#include "ui_widgetbottom.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include <qpainter.h>
#include "widgetgame.h"
#include "game/cplayerclientme.h"
#include "general/cgeneral.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "game/cgameclient.h"
#include <qlabel.h>
#include "ui/widget/widgetcard.h"
#include "thread/ccheckpointer.h"
#include "ui_widgetgame.h"
#include "ui/widget/widgetgameplayer.h"
#include <QMouseEvent>
#include "general/cskill.h"
#include "network/cclient.h"
#include "card/ccard.h"

#define PLAYER ((CPlayerClient*)player)

WidgetBottom::WidgetBottom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBottom)
{
    ui->setupUi(this);
    init();
    w->imageLoad(w->wLoadSource->bgBottomLeft,this);
    w->imageLoad(w->wLoadSource->bgBottomRight,this);
    w->imageLoad(w->wLoadSource->bgBottomEquip,this);
    QFont font("Arial",12);
    font.setBold(true);
    for(int i=0;i<5;i++)
    {
        labelBloods[i]=new QLabel(this);
        labelBloods[i]->resize(22,26);
        labelBloods[i]->setAlignment(Qt::AlignCenter);
        labelBloods[i]->setFont(font);
    }
    w->imageLoad(w->wLoadSource->handCardNumber,ui->labelHands);
    w->imageLoad(w->wLoadSource->frameMeSelected,ui->labelSelected);
    arrangeBloods();
}

WidgetBottom::~WidgetBottom()
{
    delete ui;
}

void WidgetBottom::init()
{
    handSelectable=false;
    zhuangBeiSelectable=false;
    maxSelection=1;
    selectedCards=0;
    me=nullptr;
    avatarOpacity=false;
    ui->labelSelected->setVisible(false);
}

void WidgetBottom::arrangeBloods()
{
    for(int i=0;i<5;i++)
    {
        labelBloods[i]->move(width()-30,13+30*i);
    }
    ui->labelKingdom->move(width()-169,2);
    ui->labelHands->move(width()-192,148);
    ui->labelHandsNumber->move(width()-192,148);
    ui->labelRole->move(width()-196,0);
    ui->labelSelected->move(width()-163,10);
}

void WidgetBottom::resizeEvent(QResizeEvent *)
{
    arrangeBloods();
    arrangeCards();
}

void WidgetBottom::paintEvent(QPaintEvent *ev)
{
    if(w->wLoadSource->ready&&w->wLoadSource->bgBottomLeft->loaded&&w->wLoadSource->bgBottomRight->loaded)
    {
        QPainter painter(this);
        painter.fillRect(0,0,width(),height(),QColor(0,0,0));
        CGameClient* game=w->mwCLient->ui->widgetGame->game;
        if(game->me&&game->me->general)
        {
            CImage *img=CImage::avatarById(game->me->general->id);
            if(img->loaded)
            {
                if(avatarOpacity) painter.setOpacity(0.7);
                painter.drawPixmap(QPoint(width()-163,10),img->pixmap,QRect(64,24,134,134));
                painter.setOpacity(1.0);
            }
        }
        painter.drawPixmap(width()-171,0,w->wLoadSource->bgBottomRight->pixmap);
        if(w->wLoadSource->bgBottomEquip->loaded)
            painter.drawPixmap(0,0,w->wLoadSource->bgBottomEquip->pixmap);
        painter.drawPixmap(164,0,w->wLoadSource->bgBottomLeft->pixmap.scaled(width()-335,170));
    }
    QWidget::paintEvent(ev);
}

void WidgetBottom::setMaxHP(int hp)
{
    if(hp<=5)
    {
        int i;
        for(i=0;i<hp;i++)
        {
            labelBloods[i]->show();
        }
        for(;i<5;i++)
        {
            labelBloods[i]->hide();
        }
    }
    else
    {
        labelBloods[0]->show();
        labelBloods[1]->show();
        labelBloods[2]->show();
        labelBloods[3]->hide();
        labelBloods[4]->hide();
        labelBloods[1]->setText("/");
        labelBloods[2]->setText(QString::number(hp));
    }
    setHP(me->HP);
}

void WidgetBottom::setHP(int hp)
{
    if(me->maxHP>5)
    {
        labelBloods[0]->setText(QString::number(hp));
        QString s;
        if(hp>=5)
            s="(147,216,124)";
        else if(hp==4)
            s="(199,234,130)";
        else if(hp==3)
            s="(234,234,131)";
        else if(hp==2)
            s="(255,180,127)";
        else
            s="(240,97,97)";
        s="color: rgb"+s+";";
        for(int i=0;i<3;i++)
        {
            labelBloods[i]->setStyleSheet(s);
        }
    }
    else if(me->maxHP>=1)
    {
        CImage *img;
        if(hp==me->maxHP)
            img=w->wLoadSource->blood[5];
        else
            img=w->wLoadSource->blood[hp];
        int i;
        for(i=0;i<hp;i++)
        {
            w->imageLoad(img,labelBloods[i]);
        }
        for(;i<me->maxHP;i++)
        {
            w->imageLoad(w->wLoadSource->blood[0],labelBloods[i]);
        }
    }
}

void WidgetBottom::setKingdom(int kingdom)
{
    w->imageLoad(w->wLoadSource->kingdomIcon[kingdom],ui->labelKingdom);
}

void WidgetBottom::setHands(int hands)
{
    ui->labelHandsNumber->setText(QString::number(hands));
}

void WidgetBottom::setRole(int role)
{
    roleHiden=false;
    w->imageLoad(w->wLoadSource->roleCorner[role],ui->labelRole);
}

void WidgetBottom::on_labelRole_clicked()
{
    if(me)
    {
        if(roleHiden)
        {
            roleHiden=false;
            w->imageLoad(w->wLoadSource->roleCorner[me->role],ui->labelRole);
        }
        else
        {
            roleHiden=true;
            w->imageLoad(w->wLoadSource->roleCorner[ROLE_UNKNOWN],ui->labelRole);
        }
    }
}

void WidgetBottom::renewCardInfo()
{
    WidgetCard* wcard;
    foreach (wcard, hands) {
        CCheckPointer::deleteObject(wcard);
    }
    hands.clear();
    CCard* card;
    foreach (card, me->hands) {
        wcard=new WidgetCard(w->mwCLient->centralWidget());
        connect(wcard,wcard->clicked,this,this->handleHandsClick);
        wcard->setCard(card);
        wcard->show();
        hands.append(wcard);
    }
    setHands(hands.size());
    arrangeCards();
}

void WidgetBottom::removeHand(CCard *card)
{
    WidgetCard* wcard;
    foreach (wcard, hands) {
        if(wcard->card==card)
        {
            hands.removeOne(wcard);
            CCheckPointer::deleteObject(wcard);
            arrangeCards();
            setHands(hands.size());
            return;
        }
    }
}

void WidgetBottom::addHand(CCard *card)
{
    WidgetCard *wcard=new WidgetCard(w->mwCLient->centralWidget());
    connect(wcard,wcard->clicked,this,this->handleHandsClick);
    wcard->setCard(card);
    hands.append(wcard);
    wcard->show();
    setHands(hands.size());
    arrangeCards();
}

void WidgetBottom::arrangeCards()
{
    if(hands.isEmpty()) return;
    int space,start,end;
    start=164;
    end=width()-278;
    space=93;
    if(hands.size()>1)
    {
        space=(end-start)/(hands.size()-1);
        if(space>93) space=93;
    }
    int y;
    QPoint point;
    for(int i=0;i<hands.size();i++)
    {
        if(hands[i]->isOn)
            y=0;
        else
            y=33;
        point=mapTo(w->mwCLient->centralWidget(),QPoint(start+i*space,y));
        hands[i]->move(point);
    }
}

void WidgetBottom::setCardSelectable()
{
    WidgetCard* wcard;
    bool b;
    foreach (wcard, hands) {
        if(handSelectable)
            b=wcard->card->type->canUse(me,wcard->card);
        else
            b=false;
        wcard->setSelectable(b);
    }
    //todo:装备
}

void WidgetBottom::handsAllOff()
{
    WidgetCard *wcard;
    foreach (wcard, hands) {
        wcard->isOn=false;
    }
    selectedCards=0;
    WidgetGame *game=w->mwCLient->ui->widgetGame;
    game->playerSelectionNormal();
    game->ui->pushButtonOK->setEnabled(false);
    game->ui->pushButtonCancel->setEnabled(false);
    arrangeCards();
}

void WidgetBottom::handleHandsClick()
{
    WidgetCard *wcard=(WidgetCard *)sender();
    if(!wcard->selectable) return;
    CCard *&currentCard=w->client->game->currentCard;
    int &mode=w->client->game->mode;
    WidgetGame *game=w->mwCLient->ui->widgetGame;
    if(maxSelection<=selectedCards)
    {
        if(wcard->isOn)
        {
            wcard->isOn=false;
            currentCard=nullptr;
            selectedCards--;
            if(mode==UIMODE_USECARD)
            {
                game->playerSelectionNormal();
                game->ui->pushButtonOK->setEnabled(false);
                game->ui->pushButtonCancel->setEnabled(false);
            }
        }
        else if(maxSelection==1)
        {
            WidgetCard *wc;
            foreach (wc, hands) {
                if(wc->isOn)
                {
                    wc->isOn=false;
                    break;
                }
            }
            wcard->isOn=true;
            if(mode==UIMODE_USECARD) w->client->game->cardDirection(wcard->card);
        }
        else
            return;
    }
    else
    {
        if(wcard->isOn)
        {
            wcard->isOn=false;
            selectedCards--;
        }
        else
        {
            wcard->isOn=true;
            selectedCards++;
            if(mode==UIMODE_USECARD) w->client->game->cardDirection(wcard->card);
        }
    }
    if(mode==UIMODE_PLAYCARD)
    {
        if(maxSelection==selectedCards)
        {
            w->mwCLient->ui->widgetGame->ui->pushButtonOK->setEnabled(true);
        }
        else
            w->mwCLient->ui->widgetGame->ui->pushButtonOK->setEnabled(false);
    }
    arrangeCards();
}

void WidgetBottom::playPhaseInit()
{
    handSelectable=true;
    zhuangBeiSelectable=false;
    maxSelection=1;
    w->client->game->mode=UIMODE_USECARD;
    setCardSelectable();
    WidgetGame *game=w->mwCLient->ui->widgetGame;
    game->ui->pushButtonOK->setEnabled(false);
    game->ui->pushButtonCancel->setEnabled(false);
    game->ui->pushButtonEnd->setEnabled(true);
}

void WidgetBottom::endSelect()//结束选择模式
{
    WidgetCard *wcard;
    foreach (wcard, hands) {
        wcard->isOn=false;
        wcard->setSelectable(false);
    }
    //todo:装备
    selectedCards=0;
    arrangeCards();
    WidgetGame *game=w->mwCLient->ui->widgetGame;
    game->ui->pushButtonOK->setEnabled(false);
    game->ui->pushButtonCancel->setEnabled(false);
    game->ui->pushButtonEnd->setEnabled(false);
    game->playerSelectionNormal();
}

void WidgetBottom::setSelectable(bool b)
{
    if(b)
    {
        avatarOpacity=false;
    }
    else
    {
        avatarOpacity=true;
    }
    update();
}

void WidgetBottom::setSelected(bool b)
{
    if(b)
    {
        setSelectable(true);
        ui->labelSelected->setVisible(true);
    }
    else
        ui->labelSelected->setVisible(false);
}

void WidgetBottom::setNoSelect()
{
    setSelected(false);
    avatarOpacity=false;
    me->selectable=false;
    update();
}

void WidgetBottom::mousePressEvent(QMouseEvent *ev)
{
    if(!me||!me->selectable) return;
    int start,end;
    start=width()-163;
    end=width()-29;
    if(ev->x()>=start&&ev->x()<=end&&ev->y()>=10&&ev->y()<=144)
    {
        w->client->game->targetClicked(me);
    }
}

void WidgetBottom::setSelectMode(quint8 suit, quint16 type, const QList<quint8> type2, int number, int cardMode)
{
    endSelect();
    w->mwCLient->ui->widgetGame->ui->pushButtonEnd->setEnabled(false);
    w->mwCLient->ui->widgetGame->ui->pushButtonCancel->setEnabled(true);
    w->mwCLient->ui->widgetGame->ui->pushButtonOK->setEnabled(false);
    w->client->game->mode=UIMODE_PLAYCARD;
    WidgetCard *wcard;
    if(cardMode&CARDMODE_HANDS)
    {
        handSelectable=true;
        foreach (wcard, hands) {
            if((suit&wcard->card->suit)&&((wcard->card->type->type1&type)||type2.contains(wcard->card->type->type2)))
                wcard->setSelectable(true);
            else
                wcard->setSelectable(false);
        }
    }
    else
    {
        handSelectable=false;
        foreach (wcard, hands) {
            wcard->setSelectable(false);
        }
    }
    if(cardMode&CARDMODE_ZHUANGBEI)
    {
        zhuangBeiSelectable=true;
    }
    else
    {
        zhuangBeiSelectable=false;
    }
    maxSelection=number;
    selectedCards=0;
}

QList<QVariant> WidgetBottom::playCardsList()
{
    QList<QVariant> list;
    list.append(maxSelection);
    WidgetCard *wcard;
    foreach (wcard, hands) {
        if(wcard->isOn)
            list.append(wcard->card->id);
    }
    //todo:装备
    return list;
}
