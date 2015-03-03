#include "mainwindowclient.h"
#include "ui_mainwindowclient.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "mainwindowserver.h"
#include <qmessagebox.h>
#include "network/cserver.h"
#include "network/cclient.h"

MainWindowClient::MainWindowClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowClient)
{
    dialogChooseRoom=nullptr;

    ui->setupUi(this);
    ui->splitterMain->setStretchFactor(0,1);
    ui->splitterMain->setStretchFactor(1,0);
    ui->splitterRight->setStretchFactor(0,2);
    ui->splitterRight->setStretchFactor(1,1);

}

MainWindowClient::~MainWindowClient()
{
    delete ui;
}

void MainWindowClient::init()
{
    ui->widgetGame->setVisible(false);
    ui->widgetRoom->setVisible(false);
    ui->widgetBottom->setVisible(false);
}

int MainWindowClient::closeClient(QWidget *widget)
{
    int r;
    if(w->server)
    {
        r=QMessageBox::question(widget,"确实要退出吗？","服务器正在运行中，要退出服务器吗？",
                                QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(r==QMessageBox::Yes)
        {
            w->server->deleteLater();
            w->server=nullptr;
            w->client->end2();
        }
        else if(r==QMessageBox::No)
        {
            w->client->end2();
        }
        else
            return QMessageBox::Cancel;
    }
    else
    {
        r=QMessageBox::question(widget,"确认","确实要退出吗？",
                                QMessageBox::Yes|QMessageBox::Cancel);
        if(r==QMessageBox::Yes)
        {
            w->client->end2();
        }
        else
            return QMessageBox::Cancel;
    }
    return r;
}

void MainWindowClient::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
    closeClient(this);
}
