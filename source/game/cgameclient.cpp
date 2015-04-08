#include "cgameclient.h"

#include "mainwindow.h"
extern MainWindow *w;

#include "network/cclient.h"
#include "ui/widget/widgetgame.h"
#include "network/define.h"
#include "game/cplayerclientme.h"
#include "ui/mainwindow/mainwindowclient.h"
#include "ui_mainwindowclient.h"
#include "ui/widget/widgetgameplayer.h"
#include "general/cgeneral.h"
#include "ui/dialog/dialogchoosegeneral.h"
#include "game/coperation.h"
#include "ui_widgetgame.h"
#include "ui/widget/widgetroomplayer.h"
#include "ui_widgetgameplayer.h"
#include "general/cskill.h"
#include "card/ccard.h"
#include "game/cevent.h"

#define COMPARE(a) if(a){w->client->end2("与服务器断开连接");return;}
#define PLAYER ((CPlayerClient*)player)

CGameClient::CGameClient(WidgetGame *game)
{
    this->widgetGame=game;
    me=nullptr;
    players.resize(w->client->numberOfPlayer);
    connect(this,this->startGame,this,this->s1needRole,Qt::QueuedConnection);
    isServer=false;
    twoStage=false;
    twoStageLock=0;
    currentCard=nullptr;
    //connect(this,this->newData,this,this->test);
}

CGameClient::~CGameClient()
{
    w->mwCLient->ui->widgetGame->game=nullptr;
}

void CGameClient::s1needRole()
{
    WidgetGamePlayer *pl;
    foreach (pl, widgetGame->players) {
        pl->moveWidgets();
    }
    addFunc([&](){
        QByteArray ba=serverData.takeFirst();
        COMPARE(ba[0]!=GAME_INIT);
        COMPARE(ba.length()!=w->client->numberOfPlayer+2);
        quint8 c=ba[1];
        COMPARE(c>3);
        me=new CPlayerClientMe();
        w->mwCLient->ui->widgetBottom->me=me;
        me->setRole(c);
        for(int i=0;i<w->client->numberOfPlayer;i++)
        {
            c=ba[i+2];
            COMPARE(c>=w->client->numberOfPlayer);
            if(c==w->mwCLient->ui->widgetRoom->me)
            {
                players[i]=me;
                me->position=i;
            }
            else
            {
                CPlayerClient *player=new CPlayerClient;
                players[i]=player;
                player->position=i;
            }
            auto player=players[i];
            PLAYER->wrp=w->mwCLient->ui->widgetRoom->players[c];
            players[i]->game=this;
        }
        int cp=me->position-1;
        for(int i=0;i<w->client->numberOfPlayer-1;i++)
        {
            if(cp<0) cp=w->client->numberOfPlayer-1;
            auto player=players[cp];
            widgetGame->players[i]->setPlayer(PLAYER);
            PLAYER->wgp=widgetGame->players[i];
            PLAYER->wgp->ui->labelNick->setText(PLAYER->wrp->nick);
            if(cp)
                players[cp]->setRole(ROLE_UNKNOWN);
            else
                players[cp]->setRole(ROLE_ZHU);
            cp--;
        }
        COMPARE((me->position==0)^(me->role==ROLE_ZHU));
        s2needChooseGeneral();
    });
}

void CGameClient::s2needChooseGeneral()
{
    if(me->role==ROLE_ZHU)
    {
        addFunc([&](){
            QByteArray ba=serverData.takeFirst();
            COMPARE(!bhChooseGeneral(ba));
        });
        addFunc([&](){
            QByteArray ba=serverData.takeFirst();
            COMPARE(!bhGeneralChosen(ba));
            me->stopTimeCount();
            widgetGame->displayTip("等待其他人选将");
            for(int i=1;i<w->client->numberOfPlayer;i++)
            {
                auto player=players[i];
                PLAYER->startTimeCount(w->client->choiceTimeout,false);
            }
        });
    }
    else
    {
        auto player=players[0];
        PLAYER->startTimeCount(w->client->choiceTimeout,false);
        widgetGame->displayTip("等待主公选将");
        addFunc([&](){
            widgetGame->hideTip();
            QByteArray ba=serverData.takeFirst();
            auto player=players[0];
            PLAYER->stopTimeCount();
            COMPARE(!bhGeneralChosen(ba));
            for(int i=1;i<w->client->numberOfPlayer;i++)
            {
                if(i!=me->position)
                {
                    auto player=players[i];
                    PLAYER->startTimeCount(w->client->choiceTimeout,false);
                }
            }
        });
        addFunc([&](){
            QByteArray ba=serverData.takeFirst();
            COMPARE(!bhChooseGeneral(ba));
        });
    }
    for(int i=1;i<w->client->numberOfPlayer;i++)
    {
        auto f=std::bind([&](int n){
            QByteArray ba=serverData.takeFirst();
            COMPARE(!bhGeneralChosen(ba));
            if(n==w->client->numberOfPlayer-1)
            {
                widgetGame->hideTip();
                for(int i=1;i<w->client->numberOfPlayer;i++)
                {
                    auto player=players[i];
                    PLAYER->stopTimeCount();
                }
                s3needAllGeneralAndCards();
            }
        },i);
        addFunc(f);
    }
}

