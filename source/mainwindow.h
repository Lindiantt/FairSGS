#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qsettings.h>
#include <qcombobox.h>
#include <qpair.h>
#include "ui/dialogpersonalsettings.h"
#include "ui/mainwindowserverlist.h"
#include "network/cserver.h"
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool splitAddress(const QString &, QString &hostname, quint16 &port);
    QByteArray generateUID();
    void setupDB();

    Ui::MainWindow *ui;
    QComboBox* comboBoxShen[8];
    QSettings *settings;
    DialogPersonalSettings *dialogPersonalSettings;
    MainWindowServerList *mwServerList;
    QNetworkAccessManager *network;
    CServer *server;
    QSqlDatabase db;

private slots:
    void on_actionExit_triggered();

    void on_radioButtonNone_toggled(bool checked);

    void on_radioButtonUserAndPassword_toggled(bool checked);

    void on_radioButtonPassword_toggled(bool checked);

    void on_pushButtonShenBan_clicked();

    void on_pushButtonShenDefault_clicked();

    void on_radioButtonServerNone_toggled(bool checked);

    void on_radioButtonServerPassword_toggled(bool checked);

    void on_radioButtonServerUserAndPassword_toggled(bool checked);

    void on_actionAboutQt_triggered();

    void on_actionPersonalSettings_triggered();

    void on_pushButtonFindServer_clicked();

    void on_pushButtonStartServer_clicked();

private:
    QByteArray savedPassword;

    bool addressIsIP(const QString &);

    void getSavedUserAndPassword();
    void saveServerConfig();
    void saveClientConfig();
};

#endif // MAINWINDOW_H
