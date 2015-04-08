#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qmessagebox.h>
#include <QComboBox>
#include "qtoolbutton.h"

#include "card/ccard.h"
#include "network/cserver.h"
#include "ui/mainwindow/mainwindowserverlist.h"
#include "ui/dialog/dialogpersonalsettings.h"
#include "general/cgeneral.h"
#include "thread/workerloadsource.h"
#include "network/cclient.h"
#include "network/define.h"
#include "thread/cimage.h"
#include "ui/dialog/dialogregister.h"
#include "qfontdatabase.h"
#include "thread/cicon.h"
#include "thread/ccheckpointer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont("font/simli.ttf");
    QString s=QCoreApplication::applicationDirPath();
    settings=new QSettings(s+"/fairsgs.ini",QSettings::IniFormat);
    dialogPersonalSettings=NULL;
    dialogConnect=nullptr;
    mwServerList=NULL;
    mwCLient=nullptr;
    mwServer=nullptr;
    dialogRegister=nullptr;
    client=nullptr;
    server=nullptr;
    network=new QNetworkAccessManager();
    QThread *thread=new QThread();
    wLoadSource=new workerLoadSource();
    wLoadSource->moveToThread(thread);
    thread->start();
    emit wLoadSource->wakeUp();
    connect(this,this->runFunction,this,this->handleRun,Qt::QueuedConnection);
    qRegisterMetaType<std::function<void()>>("std::function<void()>");
    allGenerals=CGeneral::createAll();
    generalShen=CGeneral::createShen();
    CCard::init(cardType,cardBiao,cardEx,cardJunzheng,cardJiexiantupo);
    int favorite=settings->value("personal/favorite",-1).toInt();
    if(favorite==-1)
    {
        qsrand(QTime::currentTime().msecsSinceStartOfDay());
        favorite=qrand()%MAX_GENERAL;
        settings->setValue("personal/favorite",favorite);
    }

    ui->setupUi(this);

    this->setWindowTitle("Fair三国杀 " VERSION_STRING);
    //以下是界面的初始化，载入已保存的设置
    //连接服务器页
    QStringList sl;
    settings->beginGroup("connect");
    sl=settings->value("serverlist").toStringList();
    if(!sl.empty())
        ui->comboBoxServerAddress->addItems(sl);
    s=settings->value("serveraddress","127.0.0.1:5678").toString();
    ui->widgetUser->setVisible(false);
    ui->widgetPassword->setVisible(false);
    ui->checkBoxRemember->setVisible(false);
    ui->pushButtonRegister->setVisible(false);
    ui->comboBoxServerAddress->setCurrentText(s);
    int i;
    bool b;
    settings->endGroup();
    //基础设定页
    settings->beginGroup("server");
    i=settings->value("port",5678).toInt();
    ui->spinBoxPort->setValue(i);
    b=settings->value("upnp",true).toBool();
    ui->checkBoxUPNP->setChecked(b);
    b=settings->value("joinlist",true).toBool();
    ui->checkBoxJoinList->setChecked(b);
    s=settings->value("servername","一起来玩三国杀吧！").toString();
    ui->lineEditServerName->setText(s);
    i=settings->value("players",8).toInt();
    ui->spinBoxPlayers->setValue(i);
    b=settings->value("shuangnei",false).toBool();
    ui->checkBoxShuangNei->setChecked(b);
    i=settings->value("choices",5).toInt();
    ui->spinBoxChoices->setValue(i);
    i=settings->value("choiceszg",2).toInt();
    ui->spinBoxChoicesZG->setValue(i);
    i=settings->value("choicesnj",2).toInt();
    ui->spinBoxChoicesNJ->setValue(i);
    i=settings->value("rooms",1).toInt();
    ui->spinBoxRooms->setValue(i);
    b=settings->value("ex",true).toBool();
    ui->checkBoxEX->setChecked(b);
    b=settings->value("junzheng",true).toBool();
    ui->checkBoxJunZheng->setChecked(b);
    b=settings->value("cardjiexiantupo",true).toBool();
    ui->checkBoxCardJieXian->setChecked(b);
    //武将扩展包页
    b=settings->value("feng",true).toBool();
    ui->checkBoxFeng->setChecked(b);
    b=settings->value("huo",true).toBool();
    ui->checkBoxHuo->setChecked(b);
    b=settings->value("lin",true).toBool();
    ui->checkBoxLin->setChecked(b);
    b=settings->value("shan",true).toBool();
    ui->checkBoxShan->setChecked(b);
    b=settings->value("yijiang",true).toBool();
    ui->checkBoxYiJiang->setChecked(b);
    b=settings->value("yijiang2012",true).toBool();
    ui->checkBoxYiJiang2012->setChecked(b);
    b=settings->value("yijiang2013",true).toBool();
    ui->checkBoxYiJiang2013->setChecked(b);
    b=settings->value("yijiang2014",true).toBool();
    ui->checkBoxYiJiang2014->setChecked(b);
    b=settings->value("sp",true).toBool();
    ui->checkBoxSP->setChecked(b);
    b=settings->value("xingsp",true).toBool();
    ui->checkBoxXingSP->setChecked(b);
    b=settings->value("ol",true).toBool();
    ui->checkBoxOL->setChecked(b);
    b=settings->value("jiexiantupo",true).toBool();
    ui->checkBoxJieXianTuPo->setChecked(b);
    sl.clear();
    sl<<"禁止"<<"极低"<<"低"<<"一般";
    int j;
    s="shen";
    for(i=0;i<8;i++)
    {
        comboBoxShen[i]=new QComboBox(ui->groupBoxShen);
        comboBoxShen[i]->move(i/4*150+80,i%4*30+40);
        //comboBoxShen[i]->resize(69,22);
        comboBoxShen[i]->addItems(sl);
        j=settings->value(s+QString::number(i),i<6?2:1).toInt();
        comboBoxShen[i]->setCurrentIndex(j);
    }
    //禁将页
    sl.clear();
    sl<<"禁用"<<"名字"<<"称号"<<"势力"<<"血量"<<"扩展包";
    ui->tableWidgetBan->setHorizontalHeaderLabels(sl);
    ui->tableWidgetBan->setColumnWidth(0,32);
    ui->tableWidgetBan->setColumnWidth(1,80);
    ui->tableWidgetBan->setColumnWidth(2,100);
    ui->tableWidgetBan->setColumnWidth(3,32);
    ui->tableWidgetBan->setColumnWidth(4,32);
    ui->tableWidgetBan->setColumnWidth(5,80);
    ui->tableWidgetBan->setRowCount(MAX_GENERAL);
    checkBoxBans=new QCheckBox*[MAX_GENERAL];
    s="ban";
    QTableWidgetItem *item;
    for(int i=0;i<MAX_GENERAL;i++)
    {
        QWidget *widget=new QWidget();
        QHBoxLayout *layout=new QHBoxLayout(widget);
        checkBoxBans[i]=new QCheckBox();
        b=settings->value(s+QString::number(i),false).toBool();
        checkBoxBans[i]->setChecked(b);
        layout->addWidget(checkBoxBans[i]);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0,0,0,0);
        ui->tableWidgetBan->setCellWidget(i,0,widget);
        item=new QTableWidgetItem(allGenerals[i]->name);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetBan->setItem(i,1,item);
        item=new QTableWidgetItem(allGenerals[i]->nick);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetBan->setItem(i,2,item);
        item=new QTableWidgetItem(CGeneral::kingdomName(allGenerals[i]->kingdom));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetBan->setItem(i,3,item);
        item=new QTableWidgetItem(QString::number(allGenerals[i]->maxHP));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetBan->setItem(i,4,item);
        item=new QTableWidgetItem(CGeneral::packageName(allGenerals[i]->package));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetBan->setItem(i,5,item);
    }
    //密码页
    i=settings->value("authmode",0).toInt();
    switch(i)
    {
    case 0:
        on_radioButtonServerNone_toggled(true);
        break;
    case 1:
        ui->radioButtonServerPassword->setChecked(true);
        break;
    default:
        ui->radioButtonServerUserAndPassword->setChecked(true);
        break;
    }
    s=settings->value("password","").toString();
    ui->lineEditServerPassword->setText(s);
    b=settings->value("allowregister",true).toBool();
    ui->checkBoxAllowRegister->setChecked(b);
    //杂项页
    i=settings->value("operationtimeout",12).toInt();
    ui->spinBoxOperationTimeout->setValue(i);
    i=settings->value("wuxietimeout",6).toInt();
    ui->spinBoxWuXieTimeout->setValue(i);
    i=settings->value("extretime",25).toInt();
    ui->spinBoxExtreTime->setValue(i);
    i=settings->value("choicetimeout",25).toInt();
    ui->spinBoxChoiceTimeout->setValue(i);
    i=settings->value("maxonlooker",20).toInt();
    ui->spinBoxMaxOnlooker->setValue(i);
    b=settings->value("allowchat",true).toBool();
    ui->checkBoxAllowChat->setChecked(b);
    b=settings->value("bansameip",false).toBool();
    ui->checkBoxBanSameIP->setChecked(b);

    b=settings->value("joingame",true).toBool();
    ui->checkBoxJoinGame->setChecked(b);
    settings->endGroup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getSavedUserAndPassword()
{
    QString s,s2,prefix;
    bool b;
    s=ui->comboBoxServerAddress->currentText();
    if(s!="")
    {
        quint16 port;
        b=splitAddress(s,s2,port);
        if(!b)
        {
            ui->radioButtonNone->setChecked(true);
            goto aa;
        }
        if(addressIsIP(s2))
        {
            int m=settings->value("connect/?mode",-1).toInt();
            if(m==0)
            {
                goto aa;
            }
            else if(m==1)
                ui->radioButtonPassword->setChecked(true);
            else if(m==2)
                ui->radioButtonUserAndPassword->setChecked(true);
            b=settings->value("connect/?remember",false).toBool();
            if(b)
            {
                s2=settings->value("connect/?user","").toString();
                ui->lineEditUser->setText(s2);
                savedPassword=settings->value("connect/?password").toByteArray();
                if(savedPassword.length()==20)
                {
                    ui->lineEditPassword->setText("这段文字只是用来占位的");
                }
                else
                    savedPassword.clear();
            }
        }
        else
        {
            prefix="serverlist/"+s;
            int m=settings->value(prefix+"?mode",-1).toInt();
            if(m==0)
            {
                goto aa;
            }
            else if(m==1)
                ui->radioButtonPassword->setChecked(true);
            else if(m==2)
                ui->radioButtonUserAndPassword->setChecked(true);
            b=settings->value(prefix+"?remember",false).toBool();
            if(b)
            {
                s=settings->value(prefix+"?user","").toString();
                ui->lineEditUser->setText(s);
                savedPassword=settings->value(prefix+"?password","").toByteArray();
                if(savedPassword.length()==20)
                {
                    ui->lineEditPassword->setText("这段文字只是用来占位的");
                }
                else
                    savedPassword.clear();
            }
        }
        ui->checkBoxRemember->setChecked(b);
        return;
    }
aa:
    ui->radioButtonNone->setChecked(true);
    savedPassword.clear();
    ui->lineEditPassword->clear();
}