void CGameClient::s3needAllGeneralAndCards()
{
    addFunc([&](){
        QByteArray ba=serverData.takeFirst();
        COMPARE(ba[0]!=GAME_CARDINIT);
        COMPARE(ba.length()!=5);
        QList<CCard*> list;
        for(int i=1;i<5;i++)
        {
            quint8 c=ba[i];
            CCard* card=CCard::find(c);
            COMPARE(!card);
            list.append(card);
        }
        me->deliverCard(list);
        for(int i=0;i<4;i++)
        {
            list[i]=nullptr;
        }
        for(int i=0;i<w->client->numberOfPlayer;i++)
        {
            if(i!=me->position)
                players[i]->deliverCard(list);
        }
        s4skillSetup();
    });
}

void CGameClient::needCard(int n, const QList<CPlayer *> &ntk)
{
    if(ntk.contains(me))
    {
        auto f=std::bind([&](int n){
            QByteArray ba=serverData.takeFirst();
            COMPARE(ba[0]!=GAME_OPERATIONRESULT);
            COperation *op=COperation::newOperation(this,ba);
            COMPARE(!op||op->type!=OPERATION_NEEDCARD||op->parameter.size()!=n);
            drawnCards.resize(n);
            CCard *card;
            for(int i=0;i<n;i++)
            {
                card=CCard::find(op->parameter[i].toUInt());
                COMPARE(!card);
                drawnCards[i]=card;
            }
            emit newData();
        },n);
        smartInsertFunc(f);
    }
    else
    {
        drawnCards.resize(n);
        emit newData();
    }
}

void CGameClient::needWuXieKeJi()
{
    CGame::needWuXieKeJi();
    CEvent *ev=new CEvent(this);
    if(me->hasWuXieKeJi())
    {
        ev->addFunc([&](){
            QByteArray ba=serverData.takeFirst();
            opWuXie=COperation::serverRequest(ba);
            COMPARE(!opWuXie||opWuXie->type!=OPERATION_WUXIEKEJI);
            CPlayer* player;
            foreach (player, players) {
                PLAYER->startTimeCount(w->client->wuXieTimeout,false);
            }
            QList<quint8> type2;
            type2.append(CARD_WUXIEKEJI);
            me->opSelect=opWuXie;
            w->mwCLient->ui->widgetBottom->setSelectMode(0xff,0,type2,1,CARDMODE_HANDS);
        });
    }
    else
        opWuXie=nullptr;
    ev->addFunc([&](){
        CEvent *ev2=new CEvent(this);
        w->mwCLient->ui->widgetBottom->endSelect();
        if(opWuXie) opWuXie->deleteLater();
        CPlayer *player;
        foreach (player, players) {
            PLAYER->stopTimeCount();
        }
        QByteArray ba=serverData.takeFirst();
        COperation *op=COperation::newOperation(this,ba);
        bool b=false;
        foreach (auto pair, op->reply) {
            QList<QVariant> &list=pair.second;
            if(!list.isEmpty()&&list[0].toInt()!=-1)
            {
                int n=list[0].toInt();
                if(n==10000)
                {
                    //使用技能
                    emit newData();
                    return;
                }
                b=true;
                CCard* card=CCard::find(list[0].toInt());
                COMPARE(!card);
                quint8 pos=pair.first;
                COMPARE(pos>=w->client->numberOfPlayer);
                players[pos]->removeHand(card);
                QList<CCard*> cards;
                cards.append(card);
                players[pos]->phaseCallback(PHASE_LOSECARD,&cards,(void*)LOSECARDREASON_PLAY);
                break;
            }
        }
        if(b)
        {
            wuxiePlayed=true;
            wuxieAvailable=!wuxieAvailable;
            auto f=std::bind(needWuXieKeJi,this);
            ev2->addFunc(f);
        }
        emit newData();
    });
}

void CGameClient::startNextRound()
{

}

bool CGameClient::bhGeneralChosen(QByteArray &ba)
{
    if(ba[0]!=GAME_GENERALCHOSEN) return false;
    if(ba.length()!=4) return false;
    quint8 c=ba[1];
    if(c>=w->client->numberOfPlayer) return false;
    quint16 id;
    memcpy(&id,ba.data()+2,2);
    id=qFromLittleEndian(id);
    CGeneral *gen=CGeneral::find(id);
    if(!gen) return false;
    if(me->position==c)
    {
        generalChosen=true;
        if(w->mwCLient->dialogChooseGeneral) w->mwCLient->dialogChooseGeneral->close();
    }
    players[c]->setGeneral(gen);
    return true;
}

