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
    void moveTo(quint8 pos);
    QHostAddress hostAddress;
    bool udpAvailable,ready,roomHost,admin;
    quint16 udpPort,favorite;
    quint8 pos;
    QString nick;
    int udpCode;
    bool empty;

private:
    Ui::WidgetRoomPlayer *ui;

protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
private slots:
    void handleContextMenu(QPoint);
};

#endif // WidgetRoomPLAYER_H
