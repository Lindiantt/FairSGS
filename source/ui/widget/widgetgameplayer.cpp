#include "widgetgameplayer.h"
#include "ui_widgetgameplayer.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include <QPainter>
#include "game/cplayerclient.h"
#include "general/cgeneral.h"
#include "ui/widget/widgettimecount.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "ui/widget/widgetgame.h"
#include "qsanguoshautil/uiUtils.h"
#include "ui/others/labelclickable.h"
#include "thread/ccheckpointer.h"
#include <QGraphicsOpacityEffect>
#include "network/cclient.h"
#include "game/cgameclient.h"

WidgetGamePlayer::WidgetGamePlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetGamePlayer)
{
    player=nullptr;
    ui->setupUi(this);
    CCheckPointer::addParent(this);
    w->imageLoad(w->wLoadSource->bgGamePlayer,this,this);
    w->imageLoad(w->wLoadSource->handCardNumber,ui->labelHands,this);
    w->imageLoad(w->wLoadSource->roleCorner[ROLE_ZHONG],ui->labelRoleZhong,this);
    w->imageLoad(w->wLoadSource->roleCorner[ROLE_FAN],ui->labelRoleFan,this);
    w->imageLoad(w->wLoadSource->roleCorner[ROLE_NEI],ui->labelRoleNei,this);
    w->imageLoad(w->wLoadSource->roleCorner[ROLE_UNKNOWN],ui->labelRole,this);
    ui->labelRoleZhong->setVisible(false);
    ui->labelRoleFan->setVisible(false);
    ui->labelRoleNei->setVisible(false);
    ui->widgetFangJu->setVisible(false);
    ui->widgetJianYiMa->setVisible(false);
    ui->widgetJianYiMaSmall->setVisible(false);
    ui->widgetJiaYiMa->setVisible(false);
    ui->widgetJiaYiMaSmall->setVisible(false);
    ui->widgetWuQi->setVisible(false);
    widgetTimeCount=new WidgetTimeCount(this,w->mwCLient->ui->widgetGame);
    labelPhase=new QLabel(w->mwCLient->ui->widgetGame);
    labelPhase->resize(140,12);
    labelPhase->setScaledContents(true);
    labelFrame=new QLabel(w->mwCLient->ui->widgetGame);
    labelFrame->resize(187,211);
    labelFrame->lower();
    opEffect=new QGraphicsOpacityEffect();
    opEffect->setOpacity(1.0);
    this->setGraphicsEffect(opEffect);
}

WidgetGamePlayer::~WidgetGamePlayer()
{
    delete ui;
    widgetTimeCount->deleteLater();
    labelFrame->deleteLater();
    labelPhase->deleteLater();
}

void WidgetGamePlayer::moveWidgets()
{
    QPoint point=this->mapTo(w->mwCLient->ui->widgetGame,QPoint(0,181));
    widgetTimeCount->move(point);
    labelFrame->move(this->mapTo(w->mwCLient->ui->widgetGame,QPoint(-15,-15)));
    labelPhase->move(this->mapTo(w->mwCLient->ui->widgetGame,QPoint(25,176)));
}

void WidgetGamePlayer::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    if(w->wLoadSource->ready&&w->wLoadSource->bgGamePlayer->loaded)
    {
        painter.fillRect(0,0,width(),height(),QColor(0,0,0));
        if(player&&player->general)
        {
            CImage *img=CImage::avatarById(player->general->id);
            if(img->loaded)
                painter.drawPixmap(0,10,img->pixmap.scaled(157,133));
            if(pixmapName.isNull())
                makePixmap();
        }
        painter.drawPixmap(0,0,w->wLoadSource->bgGamePlayer->pixmap.scaled(size()));
        if(player&&player->general)
        {
            if(w->wLoadSource->kingdomFrame[player->kingdom]->loaded)
                painter.drawPixmap(-2,15,w->wLoadSource->kingdomFrame[player->kingdom]->pixmap);
            painter.drawPixmap(5,24,pixmapName);
        }
    }
    QWidget::paintEvent(ev);
}

void WidgetGamePlayer::setPlayer(CPlayerClient *player)
{
    this->player=player;
}

void WidgetGamePlayer::makePixmap()
{
    QString name;
    if(player->general->displayName.isEmpty())
        name=player->general->name;
    else
        name=player->general->displayName;
    QImage imgText(18,60,QImage::Format_ARGB32);
    imgText.fill(QColor(0,0,0,0));
    QPainter painter(&imgText);
    QFont font("黑体");
    QColor color;
    int radius;
    int space;
    int fontsize;
    if(name.length()<=3)
    {
        fontsize=18;
        color.setRgb(50,50,50,200);
        radius=4;
        if(name.length()==2)
            space=30;
        else
            space=18;
    }
    else
    {
        fontsize=14;
        color.setRgb(50,50,50,150);
        radius=2;
        space=15;
    }
    font.setPixelSize(fontsize);
    painter.setFont(font);
    painter.setPen(QColor(255,255,255));
    for(int i=0;i<name.length();i++)
    {
        painter.drawText(0,i*space+fontsize,name.at(i));
    }
    QImage shadow=QSanUiUtils::produceShadow(imgText,color,radius,10);
    pixmapName=QPixmap::fromImage(shadow);
    QPainter pp(&pixmapName);
    pp.drawImage(0,0,imgText);
    //pixmapName=QPixmap::fromImage(imgText);
}

