#ifndef DIALOGCHOOSEROOM_H
#define DIALOGCHOOSEROOM_H

#include <QDialog>

namespace Ui {
class DialogChooseRoom;
}


class DialogChooseRoom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseRoom(QWidget *parent = 0);
    ~DialogChooseRoom();
    bool init(uint page,QByteArray &);
    bool rhChooseRoom();
    uint page;
    uint id;

private:
    Ui::DialogChooseRoom *ui;
    void joinRoom(uint id);
    void onlookRoom(uint id);
    void closeEvent(QCloseEvent *);
private slots:
    void handleOnlookButton();
    void on_tableWidgetRooms_cellDoubleClicked(int row, int column);
    void on_pushButtonNewRoom_clicked();
    void on_pushButtonFind_clicked();
    void on_pushButtonRefresh_clicked();
    void on_pushButtonFirst_clicked();
    void on_pushButtonLast_clicked();
    void on_pushButtonPrev_clicked();
    void on_pushButtonNext_clicked();
    void on_pushButtonJoin_clicked();
    void on_pushButtonOnlook_clicked();
};

#endif // DIALOGCHOOSEROOM_H
