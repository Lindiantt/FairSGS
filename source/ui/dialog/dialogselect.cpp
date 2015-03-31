#include "dialogselect.h"
#include "ui_dialogselect.h"
#include "game/cplayer.h"
#include <QPushButton>

DialogSelect::DialogSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelect)
{
    ui->setupUi(this);
}

DialogSelect::~DialogSelect()
{
    delete ui;
}

void DialogSelect::init(const QString &question, int selectionType, const QStringList option)
{
    selected=false;
    qDeleteAll(buttons);
    ui->label->setText(question);
    switch (selectionType) {
    case SELECTTYPE_CUSTOM:
        makeButtons(option.size());
        for(int i=0;i<option.size();i++)
        {
            buttons[i]->setText(option[i]);
        }
        break;
    case SELECTTYPE_KINGDOM:
        makeButtons(4);
        buttons[0]->setText("魏");
        buttons[1]->setText("蜀");
        buttons[2]->setText("吴");
        buttons[3]->setText("群");
        break;
    default:
        break;
    }
    calsize();
    this->show();
}

void DialogSelect::closeEvent(QCloseEvent *)
{
    if(!selected)
        emit select(0);
}

void DialogSelect::calsize()
{
    this->resize(200,37+29*buttons.size());
}

void DialogSelect::makeButtons(int n)
{
    QPushButton *button;
    buttons.resize(n);
    for(int i=0;i<n;i++)
    {
        button=new QPushButton(this);
        connect(button,SIGNAL(clicked()),this,SLOT(handleButtonClick()));
        ui->verticalLayout->addWidget(button);
        buttons[i]=button;
    }
}

void DialogSelect::handleButtonClick()
{
    QPushButton *button=(QPushButton *)sender();
    selected=true;
    for(int i=0;i<buttons.size();i++)
    {
        if(buttons[i]==button)
        {
            emit select(i);
            break;
        }
    }
    this->close();
}
