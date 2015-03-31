#ifndef DIALOGCHOOSEGENERAL_H
#define DIALOGCHOOSEGENERAL_H

#include <QDialog>
#include "pch.h"

namespace Ui {
class DialogChooseGeneral;
}

class ButtonGeneralCard;

class DialogChooseGeneral : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseGeneral(QWidget *parent = 0);
    ~DialogChooseGeneral();
    void init(QList<quint16> &,const QString &title="");

public slots:
    void buttonClicked();
signals:
    void chosen(int);
private:
    void closeEvent(QCloseEvent *);
    Ui::DialogChooseGeneral *ui;
    QList<ButtonGeneralCard*> buttons;
    bool bc;
};

#endif // DIALOGCHOOSEGENERAL_H
