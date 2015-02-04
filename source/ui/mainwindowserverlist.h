#ifndef MAINWINDOWSERVERLIST_H
#define MAINWINDOWSERVERLIST_H

#include <QMainWindow>
#include "network/cserverlist.h"
#include <qhostaddress.h>
#include <QStatusBar>

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
    void addAddress(QHostAddress &address,quint16 port);
    void socketFinished();
    Ui::MainWindowServerList *ui;

private:

};

#endif // MAINWINDOWSERVERLIST_H
