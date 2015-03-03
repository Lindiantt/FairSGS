#ifndef MAINWINDOWSERVERLIST_H
#define MAINWINDOWSERVERLIST_H

#include <QMainWindow>
#include <QStatusBar>
#include <qmenu.h>


class CServerList;
class DialogSLSettings;

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
