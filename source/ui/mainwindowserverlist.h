#ifndef MAINWINDOWSERVERLIST_H
#define MAINWINDOWSERVERLIST_H

#include <QMainWindow>
#include "network/cserverlist.h"
#include <qhostaddress.h>
#include <QStatusBar>
#include "ui/dialogslsettings.h"
#include <qmenu.h>

namespace Ui {
class MainWindowServerList;
}

class MainWindowServerList : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowServerList(QWidget *parent = 0);
    ~MainWindowServerList();
    void initWindow();
    void addAddress(int r,QHostAddress &address,quint16 port);
    void socketFinished();
    void clearServers();
    Ui::MainWindowServerList *ui;
    CServerList *serverList;
    DialogSLSettings* dialogSLSettings;
    int socketCount;
    QMenu rcMenu;

private slots:
    void handleCustomMenu(QPoint);
    void handleGetInfo();
    void handleChoose();
    void on_actionGetMore_triggered();

    void on_actionRefresh_triggered();

    void on_actionAdvance_triggered();

    void on_tableWidgetServerList_cellDoubleClicked(int row, int);

private:

};

#endif // MAINWINDOWSERVERLIST_H
