#include "cgameserver.h"
#include "network/cserver.h"
#include "network/croom.h"
#include "cplayerserver.h"

CGameServer::CGameServer(CServer *server, CRoom *room)
{
    this->server=server;
    this->room=room;    
}

CGameServer::~CGameServer()
{

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
    std::random_shuffle(players.begin(),players.end());
    QList<quint8> roles;
    roles=server->roles;
    std::random_shuffle(roles.begin(),roles.end());
    roles.prepend(ROLE_ZHU);
    int j=server->numberOfPlayer;
    int i;
    for(i=0;i<j;i++)
    {
        players[i]->position=i+1;
        players[i]->role=roles[i];
        players[i]->sendGameStart();
    }
    QList<CGeneral*> pickZG;
    pickZG=server->availableZhugong;
    if(server->choicesZG)
    {
        int l=server->availableGenerals.length();
        int pn=server->choicesZG;
        if(l<=pn)
        {
            pickZG.append(server->availableGenerals);
        }
        else
        {
            std::set<quint8> picks;
            qsrand(QTime::currentTime().msecsSinceStartOfDay());
            for(;pn>0;pn--)
            {
                j=qrand()%l;
                foreach (quint8 pk, picks) {
                    if(j>=pk)
                        j++;
                    else
                        break;
                }
                picks.insert(j);
                pickZG.append(server->availableGenerals[j]);
                l--;
            }
        }
    }
    players[0]->chooseGeneral(pickZG);
    for(i=1;i<server->numberOfPlayer;i++)
    {
        players[i]->sendGameStart();
    }
}

void CGameServer::s2needChooseGeneral()
{
    QList<CGeneral*> allGenerals,pickedGenerals,generalsForEachPlayer[server->numberOfPlayer-1];
    allGenerals=server->availableZhugong;
    allGenerals.append(server->availableGenerals);
    allGenerals.removeOne(players[0]->general);
    int n=server->choices*(server->numberOfPlayer-1)+server->choicesNJ*server->numberOfNei;
    int i,j;
    if(allGenerals.length()-1<=n)
    {
        i=0;j=0;
        bool b=false;
        QList<quint8> nei;
        foreach (CGeneral* general, allGenerals) {
            if(b)
            {
                generalsForEachPlayer[nei[i]].append(general);
                i++;
                if(i>=nei.length()) i=0;
            }
            else
            {
                generalsForEachPlayer[i].append(general);
                i++;
                if(i>=server->numberOfPlayer-1)
                {
                    i=0;
                    j++;
                    if(j==server->choices&&allGenerals.length()>j*server->numberOfPlayer)
                    {
                        for(j=0;j<server->numberOfPlayer;j++)
                        {
                            if(players[j]->role=ROLE_NEI)
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
        std::set<quint8> picks;
        qsrand(QTime::currentTime().msecsSinceStartOfDay());
        int l=allGenerals.length();
        for(;n>0;n--)
        {
            j=qrand()%l;
            foreach (quint8 pk, picks) {
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
        }
    }
    for(i=1;i<server->numberOfPlayer;i++)
    {
        players[i]->chooseGeneral(generalsForEachPlayer[i-1]);
    }
}

void CGameServer::s3needAllGeneralAndCards()
{
    QList<CCard*> cards;
    foreach (CPlayerServer* player, players) {
        cards=drawCard(4);
        player->deliverCard(cards);
    }
    this->s4skillSetup();
    this->s5gameStart();
}

void CGameServer::handleGeneralChosen()
{
    CPlayerServer* cps=(CPlayerServer*)sender();
    static int chosen=0;
    if(players[0]==cps)
    {
        s2needChooseGeneral();
    }
    else
    {
        chosen++;
        if(chosen==server->numberOfPlayer-1)
        {
            s3needAllGeneralAndCards();
        }
    }
}

void CGameServer::cardDeliver()
{

}

QList<CCard*> CGameServer::drawCard(int n)
{
    int i;
    QList<CCard*> cards;
    if(deck.length()<0)
    {
        for(i=0;i<n;i++)
            cards.append(deck.takeFirst());
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
    return cards;
}
