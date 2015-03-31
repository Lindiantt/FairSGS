#ifndef BUTTIONGENERALCARD_H
#define BUTTIONGENERALCARD_H

#include <qtoolbutton.h>

class ButtonGeneralCard : public QToolButton
{
    Q_OBJECT
public:
    ButtonGeneralCard(quint16 id,QWidget *parent);
    ~ButtonGeneralCard();
    quint16 id;
    QList<ButtonGeneralCard*> derived;

};

#endif // BUTTIONGENERALCARD_H
