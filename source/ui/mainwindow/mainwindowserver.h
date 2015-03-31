#ifndef MAINWINDOWSERVER_H
#define MAINWINDOWSERVER_H


#include <QMainWindow>



namespace Ui {
class MainWindowServer;
}

class MainWindowServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowServer(QWidget *parent = 0);
    ~MainWindowServer();
    void hideAll();
    Ui::MainWindowServer *ui;

private:

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOWSERVER_H
