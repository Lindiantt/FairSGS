#ifndef DIALOGPERSONALSETTINGS_H
#define DIALOGPERSONALSETTINGS_H

#include <QDialog>


namespace Ui {
class DialogPersonalSettings;
}

class DialogPersonalSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPersonalSettings(QWidget *parent = 0);
    ~DialogPersonalSettings();
    void initVar();

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonOK_clicked();

private:
    Ui::DialogPersonalSettings *ui;
};

#endif // DIALOGPERSONALSETTINGS_H
