#ifndef DIALOGPERSONALSETTINGS_H
#define DIALOGPERSONALSETTINGS_H

#include <QDialog>


namespace Ui {
class DialogPersonalSettings;
}

class DialogChooseGeneral;

class DialogPersonalSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPersonalSettings(QWidget *parent = 0);
    ~DialogPersonalSettings();
    void initVar();
    DialogChooseGeneral *dcg;

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonOK_clicked();

    void on_pushButton_clicked();
    void handleGeneralChosen(int);
private:
    Ui::DialogPersonalSettings *ui;
    void setFavorite(quint16);
    quint16 favorite;

};

#endif // DIALOGPERSONALSETTINGS_H
