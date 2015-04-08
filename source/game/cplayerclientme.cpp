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
#include "game/cevent.h"
#include "ui/dialog/dialogplayercard.h"

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

void CPlayerClientMe::needSelect(const QString &question,int selectionType,const QList<QVariant> values,
                                 const QList<QVariant> &defaultResult,int min,int max)
{
    CEvent *ev=new CEvent(game);
    ev->addFunc([](){});
    auto f=std::bind([&](const QString question, int selectionType, const QList<QVariant> values,
                     const QList<QVariant> &defaultResult,int min,int max){
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
        case SELECTTYPE_PLAYERCARD:
        {
            if(!w->mwCLient->dialogPlayerCard)
            {
                w->mwCLient->dialogPlayerCard=new DialogPlayerCard(w->mwCLient);
                connect(w->mwCLient->dialogPlayerCard,DialogPlayerCard::selected,this,this->handleSelected);
            }
            int i=values[0].toInt();
            CPlayer *target=game->players[i];
            w->mwCLient->dialogPlayerCard->init(target);
            startTimeCount(w->client->operationTimeout,true);
            game->smartInsertFunc([&](){
                w->mwCLient->dialogPlayerCard->finished=true;
                w->mwCLient->dialogPlayerCard->hide();
                emit GAME->newData();
            });
        }
            break;
        case SELECTTYPE_CUSTOM:
        {
            if(!w->mwCLient->dialogSelect)
            {
                w->mwCLient->dialogSelect=new DialogSelect(w->mwCLient);
                connect(w->mwCLient->dialogSelect,DialogSelect::select,this,this->handleSelected);
            }
            QVariant var;
            QStringList option;
            foreach (var, values) {
                option.append(var.toString());
            }
            w->mwCLient->dialogSelect->init(question,option);
            this->startTimeCount(w->client->operationTimeout,true);
            game->smartInsertFunc([&](){
                w->mwCLient->dialogSelect->selected=true;
                w->mwCLient->dialogSelect->hide();
                emit GAME->newData();
            });
        }
            break;
        default:
            break;
        }
    },question,selectionType,values,defaultResult,min,max);
    ev->addFunc(f);
    auto f2=std::bind([&](uint limit,int min,int max){
        opSelect->deleteLater();
        stopTimeCount();
        QByteArray ba=GAME->serverData.takeFirst();
        COMPARE(ba[0]!=GAME_OPERATIONRESULT);
        COperation *opr=COperation::newOperation(game,ba);
        COMPARE(!opr);
        COMPARE(opr->type!=OPERATION_SELECT);
        auto it=opr->reply.find(this->position);
        COMPARE(it==opr->reply.end());
        selectionList=it.value().second;
        COMPARE(selectionList.size()<min||selectionList.size()>max);
        QVariant var;
        foreach (var, selectionList) {
            COMPARE(var.toUInt()>=limit);
        }
        emit GAME->newData();
    },values.size(),min,max);
    ev->addFunc(f2);
    emit game->newData();
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
    foreach (auto card, list) {
        w->mwCLient->ui->widgetBottom->addHand(card);
    }
    CPlayer::getCard(list,player);
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

void CPlayerClientMe::showPhase(CImage *img)
{
    w->imageLoad(img,w->mwCLient->ui->widgetGame->ui->labelPhase);
}

void CPlayerClientMe::needPlay(quint8 suit, quint16 type, const QList<quint8> type2, int number, CPlayer *from, CCard *card,
     int playMode, int cardMode, const QList<CPlayer *> targets)
{
    CPlayer::needPlay(suit,type,type2,number,from,card,playMode,cardMode,targets);
    auto f=std::bind([&](quint16 type, const QList<quint8> type2, int number,int cardMode){
        QByteArray ba=GAME->serverData.takeFirst();
        COMPARE(ba[0]!=GAME_OPERATION);
        opSelect=COperation::serverRequest(ba);
        COMPARE(!opSelect||opSelect->type!=OPERATION_NEEDPLAY);
        w->mwCLient->ui->widgetBottom->setSelectMode(suit,type,type2,number,cardMode);
        startTimeCount(w->client->operationTimeout,true);
    },type,type2,number,cardMode);
    game->insertMultiFunc(f);
    game->insertMultiFunc(std::bind(needPlayCallback,this,suit,type,type2,number,from,card,playMode,cardMode,targets));
    game->smartInsertOver();
}

void CPlayerClientMe::playPhaseInit()
{
    w->mwCLient->ui->widgetBottom->playPhaseInit();
}

void CPlayerClientMe::roundInit()
{
    CPlayer::roundInit();
}

void CPlayerClientMe::removeHand(CCard *card)
{
    hands.removeOne(card);
    w->mwCLient->ui->widgetBottom->removeHand(card);
    GAME->widgetGame->displayCard(card,this);
}

void CPlayerClientMe::needShow()
{
    game->insertMultiFunc([&](){
        QByteArray ba=GAME->serverData.takeFirst();
        COMPARE(ba[0]!=GAME_OPERATION);
        opSelect=COperation::serverRequest(ba);
        COMPARE(!opSelect||opSelect->type!=OPERATION_NEEDSHOW);
        w->mwCLient->ui->widgetBottom->setSelectMode(0xff,0xffff,QList<quint8>(),1,CARDMODE_HANDS);
        w->client->game->mode=UIMODE_SHOWCARD;
    });
    game->insertMultiFunc(std::bind(needShowCallback,this));
    game->smartInsertOver();
}
