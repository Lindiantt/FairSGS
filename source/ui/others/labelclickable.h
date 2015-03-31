#ifndef LABELCLICKABLE_H
#define LABELCLICKABLE_H

#include <qlabel.h>

class LabelClickable : public QLabel
{
    Q_OBJECT
public:
    LabelClickable(QWidget *parent);
    ~LabelClickable();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // LABELCLICKABLE_H
