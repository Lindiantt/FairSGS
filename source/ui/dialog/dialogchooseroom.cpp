#include "dialogchooseroom.h"
#include "ui_dialogchooseroom.h"
#include "pch.h"
#include <qmessagebox.h>
#include <QCloseEvent>
#include "ui/mainwindow/mainwindowclient.h"
#include "mainwindow.h"
extern MainWindow *w;
#include "network/cclient.h"
#include "network/define.h"

#define COMPARE(a,b) if((a)!=(b)){w->client->end2("与服务器断开连接。");return false;}

DialogChooseRoom::DialogChooseRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseRoom)
{
    ui->setupUi(this);
    QStringList sl;
    sl<<"房间号"<<"人数"<<"状态"<<"旁观";
    ui->tableWidgetRooms->setHorizontalHeaderLabels(sl);
}

DialogChooseRoom::~DialogChooseRoom()
{
    delete ui;
}

bool DialogChooseRoom::init(uint page,QByteArray &ba)
{
    ui->tableWidgetRooms->clearContents();
    int l=ba[0];
    if(ba.length()!=l*6+1) return false;
    uint id;
    quint8 nop;
    QTableWidgetItem *item;
    QPushButton *button;
    QString s;
    for(int i=0;i<l;i++)
    {
        memcpy(&id,ba.data()+6*i+1,4);
        id=qFromLittleEndian(id);
        item=new QTableWidgetItem(QString::number(id));
        ui->tableWidgetRooms->setItem(i,0,item);
        nop=ba[6*i+5];
        item=new QTableWidgetItem(QString::number(nop)+"/"+QString::number(+w->client->numberOfPlayer));
        ui->tableWidgetRooms->setItem(i,1,item);
        if(ba[6*i+6])
            s="是";
        else
            s="否";
        item=new QTableWidgetItem(s);
        ui->tableWidgetRooms->setItem(i,2,item);
        button=new QPushButton("旁观");
        button->setObjectName(QString::number(id));
        connect(button,SIGNAL(clicked()),this,SLOT(handleOnlookButton()));
        ui->tableWidgetRooms->setCellWidget(i,3,button);
    }
    this->page=page;
    ui->labelPage->setText("第"+QString::number(page)+"页");
    if(page==1)
    {
        ui->pushButtonFirst->setEnabled(false);
        ui->pushButtonPrev->setEnabled(false);
    }
    else
    {
        ui->pushButtonFirst->setEnabled(true);
        ui->pushButtonPrev->setEnabled(true);
    }
    return true;
}

void DialogChooseRoom::handleOnlookButton()
{
    QString s=sender()->objectName();
    uint id=s.toUInt();
    onlookRoom(id);
}

void DialogChooseRoom::on_tableWidgetRooms_cellDoubleClicked(int row, int)
{
    uint id=ui->tableWidgetRooms->item(row,0)->text().toUInt();
    joinRoom(id);
}

void DialogChooseRoom::joinRoom(uint id)
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(7);
    ba[2]=ROOM_JOIN;
    this->id=id;
    id=qToLittleEndian(id);
    memcpy(ba.data()+3,&id,4);
    w->mwCLient->roomID=id;
    w->client->send(ba);
}

void DialogChooseRoom::onlookRoom(uint id)
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(7);
    ba[2]=ROOM_ONLOOK;
    id=qToLittleEndian(id);
    memcpy(ba.data()+3,&id,4);
    w->mwCLient->roomID=id;
    w->client->send(ba);
}

void DialogChooseRoom::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
    w->mwCLient->closeClient(this);
}

void DialogChooseRoom::on_pushButtonNewRoom_clicked()
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(3);
    ba[2]=ROOM_CREATE;
    w->client->send(ba);
}

void DialogChooseRoom::on_pushButtonFind_clicked()
{
    QString s=ui->lineEditRoomID->text();
    bool ok;
    uint u;
    u=s.toUInt(&ok);
    if(!ok)
    {
        QMessageBox::about(this,"错误","请输入正确的房间号。");
        return;
    }
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(7);
    ba[2]=ROOM_FIND;
    u=qToLittleEndian(u);
    memcpy(ba.data()+3,&u,4);
    w->client->send(ba);
}

void DialogChooseRoom::on_pushButtonRefresh_clicked()
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(7);
    ba[2]=ROOM_REFRESH;
    uint p=qToLittleEndian(page);
    memcpy(ba.data()+3,&p,4);
    w->client->send(ba);
}

void DialogChooseRoom::on_pushButtonFirst_clicked()
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(3);
    ba[2]=ROOM_PAGE_FIRST;
    w->client->send(ba);
}