bool MainWindow::splitAddress(const QString &address, QString &hostname, quint16 &port)
{
    int i=address.lastIndexOf(':');
    QString s;
    if(i==-1)
    {
        port=5678;
        hostname=address;
        return true;
    }
    else
    {
        s=address.mid(i+1);
        bool b;
        int p=s.toInt(&b);
        if(b&&p<=65535&&p>=0)
        {
            port=p;
            hostname=address.left(i);
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool MainWindow::addressIsIP(const QString &ip)
{
    QHostAddress ha;
    return ha.setAddress(ip);
}

void MainWindow::saveClientConfig()
{
    settings->setValue("connect/serveraddress",ui->comboBoxServerAddress->currentText());
}

void MainWindow::saveServerConfig()
{
    settings->beginGroup("server");
    //基础设定页
    settings->setValue("port",ui->spinBoxPort->value());
    settings->setValue("upnp",ui->checkBoxUPNP->isChecked());
    settings->setValue("joinlist",ui->checkBoxJoinList->isChecked());
    settings->setValue("servername",ui->lineEditServerName->text());
    settings->setValue("players",ui->spinBoxPlayers->value());
    settings->setValue("shuangnei",ui->checkBoxShuangNei->isChecked());
    settings->setValue("choices",ui->spinBoxChoices->value());
    settings->setValue("choiceszg",ui->spinBoxChoicesZG->value());
    settings->setValue("choicesnj",ui->spinBoxChoicesNJ->value());
    settings->setValue("rooms",ui->spinBoxRooms->value());
    settings->setValue("ex",ui->checkBoxEX->isChecked());
    settings->setValue("junzheng",ui->checkBoxJunZheng->isChecked());
    settings->setValue("cardjiexiantupo",ui->checkBoxCardJieXian->isChecked());
    //武将扩展包页
    settings->setValue("feng",ui->checkBoxFeng->isChecked());
    settings->setValue("huo",ui->checkBoxHuo->isChecked());
    settings->setValue("lin",ui->checkBoxLin->isChecked());
    settings->setValue("shan",ui->checkBoxShan->isChecked());
    settings->setValue("yijiang",ui->checkBoxYiJiang->isChecked());
    settings->setValue("yijiang2012",ui->checkBoxYiJiang2012->isChecked());
    settings->setValue("yijiang2013",ui->checkBoxYiJiang2013->isChecked());
    settings->setValue("yijiang2014",ui->checkBoxYiJiang2014->isChecked());
    settings->setValue("sp",ui->checkBoxSP->isChecked());
    settings->setValue("xingsp",ui->checkBoxXingSP->isChecked());
    settings->setValue("ol",ui->checkBoxOL->isChecked());
    settings->setValue("jiexiantupo",ui->checkBoxJieXianTuPo->isChecked());
    int i;
    QString s;
    s="shen";
    for(i=0;i<8;i++)
    {
        settings->setValue(s+QString::number(i),comboBoxShen[i]->currentIndex());
    }
    //禁将页
    for(int i=0;i<MAX_GENERAL;i++)
    {
        settings->setValue("ban"+QString::number(i),checkBoxBans[i]->isChecked());
    }
    //密码页
    if(ui->radioButtonServerNone->isChecked())
        i=0;
    else if(ui->radioButtonServerPassword->isChecked())
        i=1;
    else
        i=2;
    settings->setValue("authmode",i);
    settings->setValue("password",ui->lineEditPassword->text());
    settings->setValue("allowregister",ui->checkBoxAllowRegister->isChecked());
    //杂项页
    settings->setValue("operationtimeout",ui->spinBoxOperationTimeout->value());
    settings->setValue("wuxietimeout",ui->spinBoxWuXieTimeout->value());
    settings->setValue("extretime",ui->spinBoxExtreTime->value());
    settings->setValue("choicetimeout",ui->spinBoxChoiceTimeout->value());
    settings->setValue("maxonlooker",ui->spinBoxMaxOnlooker->value());
    settings->setValue("allowchat",ui->checkBoxAllowChat->isChecked());
    settings->setValue("bansameip",ui->checkBoxBanSameIP->isChecked());

    settings->setValue("joingame",ui->checkBoxJoinGame->isChecked());

    settings->endGroup();
}

QByteArray MainWindow::generateUID()
{
    auto it=QNetworkInterface::allInterfaces();
    QByteArray ba;
    if(it.isEmpty())
    {
        return ba;
    }
    ba=it[0].hardwareAddress().toLocal8Bit();
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(ba);
    return hash.result();
}

void MainWindow::setupDB()
{
    if(!db.isValid())
    {
        db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("user.db");
        if(!db.open())
        {
            QMessageBox::about(nullptr,"错误","无法打开用户数据库！");
            return;
        }
        QSqlQuery query(db);
        query.exec("CREATE  TABLE  IF NOT EXISTS \"user\" (\"username\" VARCHAR PRIMARY KEY  NOT NULL  UNIQUE , \"password\" CHAR NOT NULL )");
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_radioButtonNone_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButtonRegister->setVisible(false);
        ui->widgetUser->setVisible(false);
        ui->widgetPassword->setVisible(false);
        ui->checkBoxRemember->setVisible(false);
    }
}

void MainWindow::on_radioButtonUserAndPassword_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButtonRegister->setVisible(true);
        ui->widgetUser->setVisible(true);
        ui->widgetPassword->setVisible(true);
        ui->checkBoxRemember->setVisible(true);
    }
}

void MainWindow::on_radioButtonPassword_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButtonRegister->setVisible(false);
        ui->widgetUser->setVisible(false);
        ui->widgetPassword->setVisible(true);
        ui->checkBoxRemember->setVisible(false);
    }
}

