#include "mainwindowserver.h"
#include "ui_mainwindowserver.h"


#include "mainwindow.h"
extern MainWindow *w;
#include "network/cserver.h"

MainWindowServer::MainWindowServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowServer)
{
    ui->setupUi(this);
}

MainWindowServer::~MainWindowServer()
{
    delete ui;
}

void MainWindowServer::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
    hideAll();
    w->server->deleteLater();
    w->server=nullptr;
    w->show();
}

void MainWindowServer::hideAll()
{
    this->hide();
}
