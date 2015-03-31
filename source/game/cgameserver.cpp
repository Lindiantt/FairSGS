#include "cgameserver.h"
#include "network/cserver.h"
#include "network/croom.h"
#include "cplayerserver.h"
#include "general/cgeneral.h"

#include "mainwindow.h"
extern MainWindow *w;

#include "coperation.h"

#define PLAYER ((CPlayerServer*)player)

CGameServer::CGameServer(CServer *server, CRoom *room)
{
    this->server=server;
    this->room=room;
    isServer=true;
}

CGameServer::~CGameServer()
{

}

void CGameServer::init()
{
    availableGenerals=server->availableGenerals;
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    for(int i=0;i<MAX_SHEN;i++)
    {
        bool b=false;
        switch(server->generalShen[i])
        {
        case 1://极低
            if(qrand()%100==1) b=true;
            break;
        case 2://低
            if(qrand()%100<10) b=true;
            break;
        case 3://一般
            b=true;
            break;
        default:
            b=false;
            break;
        }
        if(b)
        {
            availableGenerals[w->generalShen[i]->id].append(w->generalShen[i]);
        }
    }
    chosen=0;
    s1needRole();
}

void CGameServer::s1needRole()
{
    deck=server->availableCards;
    std::random_shuffle(deck.begin(),deck.end());
    CPlayerServer* player;
    foreach (CPlayerSocket* handle, room->players) {
        player=new CPlayerServer(this,handle);
        players.append(player);
    }
#ifdef QT_NO_DEBUG
    std::random_shuffle(players.begin(),players.end());
#endif
    QList<quint8> roles;
    roles=server->roles;
    std::random_shuffle(roles.begin(),roles.end());
    roles.prepend(ROLE_ZHU);
    int j=server->numberOfPlayer;
    int i;
    for(i=0;i<j;i++)
    {
        auto player=players[i];
        PLAYER->position=i;
        PLAYER->role=roles[i];
        PLAYER->sendGameStart();
    }
    QList<QList<CGeneral*>> pickZG;
    foreach (auto general, server->availableZhugong) {
        QList<CGeneral*> list;
        list.append(general);
        pickZG.append(list);
    }
#ifdef QT_DEBUG
    QList<CGeneral*> list;
    list.append(w->generalShen[3]);
    pickZG.append(list);
#endif
    if(server->choicesZG)
    {
        int l=availableGenerals.size();
        int pn=server->choicesZG;
        if(l<=pn)
        {
            foreach (auto gens, availableGenerals) {
                pickZG.append(gens);
            }
        }
        else
        {
            std::set<quint16> picks;
            for(;pn>0;pn--)
            {
                j=qrand()%l;
                foreach (quint16 pk, picks) {
                    if(j>=pk)
                        j++;
                    else
                        break;
                }
                auto it=availableGenerals.begin();
                std::advance(it,j);
                picks.insert(j);
                pickZG.append(it.value());
                l--;
            }
        }
    }
    players[0]->chooseGeneral(pickZG);
}

void CGameServer::s2needChooseGeneral()
{
    QList<QList<CGeneral*>> allGenerals,pickedGenerals,generalsForEachPlayer[server->numberOfPlayer-1];
    bool b=false;//是否已把主公选将剔除了
    foreach (CGeneral *general, server->availableZhugong) {
        if(b||general!=players[0]->general)
        {
            QList<CGeneral*> list;
            list.append(general);
            allGenerals.append(list);
        }
        else
            b=true;
    }
    foreach (auto list, availableGenerals) {
        if(!b)
        {
            foreach (CGeneral *general, list) {
                if(general==players[0]->general)
                {
                    list.removeOne(general);
                    b=true;
                    break;
                }
            }
        }
        if(!list.isEmpty())
        {
            allGenerals.append(list);
        }
    }
    int n=server->choices*(server->numberOfPlayer-1)+server->choicesNJ*server->numberOfNei;
    int i,j;
    if(allGenerals.length()-1<=n)//选将不够那么大家平分
    {
        i=0;j=0;
        b=false;//是否单独为内奸分配模式
        QList<int> nei;
        foreach (auto list, allGenerals) {
            if(b)
            {
                generalsForEachPlayer[nei[i]].append(list);
                i++;
                if(i>=nei.length()) i=0;
            }
            else
            {
                generalsForEachPlayer[i].append(list);
                i++;
                if(i>=server->numberOfPlayer-1)//分配了一轮的将
                {
                    i=0;
                    j++;
                    if(j==server->choices&&allGenerals.length()>j*server->numberOfPlayer)//其他人都分配完了，还有剩给内奸的
                    {
                        for(j=1;j<server->numberOfPlayer;j++)
                        {
                            if(players[j]->role==ROLE_NEI)
                            {
                                nei.append(j-1);
                                if(nei.length()>=server->numberOfNei)
                                    break;
                            }
                        }
                        b=true;
                    }
                }
            }
        }
    }
    else
    {
        std::set<quint16> picks;
        qsrand(QTime::currentTime().msecsSinceStartOfDay());
        int l=allGenerals.length();
        for(;n>0;n--)
        {
            j=qrand()%l;
            foreach (auto pk, picks) {
                if(j>=pk)
                    j++;
                else
                    break;
            }
            picks.insert(j);
            pickedGenerals.append(allGenerals[j]);
            l--;
        }
        j=server->numberOfPlayer-1;
        int start=0;
        for(l=0;l<j;l++)
        {
            i=server->choices;
            if(players[l+1]->role==ROLE_NEI)
                i+=server->choicesNJ;
            generalsForEachPlayer[l]=pickedGenerals.mid(start,i);
            start+=i;
        }
    }
    for(i=1;i<server->numberOfPlayer;i++)
    {
        players[i]->chooseGeneral(generalsForEachPlayer[i-1]);
    }
}

