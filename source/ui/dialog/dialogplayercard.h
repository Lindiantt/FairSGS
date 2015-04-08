#ifndef DIALOGPLAYERCARD_H
#define DIALOGPLAYERCARD_H

#include <QDialog>

namespace Ui {
class DialogPlayerCard;
}

class CPlayer;

class DialogPlayerCard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPlayerCard(QWidget *parent = 0);
    ~DialogPlayerCard();
    void init(CPlayer*,bool h=true,bool z=true,bool p=true);
    bool finished;
signals:
    void selected(int);
private slots:
    void on_pushButtonHands_clicked();

    void on_pushButtonWuQi_clicked();

    void on_pushButtonFangJu_clicked();

    void on_pushButtonJiaYi_clicked();

    void on_pushButtonJianYi_clicked();

    void on_pushButtonBaoWu_clicked();

    void on_pushButtonShanDian_clicked();

    void on_pushButtonLeBuSiShu_clicked();

    void on_pushButtonBingLiangCunDuan_clicked();

private:
    Ui::DialogPlayerCard *ui;
    CPlayer *player;
    int pos[9];
    void closeEvent(QCloseEvent*);
    void buttonClick(int);
};

#endif // DIALOGPLAYERCARD_H
