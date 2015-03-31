#include "dialogchoosegeneral.h"
#include "ui_dialogchoosegeneral.h"
#include "ui/others/buttongeneralcard.h"
#include <QCloseEvent>
#include "thread/ccheckpointer.h"

DialogChooseGeneral::DialogChooseGeneral(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseGeneral)
{
    ui->setupUi(this);
    this->setFixedSize(710,500);
    bc=false;
}

DialogChooseGeneral::~DialogChooseGeneral()
{
    delete ui;
}

void DialogChooseGeneral::init(QList<quint16> &gids, const QString &title)
{
    foreach (auto button, buttons) {
        CCheckPointer::deleteObject(button);
    }
    buttons.clear();
    ButtonGeneralCard *button;
    int x=0,y=0;
    foreach (quint16 id, gids) {
        button=new ButtonGeneralCard(id,ui->scrollAreaWidgetContents);
        connect(button,SIGNAL(clicked()),this,SLOT(buttonClicked()));
        button->move(5+135*x,10+218*y);
        x++;
        if(x==5)
        {
            x=0;
            y++;
        }
        buttons.append(button);
    }
    if(x!=0) y++;
    ui->scrollAreaWidgetContents->setMinimumHeight(10+218*y);
    this->setWindowTitle(title+"请选择一名武将");
}

void DialogChooseGeneral::closeEvent(QCloseEvent *)
{
    emit chosen(-1);
}

void DialogChooseGeneral::buttonClicked()
{
    ButtonGeneralCard *button=(ButtonGeneralCard*)sender();
    emit chosen(button->id);
    close();
}
