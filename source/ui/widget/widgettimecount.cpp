#include "widgettimecount.h"
#include "ui_widgettimecount.h"
#include "ui/widget/widgetgameplayer.h"
#include "mainwindow.h"
extern MainWindow *w;
#include "network/cclient.h"
#include "game/cplayerclientme.h"
#include "game/cgameclient.h"

WidgetTimeCount::WidgetTimeCount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTimeCount)
{
    ui->setupUi(this);
    timer.setTimerType(Qt::PreciseTimer);
    connect(&timer,timer.timeout,this,this->handleTimeout);
    ui->progressBarRed->setVisible(false);
    ui->progressBarYellow->setVisible(false);
    player=nullptr;
}

WidgetTimeCount::WidgetTimeCount(WidgetGamePlayer *player, QWidget *parent) : WidgetTimeCount(parent)
{
    this->player=player;
}

WidgetTimeCount::~WidgetTimeCount()
{
    delete ui;
}

void WidgetTimeCount::startTimeCount(int time,bool useExtreTime)
{
    ui->progressBarRed->setMaximum(time*1000);
    ui->progressBarRed->setValue(time*1000);
    ui->progressBarRed->setVisible(true);
    if(useExtreTime&&w->client->extreTime)
    {
        ui->progressBarYellow->setMaximum(w->client->extreTime*1000);
        uint et;
        if(player)
            et=player->player->extreTime;
        else
        {
            CGameClient *game=(CGameClient*)w->client->game;
            et=game->me->extreTime;
        }
        ui->progressBarYellow->setValue(et);
        ui->progressBarYellow->setVisible(true);
    }
    else
        ui->progressBarYellow->setVisible(false);
    this->useExtreTime=useExtreTime;
    this->setVisible(true);
    timer.start(100);
}

void WidgetTimeCount::stop()
{
    timer.stop();
    ui->progressBarRed->setVisible(false);
    ui->progressBarYellow->setVisible(false);
}

void WidgetTimeCount::handleTimeout()
{
    int i=ui->progressBarRed->value();
    if(i)
    {
        i-=100;
        if(i<0) i=0;
        ui->progressBarRed->setValue(i);
    }
    else
    {
        if(useExtreTime)
        {
            i=ui->progressBarYellow->value();
            if(i)
            {
                i-=100;
                if(i<0) i=0;
                ui->progressBarYellow->setValue(i);
                return;
            }
        }
        timer.stop();
    }
}
