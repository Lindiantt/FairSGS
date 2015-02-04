#include "mainwindowserverlist.h"
#include "ui_mainwindowserverlist.h"
#include "network/cslsockethandle.h"

MainWindowServerList::MainWindowServerList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowServerList)
{
    ui->setupUi(this);

    ui->tableWidgetServerList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetServerList->setColumnCount(8);
    ui->tableWidgetServerList->setRowCount(0);
    QStringList sl;
    sl<<"地址"<<"延时"<<"版本兼容"<<"模式"<<"人数"<<"验证方式"<<"服务器名"<<"神将";
    ui->tableWidgetServerList->setHorizontalHeaderLabels(sl);
}

MainWindowServerList::~MainWindowServerList()
{
    delete ui;
}

MainWindowServerList::initWindow()
{

}

MainWindowServerList::addAddress(QHostAddress &address,quint16 port)
{
    int r=ui->tableWidgetServerList->rowCount();
    ui->tableWidgetServerList->setRowCount(r+1);
    QTableWidgetItem* item,first;
    QString s=address.toString()+":"+QString::number(port);
    first=new QTableWidgetItem(s);
    CSLSocketHandle* handle=new CSLSocketHandle(address,port,first);
    first.setData(0,(long)handle);
    ui->tableWidgetServerList->setItem(r,0,first);
    for(int i=1;i<6;i++)
    {
        item=new QTableWidgetItem("");
        ui->tableWidgetServerList->setItem(r,i,item);
    }
}