void MainWindow::on_pushButtonShenBan_clicked()
{
    for(int i=0;i<8;i++)
    {
        comboBoxShen[i]->setCurrentIndex(0);
    }
}

void MainWindow::on_pushButtonShenDefault_clicked()
{
    for(int i=0;i<8;i++)
    {
        comboBoxShen[i]->setCurrentIndex(i<6?2:1);
    }
}

void MainWindow::on_radioButtonServerNone_toggled(bool checked)
{
    if(checked)
    {
        ui->widgetServerPassword->setVisible(false);
        ui->checkBoxAllowRegister->setVisible(false);
        ui->pushButtonUserDatabase->setVisible(false);
    }
}

void MainWindow::on_radioButtonServerPassword_toggled(bool checked)
{
    if(checked)
    {
        ui->widgetServerPassword->setVisible(true);
        ui->checkBoxAllowRegister->setVisible(false);
        ui->pushButtonUserDatabase->setVisible(false);
    }
}

void MainWindow::on_radioButtonServerUserAndPassword_toggled(bool checked)
{
    if(checked)
    {
        ui->widgetServerPassword->setVisible(false);
        ui->checkBoxAllowRegister->setVisible(true);
        ui->pushButtonUserDatabase->setVisible(true);
    }
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,"关于QT");
}

