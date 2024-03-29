﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.h"
#include <QMainWindow>

class CCard;
class CServer;
class CClient;
class MainWindowServerList;
class DialogPersonalSettings;
class CCardType;
class CGeneral;
class MainWindowServer;
class DialogConnect;
class workerLoadSource;
class QComboBox;
class MainWindowClient;
class CImage;
class QLabel;
class DialogRegister;
class QCheckBox;
class CIcon;
class QPushButton;
class CCheckPointer;

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
    void imageLoad(CImage *,void*,void*);
    void imageLoad(CImage *,void *p=nullptr);
    void imageDirectLoad(CImage *,void *);
    void iconLoad(CIcon*,QPushButton*,void* par);
    void iconLoad(CIcon*,QPushButton *p=nullptr);

    Ui::MainWindow *ui;
    QComboBox* comboBoxShen[8];
    QSettings *settings,*scoreRecord;
    DialogPersonalSettings *dialogPersonalSettings;
    DialogConnect *dialogConnect;
    MainWindowServerList *mwServerList;
    MainWindowServer *mwServer;
    QNetworkAccessManager *network;
    CServer *server;
    QSqlDatabase db;
    workerLoadSource *wLoadSource;
    CClient *client;
    MainWindowClient *mwCLient;
    DialogRegister *dialogRegister;

    CCardType **cardType;
    QList<CCard*> cardBiao,cardEx,cardJunzheng,cardJiexiantupo;
    CGeneral **allGenerals,**generalShen;
    QCheckBox** checkBoxBans;
public slots:
    void handleRun(std::function<void()>func);
signals:
    void runFunction(std::function<void()>func);

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

    void on_pushButtonConnect_clicked();

    void on_comboBoxServerAddress_currentTextChanged(const QString &arg1);

    void on_lineEditPassword_textChanged(const QString &arg1);

    void on_pushButtonRegister_clicked();

private:
    QByteArray savedPassword;

    bool addressIsIP(const QString &);

    void getSavedUserAndPassword();
    void saveServerConfig();
    void saveClientConfig();
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
