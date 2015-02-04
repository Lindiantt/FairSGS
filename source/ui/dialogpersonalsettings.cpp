#include "dialogpersonalsettings.h"
#include "ui_dialogpersonalsettings.h"

#include <qmessagebox.h>

#include "mainwindow.h"
extern MainWindow *w;

DialogPersonalSettings::DialogPersonalSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPersonalSettings)
{
    ui->setupUi(this);
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
    w->settings->endGroup();
}

void DialogPersonalSettings::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPersonalSettings::on_pushButtonOK_clicked()
{
    QString s=ui->lineEditNick->text();
    if(s.contains(' ')||s.contains('　'))
    {
        QMessageBox::about(this,"错误","昵称不能包含空格");
        return;
    }
    if(!s.length())
    {
        QMessageBox::about(this,"错误","昵称不能为空");
        return;
    }
    w->settings->beginGroup("personal");
    w->settings->setValue("nick",ui->lineEditNick->text());
    w->settings->setValue("allowlookhands",ui->checkBoxAllowLookHands->isChecked());
    w->settings->setValue("allowlookidentity",ui->checkBoxAllowLookIdentity->isChecked());
    w->settings->endGroup();
    this->close();
}