void MainWindow::on_actionPersonalSettings_triggered()
{
    if(!dialogPersonalSettings)
    {
        dialogPersonalSettings=new DialogPersonalSettings(this);
    }
    dialogPersonalSettings->initVar();
    dialogPersonalSettings->show();
}

void MainWindow::on_pushButtonFindServer_clicked()
{
    if(!mwServerList)
    {
        mwServerList=new MainWindowServerList(this);
    }
    mwServerList->initWindow();
    mwServerList->showMaximized();
}

void MainWindow::on_pushButtonStartServer_clicked()
{
    saveServerConfig();
    server=new CServer();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    saveClientConfig();
    saveServerConfig();
}

void MainWindow::on_pushButtonConnect_clicked()
{
    QString s,ip;
    quint16 port;
    s=ui->comboBoxServerAddress->currentText();
    if(!splitAddress(s,ip,port))
    {
        QMessageBox::about(this,"错误","服务器地址不正确");
        ui->comboBoxServerAddress->setFocus();
        return;
    }
    quint8 authmode;
    QString user,prefix;
    QByteArray password;
    if(addressIsIP(ip))
        prefix="connect/";
    else
        prefix="serverlist/"+s;
    if(ui->radioButtonPassword->isChecked())
    {
        authmode=1;
        if(!savedPassword.isEmpty())
            password=savedPassword;
        else
        {
            s=ui->lineEditPassword->text();
            if(s.isEmpty())
            {
                QMessageBox::about(this,"错误","密码不能为空");
                ui->lineEditPassword->setFocus();
                return;
            }
            password=QCryptographicHash::hash(s.toUtf8(),QCryptographicHash::Sha1);
        }
    }
    else if(ui->radioButtonUserAndPassword->isChecked())
    {
        authmode=2;
        user=ui->lineEditUser->text();
        if(savedPassword.isEmpty())
            password=ui->lineEditPassword->text().toUtf8();
        else
            password=savedPassword;
        if(user.isEmpty()||password.isEmpty())
        {
            QMessageBox::about(this,"错误","用户名和密码不能为空");
            return;
        }
        if(user.contains(" ")||user.contains("　"))
        {
            QMessageBox::about(this,"错误","用户名不能包含空格");
            return;
        }
        if(user.toUtf8().length()>30)
        {
            QMessageBox::about(this,"错误","用户名的长度不能超过30字节。");
            return;
        }
        if(!savedPassword.isEmpty())
            password=QCryptographicHash::hash(user.toUtf8()+password+"FairSGS",QCryptographicHash::Sha1);
        bool b;
        if(ui->checkBoxRemember->isChecked())
        {
            b=true;
            settings->setValue(s+"?user",user);
            settings->setValue(s+"?password",password);
        }
        else
            b=false;
        settings->setValue(prefix+"?remember",b);
    }
    else
    {
        authmode=0;
    }
    settings->setValue(prefix+"?mode",authmode);

    QStringList sl;
    int i,j;
    j=ui->comboBoxServerAddress->count();
    for(i=0;i<j;i++)
    {
        s=ui->comboBoxServerAddress->itemText(i);
        if(s==ui->comboBoxServerAddress->currentText())
            goto aa;
        sl.append(s);
    }
    sl.append(ui->comboBoxServerAddress->currentText());
    settings->setValue("connect/serverlist",sl);
aa:
    client=new CClient(ip,port,authmode,password,user);
}

