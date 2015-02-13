#include "cserver.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
extern MainWindow *w;

#include "network/define.h"
#include <qmessagebox.h>


CServer::CServer()
{
    numberOfSockets=0;
    //初始化服务器设置
    serverName=w->ui->lineEditServerName->text();
    numberOfPlayer=w->ui->spinBoxPlayers->value();
    choices=w->ui->spinBoxChoices->value();
    choicesZG=w->ui->spinBoxChoicesZG->value();
    choicesNJ=w->ui->spinBoxChoicesNJ->value();
    maxRoom=w->ui->spinBoxRooms->value();
    shuangNei=w->ui->checkBoxShuangNei->isChecked();
    cardEX=w->ui->checkBoxEX->isChecked();
    cardJunZheng=w->ui->checkBoxJunZheng->isChecked();
    cardJieXianTuPo=w->ui->checkBoxJieXianTuPo->isChecked();
    generalPackage[0]=w->ui->checkBoxFeng->isChecked();
    generalPackage[1]=w->ui->checkBoxHuo->isChecked();
    generalPackage[2]=w->ui->checkBoxLin->isChecked();
    generalPackage[3]=w->ui->checkBoxShan->isChecked();
    generalPackage[4]=w->ui->checkBoxYiJiang->isChecked();
    generalPackage[5]=w->ui->checkBoxYiJiang2012->isChecked();
    generalPackage[6]=w->ui->checkBoxYiJiang2013->isChecked();
    generalPackage[7]=w->ui->checkBoxYiJiang2014->isChecked();
    generalPackage[8]=w->ui->checkBoxSP->isChecked();
    generalPackage[9]=w->ui->checkBoxXingSP->isChecked();
    generalPackage[10]=w->ui->checkBoxOL->isChecked();
    generalPackage[11]=w->ui->checkBoxJieXianTuPo->isChecked();
    bool modeBan,modeDefault;
    modeBan=true;
    modeDefault=true;
    for(int i=0;i<8;i++)
    {
        generalShen[i]=w->comboBoxShen[i]->currentIndex();
        if(modeBan&&generalShen[i])
        {
            modeBan=false;
        }
        if(modeDefault)
        {
            if(i<6)
            {
                if(generalShen[i]!=2) modeDefault=false;
            }
            else if(generalShen[i]!=1)
            {
                modeDefault=false;
            }
        }
    }
    if(w->ui->radioButtonServerNone->isChecked())
        auth=0;
    else if(w->ui->radioButtonServerPassword->isChecked())
    {
        auth=1;
        QByteArray ba;
        ba=QCryptographicHash::hash(w->ui->lineEditServerPassword->text().toUtf8(),QCryptographicHash::Sha1);
        memcpy(password,ba.constData(),20);
    }
    else
    {
        w->setupDB();
        querySelect.prepare("select count(*) from user where user=? and password=? limit 1");
        if(w->ui->checkBoxAllowRegister->isChecked())
        {
            auth=2;
            queryInsert.prepare("insert into user (username,password) values (?,?)");
        }
        else
            auth=3;
    }
    operationTimeout=w->ui->spinBoxOperationTimeout->value();
    wuXieTimeout=w->ui->spinBoxWuXieTimeout->value();
    extreTime=w->ui->spinBoxExtreTime->value();
    choiceTimeout=w->ui->spinBoxChoiceTimeout->value();
    maxOnlooker=w->ui->spinBoxMaxOnlooker->value();
    //准备“服务器简略信息”内容，以备后用
    getInfoBuf=serverName.toUtf8();
    unsigned char buf[4];
    buf[3]=getInfoBuf.length();
    buf[1]=numberOfPlayer<<4;
    if(shuangNei) buf[1]|=0b1000;
    buf[1]|=auth<<1;
    if(modeBan)//这地方buf[0]是没用的，只是用来当一下临时存储
        buf[0]=SHENMODE_BAN;
    else if(modeDefault)
        buf[0]=SHENMODE_DEFAULT;
    else
        buf[0]=SHENMODE_CUSTOM;
    *(quint16*)(buf+1)|=((quint16)buf[0])<<7;//设置神将模式的两个bit
    getInfoBuf.prepend((char*)buf+1,3);
    //准备getInfoError，以备后用
    versionError=VERSION_STRING;
    buf[0]=NETWORK_ERROR_VERSION;
    buf[1]=versionError.length();
    versionError.prepend((char*)buf,2);
    //准备authError
    authmodeError.resize(2);
    authmodeError[0]=CONNECT_ERROR_AUTHMODE;
    authmodeError[1]=auth>=2?2:auth;
    //启动服务器
    port=w->ui->spinBoxPort->value();
    server=new QTcpServer();
    connect(server,server->newConnection,this,this->handleNewConnection);
    server->listen(QHostAddress::Any,port);
    if(w->ui->checkBoxUPNP->isChecked())
    {
        upnp=new QtUpnpPortMapping;
        connect(upnp,upnp->finished,this,this->handleUpnpFinished);
        upnp->addPortMapping(port,port,"FairSGS");
        upnp->addPortMapping(port,port,"FairSGS",false);
    }
    else
    {
        upnp=NULL;
        firstReg();
    }
}

CServer::~CServer()
{
    if(upnp)
    {
        upnp->deleteLater();
    }
}

void CServer::regServer()
{
    reply=w->network->get(QNetworkRequest(QUrl(regUrl)));
    connect(reply,reply->finished,this,this->handleReplyFinished);
}

void CServer::handleReplyFinished()
{
    char buf;
    regSuccessed=false;
    if(reply->bytesAvailable()==1)
    {
        reply->read(&buf,1);
        if(buf=='1')
        {
            QMessageBox::about(NULL,"加入“查找服务器”列表失败","失败原因：外网无法访问此服务器。");
        }
        else if(buf=='0')
        {
            regSuccessed=true;
            QTimer::singleShot(3600000,Qt::VeryCoarseTimer,this,this->regServer);
        }
        else
            QMessageBox::about(NULL,"加入“查找服务器”列表失败","失败原因：列表服务器异常。");
    }
    else
        QMessageBox::about(NULL,"加入“查找服务器”列表失败","失败原因：列表服务器异常。");
    reply->deleteLater();
}

void CServer::firstReg()
{
    if(w->ui->checkBoxJoinList->isChecked())
    {
        QString s;
        s=w->settings->value("slconfig/regurl",URL_DEFAULTREG).toString();
        s+="reg.php?p="+QString::number(port);
        regUrl=s.toLocal8Bit();
        regServer();
    }
    else
    {
        regSuccessed=false;
    }
}

void CServer::handleUpnpFinished()
{
    disconnect(upnp,0,0,0);
    firstReg();
}

void CServer::handleNewConnection()
{
    numberOfSockets++;
    QTcpSocket *socket=server->nextPendingConnection();
    CPlayerSocket *handle=new CPlayerSocket(socket,this);
    sockets.append(handle);
}

void CServer::removeSocket(CPlayerSocket *socket)
{
    sockets.removeOne(socket);
}