void DialogChooseRoom::on_pushButtonLast_clicked()
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(3);
    ba[2]=ROOM_PAGE_LAST;
    w->client->send(ba);
}

void DialogChooseRoom::on_pushButtonPrev_clicked()
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(7);
    ba[2]=ROOM_PAGE_PREV;
    uint p=qToLittleEndian(page);
    memcpy(ba.data()+3,&p,4);
    w->client->send(ba);
}

void DialogChooseRoom::on_pushButtonNext_clicked()
{
    this->setEnabled(false);
    QByteArray ba;
    ba.resize(7);
    ba[2]=ROOM_PAGE_NEXT;
    uint p=qToLittleEndian(page);
    memcpy(ba.data()+3,&p,4);
    w->client->send(ba);
}

void DialogChooseRoom::on_pushButtonJoin_clicked()
{
    auto si=ui->tableWidgetRooms->selectedItems();
    if(si.isEmpty())
    {
        QMessageBox::about(this,"错误","请选择要加入的房间。");
        return;
    }
    QString s=ui->tableWidgetRooms->item(si[0]->row(),0)->text();
    uint u=s.toUInt();
    joinRoom(u);
}

void DialogChooseRoom::on_pushButtonOnlook_clicked()
{
    auto si=ui->tableWidgetRooms->selectedItems();
    if(si.isEmpty())
    {
        QMessageBox::about(this,"错误","请选择要旁观的房间。");
        return;
    }
    QString s=ui->tableWidgetRooms->item(si[0]->row(),0)->text();
    uint u=s.toUInt();
    onlookRoom(u);
}

bool DialogChooseRoom::rhChooseRoom(QByteArray &ba)
{
    this->setEnabled(true);
    switch (ba[0]) {
    case ROOM_JOIN_ERROR_INVALID:
        COMPARE(ba.length(),1)
        QMessageBox::about(this,"错误","房间不存在。");
        break;
    case ROOM_JOIN_ERROR_FULL:
        COMPARE(ba.length(),1)
        QMessageBox::about(this,"错误","房间已满，无法进入。");
        break;
    case ROOM_JOIN_ERROR_CANONLOOK:
    {
        COMPARE(ba.length(),1)
        int r=QMessageBox::question(this,"确认","房间已满或正在游戏中，要进入观战吗？");
        if(r==QMessageBox::Yes)
        {
            onlookRoom(id);
            return true;
        }
    }
        break;
    case ROOM_JOIN_OK:
        COMPARE(ba.length(),1);
        w->client->state=CLIENTSTATE_ENTERROOM;
        this->hide();
        break;
    case ROOM_CREATE_ERROR_FULL:
        COMPARE(ba.length(),1)
        QMessageBox::about(this,"错误","房间数已达上限。");
        break;
    case ROOM_CREATE_OK:
    {
        COMPARE(ba.length(),5);
        uint u;
        memcpy(&u,ba.data()+1,4);
        u=qFromLittleEndian(u);
        w->mwCLient->roomID=u;
    }
        break;
    case ROOM_ONLOOK_ERROR_INVALID:
        COMPARE(ba.length(),1)
        QMessageBox::about(this,"错误","房间不存在。");
        break;
    case ROOM_ONLOOK_ERROR_FULL:
        COMPARE(ba.length(),1)
        QMessageBox::about(this,"错误","房间已满，无法进入观战。");
        break;
    case ROOM_ONLOOK_OK:
        COMPARE(ba.length(),1);
        w->client->state=CLIENTSTATE_ONLOOKROOM;
        this->hide();
        break;
    case ROOM_FIND_ERROR:
        COMPARE(ba.length(),1)
        QMessageBox::about(this,"错误","房间号不存在。");
        break;
    case ROOM_FIND_OK:
        COMPARE(ba.length(),3)
    {
        QByteArray buf;
        buf.resize(7);
        buf[0]=1;
        uint id=qToLittleEndian(this->id);
        memcpy(buf.data()+1,&id,4);
        memcpy(buf.data()+5,ba.data()+1,2);
        init(1,buf);
    }
        break;
    case ROOM_PAGE_ERROR:
        COMPARE(ba.length(),1)
        QMessageBox::about(this,"错误","翻页失败。");
        break;
    case ROOM_PAGE_OK:
    case ROOM_REFRESH_OK:
        if(ba.length()<6) COMPARE(1,0);
        memcpy(&page,ba.data()+1,4);
        page=qFromLittleEndian(page);
        ba.remove(0,5);
        if(!init(page,ba)) COMPARE(1,0);
        break;
    default:
        break;
    }
    return true;
}
