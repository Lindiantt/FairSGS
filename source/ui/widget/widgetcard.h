#ifndef WIDGETCARD_H
#define WIDGETCARD_H

#include <QWidget>

class CCard;
class QGraphicsOpacityEffect;

namespace Ui {
class WidgetCard;
}

class WidgetCard : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetCard(QWidget *parent = 0);
    ~WidgetCard();
    void setCard(CCard*);
    void setSelectable(bool);
    bool isOn;
    CCard* card;
    bool selectable;
    QGraphicsOpacityEffect *opEffect;
signals:
    void clicked();
private:
    Ui::WidgetCard *ui;
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
};

#endif // WIDGETCARD_H
