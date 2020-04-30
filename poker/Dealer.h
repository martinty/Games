#pragma once

#include "CardDeck.h"
#include "Player.h"

class Dealer {
   private:
    CardDeck deck;
    vector<Card> table;
    deque<Player> players;
    vector<Player*> activePlayers;
    void burnCard();
    void printTable();
    void playersAct(int startIndex);
    int pot = 0;
    const int bigBlind = 10;
    const int smallBlind = bigBlind / 2;

   public:
    Dealer();
    ~Dealer() = default;
    void init();
    void preFlop();
    void flop();
    void turn();
    void river();
    void showdown();
};