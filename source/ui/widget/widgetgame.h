#ifndef WIDGETGAME_H
#define WIDGETGAME_H

#include <QWidget>

namespace Ui {
class WidgetGame;
}

class WidgetGamePlayer;
class CGameClient;
class CPlayerClientMe;
class QProgressBar;
class CPlayer;
class CCard;

class WidgetGame : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetGame(QWidget *parent = 0);
    ~WidgetGame();
    void init();
    bool rhPlaying(QByteArray &);
    void displayTip(const QString &);
    void hideTip();
    void playerSelectionInit();
    void playerSelectionNormal();
    void displayCard(CCard* card,CPlayer* from=nullptr);
    QVector<WidgetGamePlayer*> players;
    void arrangePlayers();
    QList<CPlayer*> currentSelectedTargets();
    CGameClient *game;
    Ui::WidgetGame *ui;

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonOK_clicked();

private:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // WIDGETGAME_H
