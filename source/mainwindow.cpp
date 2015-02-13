#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qmessagebox.h>
#include <qhostaddress.h>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString s=QCoreApplication::applicationDirPath();
    settings=new QSettings(s+"/fairsgs.ini",QSettings::IniFormat);
    dialogPersonalSettings=NULL;
    mwServerList=NULL;
    network=new QNetworkAccessManager();

    ui->setupUi(this);

    //以下是界面的初始化，载入已保存的设置
    //连接服务器页
    QStringList sl;
    settings->beginGroup("connect");
    sl=settings->value("serverlist").toStringList();
    if(!sl.empty())
        ui->comboBoxServerAddress->addItems(sl);
    s=settings->value("serveraddress","127.0.0.1:5678").toString();
    ui->comboBoxServerAddress->setCurrentText(s);
    int i;
    i=settings->value("authmode",0).toInt();
    bool b;
    switch (i) {
    case 0:
        on_radioButtonNone_toggled(true);
        break;
    case 1:
        ui->radioButtonPassword->setChecked(true);
        break;
    default:
        ui->radioButtonUserAndPassword->setChecked(true);
        break;
    }
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
    ui->lineEditPassword->setText(s);
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
    QString s,s2;
    bool b;
    s=ui->comboBoxServerAddress->currentText();
    if(s!="")
    {
        quint16 port;
        b=splitAddress(s,s2,port);
        if(!b)
        {
            return;
        }
        if(addressIsIP(s2))
        {
            b=settings->value("connect/remember",false).toBool();
            if(b)
            {
                s2=settings->value("connect/user","").toString();
                ui->lineEditUser->setText(s2);
                savedPassword=settings->value("connect/password").toByteArray();
                if(savedPassword.length()==20)
                {
                    ui->lineEditPassword->setText("这段文字只是用来占位的");
                }
            }
        }
        else
        {
            s2="serverlist/";
            s2+=s;
            b=settings->value(s2+"?remember",false).toBool();
            if(b)
            {
                s=settings->value(s2+"?user","").toString();
                ui->lineEditUser->setText(s);
                savedPassword=settings->value(s2+"?password","").toByteArray();
                if(savedPassword.length()==20)
                {
                    ui->lineEditPassword->setText("这段文字只是用来占位的");
                }
            }
        }
        ui->checkBoxRemember->setChecked(b);
    }
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
        int i=s.toInt(&b);
        if(b&&i<=65535&&i>=0)
        {
            port=i;
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
    QStringList sl;
    int i,j;
    settings->beginGroup("connect");
    j=ui->comboBoxServerAddress->count();
    for(i=0;i<j;i++)
    {
        sl.append(ui->comboBoxServerAddress->itemText(i));
    }
    settings->setValue("serverlist",sl);
    settings->setValue("serveraddress",ui->comboBoxServerAddress->currentText());
    if(ui->radioButtonNone->isChecked())
        i=0;
    else if(ui->radioButtonPassword->isChecked())
        i=1;
    else
        i=2;
    settings->setValue("authmode",i);
    settings->endGroup();
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
    auto ba=it[0].hardwareAddress().toLocal8Bit();
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
        getSavedUserAndPassword();
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
    server=new CServer();
    //显示窗口...
}
