#include "Dealer.h"

#include <iomanip>

Dealer::Dealer() {
    players.push_back(Player{table, players, "Martin"});
    players.push_back(Player{table, players, "Olav"});
    // players.push_back(Player{table, players, "Simen"});
    // players.push_back(Player{table, players, "Joar"});
}

void Dealer::burnCard() { deck.drawCard(); }

void Dealer::printTable() {
    cout << "Table: ";
    for (const Card& card : table) {
        cout << setw(6) << card;
    }
    cout << "\n\n";
}

void Dealer::init() {
    deck = CardDeck();
    deck.shuffle();
    table.clear();
    activePlayers.clear();
    pot = 0;
    for (Player& p : players) {
        p.reset();
        activePlayers.push_back(&p);
    }
}

void Dealer::playersAct(int startIndex) {
    int standingBet = 0;
    Act action = Act::check;

    if (startIndex == 3) {
        standingBet = bigBlind;
        action = Act::bet;
    }
    auto start = activePlayers.begin();

    while (startIndex > 0) {
        start++;
        startIndex--;
        if (start == activePlayers.end()) start = activePlayers.begin();
    }

    bool onlyAllin = true;
    int potAct = 0;
    for (const auto& p : activePlayers) {
        potAct += p->getTotalBet();
        if (p->getAction() != Act::allIn) onlyAllin = false;
    }

    if (onlyAllin) return;

    for (auto it = start;;) {
        if ((int)activePlayers.size() == 1) break;
        if ((*it)->getAction() == Act::allIn) continue;
        cout << "\nPot on table: " << pot + potAct << "\n";
        Move move = (*it)->makeMove(standingBet, action);

        potAct = 0;
        for (const auto& p : activePlayers) potAct += p->getTotalBet();

        if (move.action == Act::fold) {
            it = activePlayers.erase(it);
        } else {
            if (move.action != Act::call && move.action != Act::check) {
                action = move.action;
                if (standingBet <= move.bet) {
                    standingBet = move.bet;
                }
                for (auto it2 = activePlayers.begin();
                     it2 != activePlayers.end(); it2++) {
                    if (it2 != it && (*it2)->getAction() != Act::allIn) {
                        (*it2)->setStatus(Status::waiting);
                    }
                }
            } else {
                bool nextStage = true;
                for (const auto& p : activePlayers) {
                    if (p->getStatus() == Status::waiting) {
                        nextStage = false;
                        break;
                    }
                }
                if (nextStage) break;
            }
            it++;
        }

        onlyAllin = true;
        for (const auto& p : activePlayers) {
            if (p->getAction() != Act::allIn) onlyAllin = false;
        }
        if (onlyAllin) break;

        if (it == activePlayers.end()) it = activePlayers.begin();
        cout << "\n--------------------\n";
    }

    pot += potAct;
}

void Dealer::preFlop() {
    cout << "\n-------------------- Pre-flop --------------------\n";
    for (int i{0}; i < 2; i++) {
        for (Player& p : players) {
            p.newCard(deck.drawCard());
        }
    }

    for (auto& p : activePlayers) {
        p->calculateHandRank();
    }

    auto it = activePlayers.begin() + 1;
    (*it)->blind(smallBlind);
    if (++it == activePlayers.end()) it = activePlayers.begin();
    (*it)->blind(bigBlind);

    playersAct(3);
}

void Dealer::flop() {
    cout << "\n-------------------- Flop ------------------------\n";
    burnCard();
    for (int i{0}; i < 3; i++) {
        table.push_back(deck.drawCard());
    }

    for (auto& p : activePlayers) {
        p->calculateHandRank();
        p->resetTotalBet();
        p->setStatus(Status::waiting);
    }

    playersAct(1);
}

void Dealer::turn() {
    cout << "\n-------------------- Turn ------------------------\n";
    burnCard();
    table.push_back(deck.drawCard());

    for (auto& p : activePlayers) {
        p->calculateHandRank();
        p->resetTotalBet();
        p->setStatus(Status::waiting);
    }

    playersAct(1);
}

void Dealer::river() {
    cout << "\n-------------------- River -----------------------\n";
    burnCard();
    table.push_back(deck.drawCard());

    for (auto& p : activePlayers) {
        p->calculateHandRank();
        p->resetTotalBet();
        p->setStatus(Status::waiting);
    }

    playersAct(1);
}

void Dealer::showdown() {
    cout << "\n-------------------- Showdown --------------------\n";

    printTable();
    for (auto& p : activePlayers) {
        p->calculateHandRank();
    }

    vector<Player*> winners{activePlayers.front()};
    for (auto it = activePlayers.begin() + 1; it != activePlayers.end(); it++) {
        if (*winners.back() == **it) {
            winners.push_back(*it);
        } else if (*winners.back() < **it) {
            winners.clear();
            winners.push_back(*it);
        }
    }
    pot = pot / (int)winners.size();
    cout << "Pot for each winner: " << pot << "\n";
    cout << "The winner(s): ";
    for (const auto& w : winners) {
        cout << w->getName() << ", ";
        w->givePot(pot);
    }
    cout << "\n\n";

    for (auto& p : activePlayers) {
        cout << *p << "\n\n";
    }

    Player last = players.back();
    players.pop_back();
    players.push_front(last);

    cout << "--------------------------------------------------\n\n";
}