void WidgetGamePlayer::on_labelRole_clicked()
{
    if(player&&player->role!=ROLE_ZHU)
    {
        if(ui->labelRoleZhong->isVisible())
        {
            hideRole();
        }
        else
        {
            w->imageLoad(w->wLoadSource->roleCorner[ROLE_UNKNOWN],ui->labelRole,this);
            ui->labelRoleZhong->setVisible(true);
            ui->labelRoleFan->setVisible(true);
            ui->labelRoleNei->setVisible(true);
        }
    }
}

void WidgetGamePlayer::on_labelRoleZhong_clicked()
{
    hideRole();
    w->imageLoad(w->wLoadSource->roleCorner[ROLE_ZHONG],ui->labelRole,this);
}

void WidgetGamePlayer::on_labelRoleFan_clicked()
{
    hideRole();
    w->imageLoad(w->wLoadSource->roleCorner[ROLE_FAN],ui->labelRole,this);
}

void WidgetGamePlayer::on_labelRoleNei_clicked()
{
    hideRole();
    w->imageLoad(w->wLoadSource->roleCorner[ROLE_NEI],ui->labelRole,this);
}

void WidgetGamePlayer::hideRole()
{
    ui->labelRoleZhong->setVisible(false);
    ui->labelRoleFan->setVisible(false);
    ui->labelRoleNei->setVisible(false);
}

void WidgetGamePlayer::setMaxHp(quint8 hp)
{
    int bgw;
    if(hp>=5)
        bgw=4;
    else if(hp<=1)
        bgw=0;
    else
        bgw=hp-1;
    int wid;
    wid=24+bgw*14;
    ui->labelBloodBg->resize(wid,17);
    ui->labelBloodBg->move(163-wid,98);
    w->imageLoad(w->wLoadSource->bloodBack[bgw],ui->labelBloodBg,this);
    QLabel *label;
    foreach (label, labelBloods) {
        CCheckPointer::deleteObject(label);
    }
    if(hp<=5)
    {
        labelBloods.resize(hp);
        for(int i=0;i<hp;i++)
        {
            label=new QLabel(this);
            CCheckPointer::addParent(label);
            label->setScaledContents(true);
            label->setGeometry(163-wid+14*i,98,14,17);
            label->show();
            labelBloods[i]=label;
        }
    }
    else
    {
        labelBloods.resize(1);
        label=new QLabel(this);
        label->setGeometry(163-wid,98,wid,17);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color: rgb(147, 216, 124);");
        QFont font("Arial",12);
        font.setBold(true);
        label->setFont(font);
        label->show();
        labelBloods[0]=label;
    }
    ui->labelRoleNei->raise();
    setHp(player->HP);
}

void WidgetGamePlayer::setHp(signed char hp)
{
    if(player->maxHP>5)
    {
        labelBloods[0]->setText(QString::number(hp)+" / "+QString::number(player->maxHP));
    }
    else if(player->maxHP>=1)
    {
        int i;
        for(i=0;i<player->maxHP-hp;i++)
        {
            w->imageLoad(w->wLoadSource->blood[0],labelBloods[i],labelBloods[i]);
        }
        CImage* img;
        if(player->maxHP==hp)
            img=w->wLoadSource->blood[5];
        else if(hp>0)
        {
            img=w->wLoadSource->blood[hp];
        }
        else
            return;
        for(;i<player->maxHP;i++)
        {
            w->imageLoad(img,labelBloods[i],labelBloods[i]);
        }
    }
}

void WidgetGamePlayer::setHands(int n)
{
    ui->labelHandsNumber->setText(QString::number(n));
}

void WidgetGamePlayer::setSelected(bool b)
{
    if(b)
    {
        setSelectable(true);
        w->imageLoad(w->wLoadSource->framePlayerSelected,labelFrame);
        labelFrame->setVisible(true);
    }
    else
    {
        labelFrame->setVisible(false);
    }
}

void WidgetGamePlayer::setSelectable(bool b)
{
    if(b)
    {
        opEffect->setOpacity(1.0);
    }
    else
    {
        opEffect->setOpacity(0.7);
    }
    update();
}

void WidgetGamePlayer::setNoSelect()
{
    setSelected(false);
    player->selectable=false;
    opEffect->setOpacity(1.0);
    update();
}

void WidgetGamePlayer::mousePressEvent(QMouseEvent *)
{
    if(!player||!player->selectable) return;
    w->client->game->targetClicked(player);
}
