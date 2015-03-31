#include "dialogregister.h"
#include "ui_dialogregister.h"

#include "mainwindow.h"
extern MainWindow *w;
#include <qmessagebox.h>
#include "network/cclient.h"
#include "network/define.h"
#include "ui/mainwindow/mainwindowserver.h"

DialogRegister::DialogRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRegister)
{
    ui->setupUi(this);

    this->setFixedSize(267,188);
    timer.setSingleShot(true);
    connect(&timer,timer.timeout,this,this->handleTimeout);
}

DialogRegister::~DialogRegister()
{
    delete ui;
}

void DialogRegister::init(int reg)
{
    this->reg=reg;
    ui->lineEditPassword->clear();
    ui->lineEditRepeat->clear();
    ui->lineEditUser->clear();
    if(reg)
    {
        ui->checkBoxRemember->hide();
        ui->lineEditRepeat->show();
    }
    else
    {
        ui->checkBoxRemember->show();
        ui->lineEditRepeat->hide();
        bool b=w->settings->value("register/remember",false).toBool();
        ui->checkBoxRemember->setChecked(b);
        if(b)
        {
            ui->lineEditUser->setText(w->settings->value("register/user").toString());
            savedPassword=w->settings->value("register/password").toByteArray();
            if(savedPassword.length()!=20)
                savedPassword.clear();
            else
                ui->lineEditPassword->setText("这段文字只是用来占位的");
        }
        else
            savedPassword.clear();
        ok=false;
    }
}

void DialogRegister::on_lineEditPassword_textChanged(const QString &)
{
    savedPassword.clear();
}

void DialogRegister::on_pushButtonOK_clicked()
{
    QString user=ui->lineEditUser->text();
    if(user.isEmpty()||user.contains(' ')||user.contains("　"))
    {
        QMessageBox::warning(this,"错误","用户名不能包含空格。");
        return;
    }
    if(user.toUtf8().length()>30)
    {
        QMessageBox::warning(this,"错误","用户名的长度不能超过30字节。");
        return;
    }
    if(reg==1)
    {
        if(ui->lineEditPassword->text().isEmpty()||ui->lineEditPassword->text()!=ui->lineEditRepeat->text())
        {
            QMessageBox::warning(this,"错误","两次输入的密码不相同。");
            return;
        }
        socket=new QTcpSocket(this);
        connect(socket,socket->connected,this,this->handleConnected);
        connect(socket,socket->readyRead,this,this->handleRead);
        connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
        timer.start(10000);
        this->user=user.toUtf8();
        password=ui->lineEditPassword->text().toUtf8();
        password=QCryptographicHash::hash(this->user+password+"FairSGS",QCryptographicHash::Sha1);
        socket->connectToHost(ip,port);
        ui->pushButtonOK->setEnabled(false);
    }
    else if(reg==2)
    {

    }
    else
    {
        if(savedPassword.length()==20)
            password=savedPassword;
        else
        {
            if(ui->lineEditPassword->text().isEmpty())
            {
                QMessageBox::warning(this,"错误","密码不能为空。");
                return;
            }
            password=ui->lineEditPassword->text().toUtf8();
            password=QCryptographicHash::hash(user.toUtf8()+password+"FairSGS",QCryptographicHash::Sha1);
        }
        ok=true;
        this->close();
        w->client=new CClient(ip,port,2,password,user);
    }
}

void DialogRegister::closeEvent(QCloseEvent *)
{
    if(reg)
    {
        if(!ui->pushButtonOK->isEnabled())
        {
            regOver();
        }
    }
    else if(!ok)
    {
        w->hide();
        w->mwServer->showMaximized();
    }
}

void DialogRegister::handleError(QAbstractSocket::SocketError)
{
    regOver();
    QMessageBox::warning(this,"注册失败","连接错误。");
}

void DialogRegister::handleTimeout()
{
    regOver();
    QMessageBox::warning(this,"注册失败","连接超时。");
}

void DialogRegister::handleConnected()
{
    timer.start(10000);
    QByteArray sendbuf;
    sendbuf.resize(4);
    quint16 id=VALIDATIONNUMBER;
    id=qToLittleEndian(id);
    memcpy(sendbuf.data(),&id,2);
    sendbuf[2]=ACTION_REGISTER;
    sendbuf[3]=user.length();
    sendbuf.append(user);
    sendbuf.append(password);
    socket->write(sendbuf);
}

#define COMPARE(a) if(a){regOver();QMessageBox::warning(this,"注册失败","服务器通信格式错误。");this->close();return;}

void DialogRegister::handleRead()
{
    QByteArray ba;
    ba=socket->read(1);
    COMPARE(ba.length()!=1);
    if(ba[0]==NETWORK_ERROR_VERSION)
    {
        ba=socket->read(1);
        COMPARE(ba.length()!=1||ba[0]<=(char)0);
        int i=ba[0];
        ba=socket->read(i);
        COMPARE(ba.length()!=i);
        QString s=QString::fromUtf8(ba);
        regOver();
        QMessageBox::warning(this,"注册失败","版本不符。\n服务器的版本为："+s);
        this->close();
        return;
    }
    COMPARE(ba[0]!=NETWORK_OK);
    ba=socket->read(1);
    COMPARE(ba.length()!=1);
    regOver();
    switch (ba[0]) {
    case REGISTER_ERROR_NOTALLOW:
        QMessageBox::warning(this,"注册失败","服务器不允许注册。");
        this->close();
        break;
    case REGISTER_ERROR_DUPLICATION:
        QMessageBox::warning(this,"注册失败","用户名已被注册。");
        break;
    case REGISTER_OK:
        QMessageBox::about(this,"提示","注册成功！");
        this->close();
        break;
    default:
        QMessageBox::warning(this,"注册失败","服务器通信格式错误。");
        this->close();
        break;
    }
}

void DialogRegister::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogRegister::regOver()
{
    socket->deleteLater();
    timer.stop();
    ui->pushButtonOK->setEnabled(true);
}
