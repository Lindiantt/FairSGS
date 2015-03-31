#ifndef DIALOGSELECT_H
#define DIALOGSELECT_H

#include <QDialog>

namespace Ui {
class DialogSelect;
}

class DialogSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelect(QWidget *parent = 0);
    ~DialogSelect();
    void init(const QString &,int selectionType,const QStringList option);
    bool selected;
signals:
    void select(int);
private slots:
    void handleButtonClick();
private:
    void closeEvent(QCloseEvent *);
    void calsize();
    void makeButtons(int);
    Ui::DialogSelect *ui;
    QVector<QPushButton*> buttons;

};

#endif // DIALOGSELECT_H