void CGameServer::s3needAllGeneralAndCards()
{
    foreach (auto player, players) {
        drawCard(4);
#ifdef QT_DEBUG
        //drawnCards[0]=w->cardJunzheng[11];
#endif
        PLAYER->deliverCard(getDrawnCards());
    }
    this->s4skillSetup();
}

void CGameServer::s4skillSetup()
{
    CGame::s4skillSetup();

    foreach (auto player, players) {
        if(PLAYER->kingdom==KINGDOM_SHEN)
        {
            auto f=std::bind([&](CPlayer* player){
                currentOperation=newOperation(OPERATION_SELECT);
                PLAYER->needSelect(currentOperation);
                currentOperation->send();
                if(currentOperation->finished)
                    emit newData();
                else
                    connect(currentOperation,currentOperation->signalFinished,this,this->doFuncs);
            },player);
            addFunc(f);
            auto f2=std::bind([&](CPlayer* player){
                auto list=currentOperation->reply[PLAYER->position].second;
                if(list.isEmpty()) return;
                PLAYER->setKingdom(list[0].toInt());
                emit newData();
            },player);
            addFunc(f2);
        }
    }
    addFunc(std::bind(s5gameStart,this));
    emit newData();
}

void CGameServer::handleGeneralChosen()
{
    CPlayerServer* cps=(CPlayerServer*)sender();
    if(players[0]==cps)
    {
        QByteArray sendbuf;
        sendbuf.resize(6);
        sendbuf[2]=GAME_GENERALCHOSEN;
        sendbuf[3]=cps->position;
        quint16 id=qToLittleEndian(cps->general->id);
        memcpy(sendbuf.data()+4,&id,2);
        foreach (auto player, players) {
            PLAYER->networkSend(sendbuf);
        }
        s2needChooseGeneral();
    }
    else
    {
        chosen++;
        if(chosen==server->numberOfPlayer-1)
        {
            QByteArray sendbuf;
            sendbuf.resize(6);
            sendbuf[2]=GAME_GENERALCHOSEN;
            quint16 id;
            foreach (auto player, players) {
                for(int i=1;i<server->numberOfPlayer;i++)
                {
                    sendbuf[3]=players[i]->position;
                    id=qToLittleEndian(players[i]->general->id);
                    memcpy(sendbuf.data()+4,&id,2);
                    PLAYER->networkSend(sendbuf);
                }
            }
            s3needAllGeneralAndCards();
        }
    }
}

void CGameServer::cardDeliver()
{

}

void CGameServer::drawCard(int n)
{
    int i;
    QList<CCard*> cards;
    if(deck.length()>=n)
    {
        for(i=0;i<n;i++)
            cards.append(deck.takeFirst());
    }
    else if(deck.length()+deadwood.length()<n)
    {
        //和局
    }
    else
    {
        cards=deck;
        deck=deadwood;
        deadwood.clear();
        std::random_shuffle(deck.begin(),deck.end());
        n-=cards.length();
        for(i=0;i<n;i++)
            cards.append(deck.takeFirst());
    }
    drawnCards=drawnCards.fromList(cards);
}

void CGameServer::needCard(int n, QSet<quint8> ntk)
{
    drawCard(n);
    QByteArray sendbuf;
    sendbuf.resize(3+n);
    sendbuf[2]=GAME_NEEDCARD;
    for(int i=0;i<n;i++)
    {
        sendbuf[3+i]=drawnCards[i]->id;
    }
    foreach (quint8 id, ntk) {
        auto player=players[id];
        PLAYER->networkSend(sendbuf);
    }
    emit newData();
}

void CGameServer::needWuXieKeJi()
{

}

void CGameServer::startNextRound()
{

}

COperation* CGameServer::newOperation(quint8 type)
{
    COperation* op;
    uint id;
    auto it=operations.end();
    if(it!=operations.begin())
    {
        it--;
        op=it.value();
        id=op->id+1;
    }
    else
        id=0;
    op=new COperation(this);
    op->id=id;
    op->type=type;
    operations.insert(operations.end(),id,op);
    return op;
}
