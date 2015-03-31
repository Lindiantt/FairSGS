#ifndef WidgetRoom_H
#define WidgetRoom_H

#include <QWidget>

namespace Ui {
class WidgetRoom;
}

class QLabel;

class WidgetRoomPlayer;

class WidgetRoom : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetRoom(QWidget *parent = 0);
    ~WidgetRoom();
    bool rhEnterRoom(QByteArray &);
    bool rhInRoom(QByteArray &);
    bool rhStartGame(QByteArray &);
    void init();
    void clear();
    void arrangePlayers();
    void buttonInit(bool isPlayer,bool isRoomHost);
    QVector<WidgetRoomPlayer*> players;
    bool disable;
    char me;


private slots:
    void on_pushButtonReady_clicked(bool checked);
    void on_pushButtonReturn_clicked();

    void on_pushButtonSitDown_clicked();

    void on_pushButtonStart_clicked();

private:
    Ui::WidgetRoom *ui;
    int readPlayerInfo(QByteArray &,int);
};

#endif // WidgetRoom_H
