#include "widgetroomplayer.h"
#include "ui_widgetroomplayer.h"
#include "thread/workerloadsource.h"
#include "mainwindow.h"
extern MainWindow *w;

WidgetRoomPlayer::WidgetRoomPlayer(quint8 pos, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetRoomPlayer)
{
    ui->setupUi(this);
    this->pos=pos;
}

WidgetRoomPlayer::~WidgetRoomPlayer()
{
    delete ui;
}

void WidgetRoomPlayer::setFavorite(quint16 favor)
{
    this->favorite=favor;
    w->imageLoad(w->wLoadSource->generalCards[favor],ui->labelPicture);
}

void WidgetRoomPlayer::init()
{
    empty=true;
    this->roomHost=false;
    this->admin=false;
    this->ready=false;
    this->udpAvailable=false;
    ui->labelAdmin->setVisible(false);
    ui->labelRoomHost->setVisible(false);
    ui->labelReady->setVisible(false);
    ui->labelPicture->clear();
    ui->labelNick->clear();
}

void WidgetRoomPlayer::setRoomHost()
{
    roomHost=true;
    ui->labelRoomHost->setVisible(true);
}

void WidgetRoomPlayer::setAdmin()
{
    admin=true;
    ui->labelAdmin->setVisible(true);
}

void WidgetRoomPlayer::setReady(bool b)
{
    ready=b;
    ui->labelReady->setVisible(b);
}

void WidgetRoomPlayer::setNick(const QString &n)
{
    this->nick=n;
    ui->labelNick->setText(n);
}
