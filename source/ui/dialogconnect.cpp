#include "dialogconnect.h"
#include "ui_dialogconnect.h"
#include <qmovie.h>

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "network/cclient.h"

DialogConnect::DialogConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnect)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    QPoint p=parent->pos();
    p.rx()+=(parent->width()-this->width())/2;
    p.ry()+=(parent->height()-this->height())/2;
    move(p);
    setWindowOpacity(0.8);
}

DialogConnect::~DialogConnect()
{
    delete ui;
}

void DialogConnect::on_pushButtonCancel_clicked()
{
    end();
    w->client->end();
}

void DialogConnect::end()
{
    emit w->wLoadSource->stopMovie(w->wLoadSource->movieLoading);
    this->hide();
}

void DialogConnect::init()
{
    emit w->wLoadSource->newWork(WORKTYPE_MOVIE,w->wLoadSource->movieLoading,ui->labelLoading);
}