void MainWindow::handleRun(std::function<void()> func)
{
    func();
}

void MainWindow::on_comboBoxServerAddress_currentTextChanged(const QString &)
{
    getSavedUserAndPassword();
}

void MainWindow::on_lineEditPassword_textChanged(const QString &)
{
    this->savedPassword.clear();
}

void MainWindow::imageLoad(CImage *img, void *p, void *par)
{
    if(wLoadSource->ready&&img->loaded)
    {
        imageDirectLoad(img,p);
    }
    else
    {
        CCheckPointer *cp=new CCheckPointer(p,par);
        emit wLoadSource->newWork(WORKTYPE_IMAGE,img,cp);
    }
}

void MainWindow::imageLoad(CImage *img, void *p)
{
    if(wLoadSource->ready&&img->loaded)
    {
        imageDirectLoad(img,p);
    }
    else
    {
        emit wLoadSource->newWork(WORKTYPE_IMAGE,img,p);
    }
}

void MainWindow::imageDirectLoad(CImage *img, void *p)
{
    if(p)
    {
        QWidget *widget=(QWidget *)p;
        if(QLabel *label=qobject_cast<QLabel*>(widget))
            label->setPixmap(img->pixmap);
        else if(QToolButton *button=qobject_cast<QToolButton*>(widget))
            button->setIcon(QIcon(img->pixmap));
    }
}

