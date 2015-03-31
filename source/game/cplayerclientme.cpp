#include "cplayerclientme.h"
#include "ui/widget/widgetgame.h"
#include "ui_widgetgame.h"
#include "mainwindow.h"
extern MainWindow *w;
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "thread/cimage.h"
#include "general/cgeneral.h"
#include "ui/dialog/dialogselect.h"
#include "game/cgameclient.h"
#include "network/define.h"
#include "game/coperation.h"
#include "network/cclient.h"
#include "general/cskill.h"

#define GAME ((CGameClient *)game)
#define COMPARE(a) if(a){w->client->end2("与服务器断开连接");return;}

CPlayerClientMe::CPlayerClientMe()
{

}

CPlayerClientMe::~CPlayerClientMe()
{

}

void CPlayerClientMe::startTimeCount(int time,bool useExtreTime)
{
    GAME->widgetGame->ui->widgetTimeCount->startTimeCount(time,useExtreTime);
}

void CPlayerClientMe::stopTimeCount()
{
    GAME->widgetGame->ui->widgetTimeCount->stop();
}

void CPlayerClientMe::setGeneral(CGeneral *gen)
{
    CPlayer::setGeneral(gen);
    CImage *img=CImage::avatarById(gen->id);
    if(!img->loaded)
        w->imageLoad(img,w->mwCLient->ui->widgetBottom);
}

void CPlayerClientMe::setKingdom(quint8 kd)
{
    CPlayer::setKingdom(kd);
    w->mwCLient->ui->widgetBottom->setKingdom(kd);
}

void CPlayerClientMe::setRole(quint8 r)
{
    CPlayer::setRole(r);
    w->mwCLient->ui->widgetBottom->setRole(r);
}

void CPlayerClientMe::setMaxHP(quint8 hp)
{
    CPlayer::setMaxHP(hp);
    w->mwCLient->ui->widgetBottom->setMaxHP(hp);
}

void CPlayerClientMe::setHP(char hp)
{
    CPlayer::setHP(hp);
    w->mwCLient->ui->widgetBottom->setHP(hp);
}

int CPlayerClientMe::needSelect(const QString &question, int selectionType, const QStringList option)
{
    GAME->addFunc([](){});
    auto f=std::bind([&](const QString question, int selectionType, const QStringList option){
        QByteArray ba=GAME->serverData.takeFirst();
        COMPARE(ba[0]!=GAME_OPERATION);
        opSelect=COperation::serverRequest(ba);
        COMPARE(!opSelect);
        COMPARE(opSelect->type!=OPERATION_SELECT);
        switch (selectionType) {
        case SELECTTYPE_YESNO:

            break;
        case SELECTTYPE_WUXIEKEJI:
            break;
        default:
        {
            if(!w->mwCLient->dialogSelect)
            {
                w->mwCLient->dialogSelect=new DialogSelect(w->mwCLient);
                connect(w->mwCLient->dialogSelect,DialogSelect::select,this,this->handleSelected);
            }
            w->mwCLient->dialogSelect->init(question,selectionType,option);
            this->startTimeCount(w->client->operationTimeout,true);
            GAME->insertFunc([&](){
                w->mwCLient->dialogSelect->selected=true;
                w->mwCLient->dialogSelect->hide();
                emit GAME->newData();
            });
        }
            break;
        }
    },question,selectionType,option);
    GAME->addFunc(f);
    GAME->addFunc([&](){
        opSelect->deleteLater();
        stopTimeCount();
        QByteArray ba=GAME->serverData.takeFirst();
        COperation *opr=COperation::newOperation(game,ba);
        COMPARE(!opr);
        COMPARE(opr->type!=OPERATION_SELECT);
        auto it=opr->reply.find(this->position);
        COMPARE(it==opr->reply.end());
        GAME->selection=it.value().second[0].toInt();
        emit GAME->newData();
    });
    return 0;
}

void CPlayerClientMe::handleSelected(int sid)
{
    this->stopTimeCount();
    QList<QVariant> list;
    list.append(sid);
    QByteArray sendbuf=opSelect->replyBuf(list);
    w->client->send(sendbuf);
}

void CPlayerClientMe::deliverCard(const QList<CCard *> &list)
{
    CPlayer::deliverCard(list);
    w->mwCLient->ui->widgetBottom->renewCardInfo();
}

void CPlayerClientMe::getCard(const QList<CCard *> &list, CPlayer *player)
{
    CPlayer::getCard(list,player);
    w->mwCLient->ui->widgetBottom->renewCardInfo();
}

void CPlayerClientMe::startPlayPhase()
{
    w->mwCLient->ui->widgetBottom->playPhaseInit();
}

void CPlayerClientMe::setSelectable(bool b)
{
    w->mwCLient->ui->widgetBottom->setSelectable(b);
    selectable=b;
}

void CPlayerClientMe::setSelected(bool b)
{
    w->mwCLient->ui->widgetBottom->setSelected(b);
    selected=b;
}

void CPlayerClientMe::setNoSelect()
{
    w->mwCLient->ui->widgetBottom->setNoSelect();
}

void CPlayerClientMe::useCard(CCard *card,QList<CPlayer*> &list)
{
    CPlayer::useCard(card,list);
    hands.removeOne(card);
    w->mwCLient->ui->widgetBottom->renewCardInfo();
    GAME->widgetGame->displayCard(card,this);
}
