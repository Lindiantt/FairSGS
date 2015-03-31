#include "buttongeneralcard.h"
#include "general/cgeneral.h"
#include "general/cskill.h"

#include "mainwindow.h"
extern MainWindow *w;
#include "thread/workerloadsource.h"
#include "thread/cimage.h"
#include "thread/ccheckpointer.h"

ButtonGeneralCard::ButtonGeneralCard(quint16 id,QWidget *parent) : QToolButton(parent)
{
    this->setIconSize(QSize(130,190));
    this->resize(130,208);
    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    this->id=id;
    CGeneral *g=CGeneral::find(id);
    CCheckPointer::addParent(this);
    if(g)
    {
        this->setText(g->name);
        QString s=g->name+" "+CGeneral::kingdomName(g->kingdom)+" "+QString::number(g->maxHP)+"血";
        foreach (CSkill* skill, g->skill) {
            s+="\n\n"+skill->name+"\n";
            s+=skill->description;
        }
        this->setToolTip(s);
        CImage *img;
        if(g->id>=10000)
            img=w->wLoadSource->shenCards[g->id-10000];
        else
            img=w->wLoadSource->generalCards[g->id];
        w->imageLoad(img,this,this);
    }
    else
    {
        this->setText(QString::number(id));
    }
}

ButtonGeneralCard::~ButtonGeneralCard()
{

}