void MainWindow::on_pushButtonRegister_clicked()
{
    QString ip;
    quint16 port;
    QString s=ui->comboBoxServerAddress->currentText();
    if(s.isEmpty())
    {
        QMessageBox::warning(this,"错误","请输入服务器地址。");
        return;
    }
    if(!splitAddress(s,ip,port))
    {
        QMessageBox::warning(this,"错误","服务器地址格式不正确。");
        return;
    }
    if(!dialogRegister)
    {
        dialogRegister=new DialogRegister(this);
    }
    dialogRegister->init(1);
    dialogRegister->ip=ip;
    dialogRegister->port=port;
    dialogRegister->show();
}

void MainWindow::iconLoad(CIcon *icon, QPushButton *button, void *par)
{
    if(wLoadSource->ready&&icon->loaded)
    {
        if(button) button->setIcon(icon->icon);
    }
    else
    {
        CCheckPointer *cp=new CCheckPointer(button,par);
        emit wLoadSource->newWork(WORKTYPE_ICON,icon,cp);
    }
}

void MainWindow::iconLoad(CIcon *icon, QPushButton* button)
{
    if(wLoadSource->ready&&icon->loaded)
    {
        button->setIcon(icon->icon);
    }
    else
    {
        emit wLoadSource->newWork(WORKTYPE_ICON,icon,button);
    }
}
