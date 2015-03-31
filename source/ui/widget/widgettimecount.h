#ifndef WIDGETTIMECOUNT_H
#define WIDGETTIMECOUNT_H

#include <QWidget>

namespace Ui {
class WidgetTimeCount;
}

class WidgetGamePlayer;

class WidgetTimeCount : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTimeCount(QWidget *parent = 0);
    WidgetTimeCount(WidgetGamePlayer *player,QWidget *parent = 0);
    ~WidgetTimeCount();
    void startTimeCount(int time,bool);
    void stop();
    WidgetGamePlayer* player;
    QTimer timer;
public slots:
    void handleTimeout();
private:
    Ui::WidgetTimeCount *ui;
    bool useExtreTime;
};

#endif // WIDGETTIMECOUNT_H
