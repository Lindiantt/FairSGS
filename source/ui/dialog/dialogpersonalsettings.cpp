#include "dialogpersonalsettings.h"
#include "ui_dialogpersonalsettings.h"

#include <qmessagebox.h>

#include "mainwindow.h"
extern MainWindow *w;
#include "general/cgeneral.h"
#include "ui/dialog/dialogchoosegeneral.h"
#include "network/define.h"

DialogPersonalSettings::DialogPersonalSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPersonalSettings)
{
    dcg=nullptr;
    ui->setupUi(this);
    setFixedSize(250,184);
}

DialogPersonalSettings::~DialogPersonalSettings()
{
    delete ui;
}

void DialogPersonalSettings::initVar()
{
    w->settings->beginGroup("personal");
    QString s=w->settings->value("nick","玩家").toString();
    ui->lineEditNick->setText(s);
    bool b=w->settings->value("allowlookhands",false).toBool();
    ui->checkBoxAllowLookHands->setChecked(b);
    b=w->settings->value("allowlookidentity",false).toBool();
    ui->checkBoxAllowLookIdentity->setChecked(b);
    favorite=w->settings->value("favorite",0).toInt();
    setFavorite(favorite);
    w->settings->endGroup();
}

void DialogPersonalSettings::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPersonalSettings::on_pushButtonOK_clicked()
{
    QString s=ui->lineEditNick->text();
    if(s.contains(' ')||s.contains("　"))
    {
        QMessageBox::about(this,"错误","昵称不能包含空格");
        return;
    }
    if(s.isEmpty())
    {
        QMessageBox::about(this,"错误","昵称不能为空");
        return;
    }
    if(s.toUtf8().length()>30)
    {
        QMessageBox::about(this,"错误","昵称不能超过30字节");
        return;
    }
    w->settings->beginGroup("personal");
    w->settings->setValue("nick",ui->lineEditNick->text());
    w->settings->setValue("allowlookhands",ui->checkBoxAllowLookHands->isChecked());
    w->settings->setValue("allowlookidentity",ui->checkBoxAllowLookIdentity->isChecked());
    w->settings->setValue("favorite",favorite);
    w->settings->endGroup();
    this->close();
}

void DialogPersonalSettings::setFavorite(quint16 favor)
{
    CGeneral *g=CGeneral::find(favor);
    if(!g)
    {
        g=CGeneral::find(0);
        favorite=0;
    }
    else
        favorite=favor;
    ui->labelFavorite->setText("最爱武将："+g->name);
}

void DialogPersonalSettings::on_pushButton_clicked()
{
    if(!dcg)
    {
        dcg=new DialogChooseGeneral(this);
        QList<quint16> list;
        for(quint16 i=0;i<MAX_GENERAL;i++)
        {
            list.append(i);
        }
        dcg->init(list);
        connect(dcg,dcg->chosen,this,this->handleGeneralChosen);
    }
    dcg->show();
}

void DialogPersonalSettings::handleGeneralChosen(int id)
{
    if(id!=-1)
        setFavorite(id);
}