bool CGameClient::bhChooseGeneral(QByteArray &ba)
{
    if(ba[0]!=GAME_CHOOSEGENERAL) return false;
    if(ba.length()<5) return false;
    int pos=2;
    quint8 c=ba[1];
    if(!c) return false;
    QList<quint16> gids;
    quint16 id;
    for(int i=0;i<c;i++)
    {
        if(ba.length()<pos+3) return false;
        memcpy(&id,ba.data()+pos,2);
        id=qFromLittleEndian(id);
        gids.append(id);
        quint8 dn=ba[pos+2];
        if(dn&&ba.length()<pos+3+2*dn) return false;
        for(int j=0;j<dn;j++)
        {
            memcpy(&id,ba.data()+pos+3+2*j,2);
            id=qFromLittleEndian(id);
            gids.append(id);
        }
        pos+=3+2*dn;
    }
    if(!w->mwCLient->dialogChooseGeneral)
    {
        w->mwCLient->dialogChooseGeneral=new DialogChooseGeneral(w->mwCLient);
        connect(w->mwCLient->dialogChooseGeneral,SIGNAL(chosen(int)),this,SLOT(handleGeneralChosen(int)));
    }
    QString s;
    if(me->role==ROLE_ZHU)
        s="您的身份是主公，";
    else
    {
        if(!players[0]->general) return false;
        s="主公选择了"+players[0]->general->name+"，您是"+QString::number(me->position+1)+"号位"+CGeneral::roleName(me->role)
                +"，";
    }
    w->mwCLient->dialogChooseGeneral->init(gids,s);
    defaultId=gids[0];
    generalChosen=false;
    w->mwCLient->dialogChooseGeneral->show();
    me->startTimeCount(w->client->choiceTimeout,false);
    return true;
}

void CGameClient::handleGeneralChosen(int id)
{
    me->stopTimeCount();
    if(!generalChosen)
    {
        generalChosen=true;
        QByteArray sendbuf;
        sendbuf.resize(5);
        sendbuf[2]=GAME_CHOOSEGENERAL;
        if(id==-1)
            id=qToLittleEndian(defaultId);
        else
            id=qToLittleEndian(id);
        memcpy(sendbuf.data()+3,&id,2);
        w->client->send(sendbuf);
    }
}

void CGameClient::cardDirection(CCard *card)
{
    currentCard=card;
    if(card->type->type2==CARD_JIEDAOSHAREN)
    {
        twoStage=true;
        twoStageLock=0;
    }
    else
        twoStage=false;
    widgetGame->playerSelectionInit();
    auto list=card->type->availableTargets(me,card);
    CPlayer *player;
    foreach (player, list) {
        PLAYER->setSelectable(true);
    }
    targetMin=card->type->minTargets;
    targetMax=card->type->maxTargets;
    me->phaseCallback(PHASE_CARDMINMAXTARGETS,card,&targetMin,&targetMax);
    if(!card->type->canSelectTarget||list.size()==targetMin)
    {
        canSelectTarget=false;
        foreach (player, list) {
            PLAYER->setSelected(true);
        }
        if(twoStage)
        {
            twoStageLock=1;
            checkSecondStage();
        }
    }
    else
    {
        canSelectTarget=true;
    }
    widgetGame->ui->pushButtonCancel->setEnabled(true);
    cardOKCheck();
}

void CGameClient::cardOKCheck()
{
    auto list=w->mwCLient->ui->widgetGame->currentSelectedTargets();
    if(currentCard->type->cardUseCheck(me,currentCard,list))
    {
        widgetGame->ui->pushButtonOK->setEnabled(true);
    }
    else
        widgetGame->ui->pushButtonOK->setEnabled(false);
}

void CGameClient::checkSecondStage()
{
    auto list=widgetGame->currentSelectedTargets();
    CPlayer* player;
    if(mode==UIMODE_USECARD)
    {
        CTempCard tcard(w->cardType[CARD_PUTONGSHA]);
        auto alist=tcard.type->availableTargets(list[0],&tcard);
        foreach (player, players) {
            if(list[0]==player) continue;
            PLAYER->setSelectable(alist.contains(player));
        }
    }
}

void CGameClient::targetClicked(CPlayer *player)
{
    if(twoStage)//双段选择模式（如借刀杀人、离间）
    {
        auto list=widgetGame->currentSelectedTargets();
        if(list.size()==0)
        {
            PLAYER->setSelected(true);
            firstTarget=player;
            checkSecondStage();
        }
        else if(list.size()==1)
        {
            if(PLAYER->selected)//回到初始阶段
            {
                PLAYER->setSelected(false);
                cardDirection(currentCard);
            }
            else//完成2阶段
            {
                PLAYER->setSelected(true);
                secondTarget=player;
            }
        }
        else//在二阶段已完成的情况下
        {
            if(!PLAYER->selected) return;
            if(firstTarget==player)//回到初始阶段
                cardDirection(currentCard);
            else//回到第二阶段
            {
                PLAYER->setSelected(false);
            }
        }
    }
    else
    {
        if(!canSelectTarget) return;
        if(PLAYER->selected)
        {
            PLAYER->setSelected(false);
        }
        else
        {
            if(widgetGame->currentSelectedTargets().size()>=targetMax)
            {
                if(targetMax==1)
                {
                    CPlayerClient *pc=static_cast<CPlayerClient*>(widgetGame->currentSelectedTargets()[0]);
                    pc->setSelected(false);
                    PLAYER->setSelected(true);
                }
                else
                    return;
            }
            PLAYER->setSelected(true);
        }
    }
    cardOKCheck();
}
