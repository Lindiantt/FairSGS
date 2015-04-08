#ifndef MAINWINDOWCLIENT_H
#define MAINWINDOWCLIENT_H

#include <QMainWindow>

namespace Ui {
class MainWindowClient;
}

class DialogChooseRoom;
class DialogChooseGeneral;
class DialogSelect;
class DialogPlayerCard;

class MainWindowClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowClient(QWidget *parent = 0);
    ~MainWindowClient();
    void init();
    int closeClient(QWidget *);
    void hideAll();
    DialogChooseRoom *dialogChooseRoom;
    DialogChooseGeneral *dialogChooseGeneral;
    DialogSelect *dialogSelect;
    DialogPlayerCard *dialogPlayerCard;
    Ui::MainWindowClient *ui;
    uint roomID;

private:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOWCLIENT_H
