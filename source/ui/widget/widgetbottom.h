#ifndef WIDGETBOTTOM_H
#define WIDGETBOTTOM_H

#include <QWidget>

namespace Ui {
class WidgetBottom;
}

class QLabel;
class CPlayerClientMe;
class WidgetCard;
class CCard;
class CPlayer;

#define UIMODE_USECARD 1
#define UIMODE_USESKILL 2


class WidgetBottom : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBottom(QWidget *parent = 0);
    ~WidgetBottom();
    void init();
    void setMaxHP(int hp);
    void setHP(int hp);
    void setKingdom(int kingdom);
    void setHands(int);
    void setRole(int);
    void renewCardInfo();
    void setCardSelectable();
    void playPhaseInit();
    void endSelect();
    void setSelected(bool);
    void setSelectable(bool);
    void setNoSelect();
    void handsAllOff();
    QList<WidgetCard*> hands;
    int maxSelection,selectedCards;
    bool handSelectable,zhuangBeiSelectable;
    bool avatarOpacity;
    CPlayerClientMe *me;

private slots:
    void on_labelRole_clicked();
    void handleHandsClick();

private:
    void paintEvent(QPaintEvent *);
    void arrangeBloods();
    void arrangeCards();
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent*);

    Ui::WidgetBottom *ui;
    QLabel *labelBloods[5];
    bool roleHiden;
};

#endif // WIDGETBOTTOM_H
