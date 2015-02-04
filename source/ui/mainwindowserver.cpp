#include "mainwindowserver.h"
#include "ui_mainwindowserver.h"

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
