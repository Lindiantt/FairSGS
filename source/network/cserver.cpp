#include "cserver.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
extern MainWindow *w;

#ifdef QT_DEBUG
#include <qmessagebox.h>
#endif

#include "cplayersocket.h"
#include "card/ccard.h"
#include "general/cgeneral.h"
#include "qtupnpportmapping.h"
#include "croom.h"
#include "ui/mainwindowserver.h"
#include "ui_mainwindowserver.h"
#include "cclient.h"

CServer::CServer()
{
    numberOfSockets=0;
    logCount=0;
    lastRoomID=0;
    server=nullptr;
    reply=nullptr;
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
    int i;
    for(i=0;i<8;i++)
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
    //准备importantInfo
    importantInfo.resize(11);
    uint mr=qToLittleEndian(maxRoom);
    memcpy(importantInfo.data(),&mr,4);
    importantInfo[4]=numberOfPlayer;
    importantInfo[5]=shuangNei?1:0;
    importantInfo[6]=operationTimeout;
    importantInfo[7]=wuXieTimeout;
    importantInfo[8]=extreTime;
    importantInfo[9]=choiceTimeout;
    importantInfo[10]=allowChat?1:0;
    //卡牌初始化
    availableCards=w->cardBiao;
    if(cardEX) availableCards.append(w->cardEx);
    if(cardJunZheng) availableCards.append(w->cardJunzheng);
    if(cardJieXianTuPo)
    {
        availableCards[84]=w->cardJiexiantupo[0];//木牛流马替代方片5闪
    }
    //武将初始化
    quint8 j;
    /*for(i=0;i<MAX_GENERAL;i++)
    {
        j=w->allGenerals[i]->package;
        if(generalPackage[j]&&!generalBan[j])
        {
            if(w->allGenerals[i]->zhuGong)
                availableZhugong.append(w->allGenerals[i]);
            else
                availableGenerals.append(w->allGenerals[i]);
        }
    }
    if(availableZhugong.length()+availableGenerals.length()<numberOfPlayer)
    {
        QMessageBox::about(nullptr,"错误","可用武将数过少。");
        this->deleteLater();
        return;
    }*/
    //身份初始化
    int n;
    if(numberOfPlayer>2)
    {
        if(shuangNei)
        {
            roles<<ROLE_NEI<<ROLE_NEI;
            numberOfNei=2;
        }
        else
        {
            roles<<ROLE_NEI;
            numberOfNei=1;
        }
        n=numberOfPlayer-roles.length()-1;
        if(n>0)
        {
            if(n%2)
            {
                n=(n-1)/2+1;
            }
            else
            {
                n=(n-2)/2+2;
            }
        }
    }
    else
    {
        n=1;
        numberOfNei=0;
    }
    for(i=0;i<n;i++)
    {
        roles.append(ROLE_FAN);
    }
    n=numberOfPlayer-roles.length()-1;
    for(i=0;i<n;i++)
    {
        roles.append(ROLE_ZHONG);
    }
    //启动服务器
    port=w->ui->spinBoxPort->value();
    server=new QTcpServer();
    connect(server,server->newConnection,this,this->handleNewConnection);
    if(!server->listen(QHostAddress::Any,port))
    {
        QMessageBox::about(nullptr,"错误","TCP端口绑定失败！");
        this->deleteLater();
        return;
    }
    if(!udp.bind(port))
    {
        int r=QMessageBox::question(nullptr,"错误","UDP端口绑定失败，用户将无法进行聊天和观战。\n确定要继续吗？");
        if(r!=QMessageBox::Yes)
        {
            this->deleteLater();
            return;
        }
    }
    else
        connect(&udp,udp.readyRead,this,this->handleUdpRead);
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
    //显示窗口
    if(!w->mwServer)
    {
        w->mwServer=new MainWindowServer();
    }
    if(w->ui->checkBoxJoinGame->isChecked())
    {
        QByteArray ba(password,20);
        w->client=new CClient("127.0.0.1",port,auth>=2?2:auth,ba,"admin");
    }
    else
    {
        w->hide();
        w->mwServer->showMaximized();
    }
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
}

CServer::~CServer()
{
    if(server) server->deleteLater();
    if(reply) reply->deleteLater();
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
    static bool firstTip=true;
    if(reply->bytesAvailable()==1)
    {
        reply->read(&buf,1);
        if(buf=='1')
        {
            QMessageBox::about(NULL,"加入“查找服务器”列表失败","失败原因：外网无法访问此服务器。");
        }
        else if(buf=='0')
        {
            if(firstTip)
            {
                firstTip=false;
                QMessageBox::about(NULL,"提示","加入“查找服务器”列表成功！");
            }
            regSuccessed=true;
            QTimer::singleShot(3600000,Qt::VeryCoarseTimer,this,this->regServer);
        }
        else
            QMessageBox::about(NULL,"加入“查找服务器”列表失败","失败原因：列表服务器异常。");
    }
    else
        QMessageBox::about(NULL,"加入“查找服务器”列表失败","失败原因：列表服务器异常。");
    reply->deleteLater();
    reply=nullptr;
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
    QTcpSocket *socket=server->nextPendingConnection();
    new CPlayerSocket(socket,this);
    log("新连接："+socket->peerAddress().toString());
}

void CServer::removeSocket(CPlayerSocket *socket)
{
    sockets.removeOne(socket);
}

void CServer::log(const QString &content)
{
    logCount++;
    if(logCount>=100000)
    {
        logCount=0;
        w->mwServer->ui->plainTextEditServer->clear();
    }
    w->mwServer->ui->plainTextEditServer->appendPlainText(content);
}

CRoom* CServer::createRoom()
{
    if(rooms.size()>=this->maxRoom) return nullptr;
    CRoom *room=new CRoom(this);
    room->id=lastRoomID;
    rooms.insert(lastRoomID,room);
    lastRoomID++;
    return room;
}

uint CServer::lastRoomPage()
{
    int l=rooms.size();
    if(!l) return 0;
    l=(l-1)/20;
    return l+1;
}

void CServer::handleUdpRead()
{
    QByteArray ba;
    QHostAddress ha;
    quint16 port;
    int i;
    while(udp.hasPendingDatagrams())
    {
        ba.resize(udp.pendingDatagramSize());
        udp.readDatagram(ba.data(),ba.length(),&ha,&port);
        if(ba.length()!=4) continue;
        memcmp(&i,ba.data(),4);
        auto it=udpVals.find(i);
        while(it!=udpVals.end()&&it.key()==i)
        {
            CPlayerSocket *player=it.value();
            if(player->hostAddress==ha)
            {
                player->setUdpPort(port);
                udpVals.erase(it);
                break;
            }
            it++;
        }
    }
}
