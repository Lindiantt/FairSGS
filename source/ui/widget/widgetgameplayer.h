#ifndef WIDGETGAMEPLAYER_H
#define WIDGETGAMEPLAYER_H

#include <QWidget>

namespace Ui {
class WidgetGamePlayer;
}

class CPlayerClient;
class WidgetTimeCount;
class QLabel;
class QGraphicsOpacityEffect;

class WidgetGamePlayer : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetGamePlayer(QWidget *parent = 0);
    ~WidgetGamePlayer();
    void setPlayer(CPlayerClient*);
    void setMaxHp(quint8);
    void setHp(signed char);
    void setHands(int);
    void setSelected(bool);
    void setSelectable(bool);
    void setNoSelect();
    void moveWidgets();
    CPlayerClient *player;
    WidgetTimeCount *widgetTimeCount;
    QLabel *labelFrame;
    Ui::WidgetGamePlayer *ui;
    QGraphicsOpacityEffect *opEffect;

private slots:
    void on_labelRole_clicked();

    void on_labelRoleZhong_clicked();

    void on_labelRoleFan_clicked();

    void on_labelRoleNei_clicked();

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void makePixmap();
    void hideRole();
    QPixmap pixmapName;
    QVector<QLabel*> labelBloods;
};

#endif // WIDGETGAMEPLAYER_H
