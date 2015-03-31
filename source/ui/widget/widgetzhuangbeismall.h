#ifndef WIDGETZHUANGBEISMALL_H
#define WIDGETZHUANGBEISMALL_H

#include <QWidget>
class QLabel;
class CCard;

class WidgetZhuangBeiSmall : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetZhuangBeiSmall(QWidget *parent = 0);
    ~WidgetZhuangBeiSmall();
    virtual void setCard(CCard*);
    QLabel *labelIcon,*labelSuit;

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *);
    QPixmap pixmapDistance,pixmapPoint;
    CCard* card;
};

#endif // WIDGETZHUANGBEISMALL_H
