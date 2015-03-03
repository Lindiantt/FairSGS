#ifndef WIDGETROOMBACKGROUND_H
#define WIDGETROOMBACKGROUND_H

#include <QWidget>

class WidgetRoomBackground : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetRoomBackground(QWidget *parent = 0);
    ~WidgetRoomBackground();

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    int oldw;
};

#endif // WIDGETROOMBACKGROUND_H
