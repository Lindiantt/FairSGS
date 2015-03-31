#ifndef WIDGETZHUANGBEIFULL_H
#define WIDGETZHUANGBEIFULL_H

#include "ui/widget/widgetzhuangbeismall.h"

class WidgetZhuangBeiFull : public WidgetZhuangBeiSmall
{
    Q_OBJECT
public:
    explicit WidgetZhuangBeiFull(QWidget *parent = 0);
    ~WidgetZhuangBeiFull();
    void setCard(CCard*);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *);
    QPixmap pixmapName;
};

#endif // WIDGETZHUANGBEIFULL_H
