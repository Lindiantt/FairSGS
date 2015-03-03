#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>

namespace Ui {
class MainWindowClient;
}

class DialogChooseRoom;

class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowClient(QWidget *parent = 0);
    ~MainWindowClient();
    void init();
    int closeClient(QWidget *);
    DialogChooseRoom *dialogChooseRoom;
    Ui::MainWindowClient *ui;
    uint roomID;

private:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOWCLIENT_H
