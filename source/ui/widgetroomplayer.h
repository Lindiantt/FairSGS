#ifndef WidgetRoomPLAYER_H
#define WidgetRoomPLAYER_H

#include <QWidget>

namespace Ui {
class WidgetRoomPlayer;
}

class WidgetRoomPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetRoomPlayer(quint8 pos,QWidget *parent = 0);
    ~WidgetRoomPlayer();
    void setFavorite(quint16);
    void setRoomHost();
    void setAdmin();
    void setReady(bool b);
    void setNick(const QString &);
    void init();
    QHostAddress hostAddress;
    bool udpAvailable,ready,roomHost,admin,me;
    quint16 udpPort,favorite;
    quint8 pos;
    QString nick;
    int udpCode;
    bool empty;

private:
    Ui::WidgetRoomPlayer *ui;
};

#endif // WidgetRoomPLAYER_H
