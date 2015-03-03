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
    bool rhEnterRoom();
    void init();
    void clear();
    void arrangePlayers();
    QVector<WidgetRoomPlayer*> players;

private:
    Ui::WidgetRoom *ui;
};

#endif // WidgetRoom_H
