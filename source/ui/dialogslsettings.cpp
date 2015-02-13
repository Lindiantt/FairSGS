#include "dialogslsettings.h"
#include "ui_dialogslsettings.h"

#include "network/define.h"
#include <qmessagebox.h>

#include "mainwindow.h"
extern MainWindow *w;

DialogSLSettings::DialogSLSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSLSettings)
{
    setFixedSize(400,190);
    ui->setupUi(this);
}

DialogSLSettings::~DialogSLSettings()
{
    delete ui;
}

void DialogSLSettings::initVar()
{
    QString s;
    s=w->settings->value("slconfig/geturl",URL_DEFAULTGET).toString();
    ui->lineEditDownload->setText(s);
    s=w->settings->value("slconfig/regurl",URL_DEFAULTREG).toString();
    ui->lineEditReg->setText(s);
    bool b=w->settings->value("slconfig/autogetinfo",true).toBool();
    ui->checkBoxAutoGetInfo->setChecked(b);
}

void DialogSLSettings::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogSLSettings::on_pushButtonOK_clicked()
{
    QString s;
    s=ui->lineEditDownload->text();
    if(!s.endsWith('/'))
    {
        s.append('/');
    }
    w->settings->setValue("slconfig/geturl",s);
    s=ui->lineEditReg->text();
    if(!s.endsWith('/'))
    {
        s.append('/');
    }
    w->settings->setValue("slconfig/regurl",s);
    w->settings->setValue("slconfig/autogetinfo",ui->checkBoxAutoGetInfo->isChecked());
    this->close();
}

void DialogSLSettings::on_pushButtonDefualt_clicked()
{
    ui->lineEditDownload->setText(URL_DEFAULTGET);
    ui->lineEditReg->setText(URL_DEFAULTREG);
    ui->checkBoxAutoGetInfo->setChecked(true);
}
