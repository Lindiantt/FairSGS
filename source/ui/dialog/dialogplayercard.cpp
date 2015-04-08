#include "dialogplayercard.h"
#include "ui_dialogplayercard.h"
#include "game/cplayer.h"
#include "card/ccard.h"

DialogPlayerCard::DialogPlayerCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPlayerCard)
{
    ui->setupUi(this);
}

DialogPlayerCard::~DialogPlayerCard()
{
    delete ui;
}

void DialogPlayerCard::init(CPlayer *player, bool h, bool z, bool p)
{
    finished=false;
    this->player=player;
    int i=0;
    if(h&&!player->hands.isEmpty())
    {
        ui->pushButtonHands->setText("手牌 "+QString::number(player->hands.size())+"张");
        ui->pushButtonHands->setEnabled(true);
        pos[0]=i;
        i++;
    }
    else
    {
        ui->pushButtonHands->setText("手牌");
        ui->pushButtonHands->setEnabled(false);
    }
    if(z&&player->zhuangBei[0])
    {
        ui->pushButtonWuQi->setText("武器 "+player->zhuangBei[0]->description());
        ui->pushButtonWuQi->setEnabled(true);
        pos[1]=i;
        i++;
    }
    else
    {
        ui->pushButtonWuQi->setText("武器");
        ui->pushButtonWuQi->setEnabled(false);
    }
    if(z&&player->zhuangBei[1])
    {
        ui->pushButtonFangJu->setText("防具 "+player->zhuangBei[1]->description());
        ui->pushButtonFangJu->setEnabled(true);
        pos[2]=i;
        i++;
    }
    else
    {
        ui->pushButtonFangJu->setText("防具");
        ui->pushButtonFangJu->setEnabled(false);
    }
    if(z&&player->zhuangBei[2])
    {
        ui->pushButtonJiaYi->setText("+1马 "+player->zhuangBei[2]->description());
        ui->pushButtonJiaYi->setEnabled(true);
        pos[3]=i;
        i++;
    }
    else
    {
        ui->pushButtonJiaYi->setText("+1马");
        ui->pushButtonJiaYi->setEnabled(false);
    }
    if(z&&player->zhuangBei[3])
    {
        ui->pushButtonJianYi->setText("-1马 "+player->zhuangBei[3]->description());
        ui->pushButtonJianYi->setEnabled(true);
        pos[4]=i;
        i++;
    }
    else
    {
        ui->pushButtonJianYi->setText("-1马");
        ui->pushButtonJianYi->setEnabled(false);
    }
    if(z&&player->zhuangBei[4])
    {
        ui->pushButtonBaoWu->setText("宝物 "+player->zhuangBei[4]->description());
        ui->pushButtonBaoWu->setEnabled(true);
        pos[5]=i;
        i++;
    }
    else
    {
        ui->pushButtonBaoWu->setText("宝物");
        ui->pushButtonBaoWu->setEnabled(false);
    }
    if(p&&player->panDingQu[0])
    {
        ui->pushButtonShanDian->setText("闪电 "+player->panDingQu[0]->description());
        ui->pushButtonShanDian->setEnabled(true);
        pos[6]=i;
        i++;
    }
    else
    {
        ui->pushButtonShanDian->setText("闪电");
        ui->pushButtonShanDian->setEnabled(false);
    }
    if(p&&player->panDingQu[1])
    {
        ui->pushButtonLeBuSiShu->setText("乐不思蜀 "+player->panDingQu[1]->description());
        ui->pushButtonLeBuSiShu->setEnabled(true);
        pos[7]=i;
        i++;
    }
    else
    {
        ui->pushButtonLeBuSiShu->setText("乐不思蜀");
        ui->pushButtonLeBuSiShu->setEnabled(false);
    }
    if(p&&player->panDingQu[2])
    {
        ui->pushButtonBingLiangCunDuan->setText("兵粮寸断 "+player->panDingQu[2]->description());
        ui->pushButtonBingLiangCunDuan->setEnabled(true);
        pos[8]=i;
        i++;
    }
    else
    {
        ui->pushButtonBingLiangCunDuan->setText("兵粮寸断");
        ui->pushButtonBingLiangCunDuan->setEnabled(false);
    }
}

void DialogPlayerCard::on_pushButtonHands_clicked()
{
    buttonClick(0);
}

void DialogPlayerCard::on_pushButtonWuQi_clicked()
{
    buttonClick(1);
}

void DialogPlayerCard::on_pushButtonFangJu_clicked()
{
    buttonClick(2);
}

void DialogPlayerCard::on_pushButtonJiaYi_clicked()
{
    buttonClick(3);
}

void DialogPlayerCard::on_pushButtonJianYi_clicked()
{
    buttonClick(4);
}

void DialogPlayerCard::on_pushButtonBaoWu_clicked()
{
    buttonClick(5);
}

void DialogPlayerCard::on_pushButtonShanDian_clicked()
{
    buttonClick(6);
}

void DialogPlayerCard::on_pushButtonLeBuSiShu_clicked()
{
    buttonClick(7);
}

void DialogPlayerCard::on_pushButtonBingLiangCunDuan_clicked()
{
    buttonClick(8);
}

void DialogPlayerCard::closeEvent(QCloseEvent *)
{
    if(!finished)
        emit selected(0);
}

void DialogPlayerCard::buttonClick(int i)
{
    finished=true;
    emit selected(pos[i]);
    close();
}
