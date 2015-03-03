#include "mainwindowserverlist.h"
#include "ui_mainwindowserverlist.h"
#include "network/cslsockethandle.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
extern MainWindow *w;

#include "network/cserverlist.h"
#include "dialogslsettings.h"

MainWindowServerList::MainWindowServerList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowServerList)
{
    dialogSLSettings=NULL;
    socketCount=0;

    ui->setupUi(this);

    ui->tableWidgetServerList->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList sl;
    sl<<"地址"<<"延时"<<"版本兼容"<<"模式"<<"人数"<<"验证方式"<<"服务器名"<<"神将";
    ui->tableWidgetServerList->setHorizontalHeaderLabels(sl);

    serverList=new CServerList;

    QAction *actionChoose,*actionGetInfo;
    actionChoose=new QAction(&rcMenu);
    actionGetInfo=new QAction(&rcMenu);
    actionGetInfo->setText("获取简略信息");
    actionChoose->setText("选定");
    rcMenu.addAction(actionGetInfo);
    rcMenu.addAction(actionChoose);
    connect(ui->tableWidgetServerList,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(handleCustomMenu(QPoint)));
    connect(actionChoose,SIGNAL(triggered()),this,SLOT(handleChoose()));
    connect(actionGetInfo,SIGNAL(triggered()),this,SLOT(handleGetInfo()));

    ui->tableWidgetServerList->setColumnWidth(0,120);
    ui->tableWidgetServerList->setColumnWidth(1,60);
    ui->tableWidgetServerList->setColumnWidth(3,74);
    ui->tableWidgetServerList->setColumnWidth(4,35);
    ui->tableWidgetServerList->setColumnWidth(6,434);
    ui->tableWidgetServerList->setColumnWidth(7,58);
}

MainWindowServerList::~MainWindowServerList()
{
    delete ui;
}

void MainWindowServerList::handleChoose()
{
    QList<QTableWidgetItem *> si;
    si=ui->tableWidgetServerList->selectedItems();
    if(si.size())
        on_tableWidgetServerList_cellDoubleClicked(si[0]->row(),0);
}

void MainWindowServerList::handleGetInfo()
{
    QList<QTableWidgetItem *> si;
    si=ui->tableWidgetServerList->selectedItems();
    if(si.size())
    {
        QString s=ui->tableWidgetServerList->item(si[0]->row(),1)->text();
        if(s=="")
        {
            CSLSocketHandle* handle;
            handle=(CSLSocketHandle*)ui->tableWidgetServerList->item(si[0]->row(),0)->data(Qt::UserRole).value<void*>();
            socketCount++;
            handle->getInfo();
        }
    }
}

void MainWindowServerList::handleCustomMenu(QPoint)
{
    QList<QTableWidgetItem *> si;
    si=ui->tableWidgetServerList->selectedItems();
    if(si.size())
        rcMenu.exec(QCursor::pos());
}

void MainWindowServerList::initWindow()
{
    if(!ui->tableWidgetServerList->rowCount())
    {
        serverList->requestList();
    }
}

void MainWindowServerList::addAddress(int r, QHostAddress &address, quint16 port)
{
    QTableWidgetItem *item,*first;
    QString s=address.toString()+":"+QString::number(port);
    first=new QTableWidgetItem(s);
    CSLSocketHandle* handle=new CSLSocketHandle(address,port,first);
    first->setData(Qt::UserRole,qVariantFromValue((void*)handle));
    ui->tableWidgetServerList->setItem(r,0,first);

    for(int i=1;i<6;i++)
    {
        item=new QTableWidgetItem("");
        ui->tableWidgetServerList->setItem(r,i,item);
    }
    bool b=w->settings->value("slconfig/autogetinfo",true).toBool();
    if(b&&socketCount<5)
    {
        socketCount++;
        handle->getInfo();
    }
}

void MainWindowServerList::clearServers()
{
    int l=ui->tableWidgetServerList->rowCount();
    QTableWidgetItem* item;
    CSLSocketHandle* handle;
    for(int i=0;i<l;i++)
    {
        item=ui->tableWidgetServerList->item(i,0);
        handle=(CSLSocketHandle*)item->data(Qt::UserRole).value<void*>();
        if(handle)
            delete handle;
    }
    ui->tableWidgetServerList->clearContents();
    ui->tableWidgetServerList->setRowCount(0);
}

void MainWindowServerList::socketFinished()
{
    if(!socketCount) return;
    bool b=w->settings->value("slconfig/autogetinfo",true).toBool();
    if(b)
    {
        QTableWidgetItem* item;
        CSLSocketHandle* handle;
        int i,l;
        l=ui->tableWidgetServerList->rowCount();
        for(i=0;i<l;i++)
        {
            item=ui->tableWidgetServerList->item(i,1);
            if(item->text()=="")
            {
                item=ui->tableWidgetServerList->item(i,0);
                handle=(CSLSocketHandle*)item->data(Qt::UserRole).value<void*>();
                handle->getInfo();
                return;
            }
        }
    }
    socketCount--;
}

void MainWindowServerList::on_actionGetMore_triggered()
{
    serverList->requestList();
}

void MainWindowServerList::on_actionRefresh_triggered()
{
    socketCount=0;
    serverList->refresh();
}

void MainWindowServerList::on_actionAdvance_triggered()
{
    if(!dialogSLSettings)
    {
        dialogSLSettings=new DialogSLSettings(this);
    }
    dialogSLSettings->initVar();
    dialogSLSettings->show();
}

void MainWindowServerList::on_tableWidgetServerList_cellDoubleClicked(int row, int)
{
    QString s;
    s=ui->tableWidgetServerList->item(row,0)->text();
    w->ui->comboBoxServerAddress->setCurrentText(s);
    s=ui->tableWidgetServerList->item(row,5)->text();
    if(s.length())
    {
        if(s.startsWith("密"))
        {
            w->ui->radioButtonPassword->setChecked(true);
        }
        else if(s.startsWith("用"))
        {
            w->ui->radioButtonUserAndPassword->setChecked(true);
        }
        else
        {
            w->ui->radioButtonNone->setChecked(true);
        }
    }
    this->close();
}
