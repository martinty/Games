#pragma once

#include <deque>
#include <list>
#include <set>
#include <vector>

#include "Card.h"

enum class Act { fold = 1, check, call, bet, raise, reReise, allIn };
enum class Status { ready, waiting };
enum class HandRank {
    highCard,
    onePair,
    twoPair,
    trips,
    straight,
    flush,
    fullHouse,
    quads,
    straightFlush,
    royalFlush
};

const map<HandRank, string> handRankStringMap{
    {HandRank::highCard, "High card"},
    {HandRank::onePair, "One pair"},
    {HandRank::twoPair, "Two pair"},
    {HandRank::trips, "Three of a kind"},
    {HandRank::straight, "Straihgt"},
    {HandRank::flush, "Flush"},
    {HandRank::fullHouse, "Full house"},
    {HandRank::quads, "Four of a kind"},
    {HandRank::straightFlush, "Straight flush"},
    {HandRank::royalFlush, "Royal flush"}};

const map<Act, string> actStringMap{
    {Act::fold, "Fold"},   {Act::check, "Check"}, {Act::call, "Call"},
    {Act::bet, "Bet"},     {Act::raise, "Raise"}, {Act::reReise, "Re-reise"},
    {Act::allIn, "All in"}};

struct Move {
    int bet;
    Act action;
};

ostream& operator<<(ostream& os, const HandRank& rhs);
ostream& operator<<(ostream& os, const Act& rhs);

class Player {
   private:
    vector<Card> hand;
    const vector<Card>& table;
    const deque<Player>& players;
    int cash = 3000;
    HandRank handRank;
    int handValue = 0;
    int totalBet = 0;
    string name;
    Act action = Act::check;
    Status status = Status::waiting;
    void printTable();

   public:
    Player(const vector<Card>& table, const deque<Player>& players, string name)
        : table{table}, players{players}, name{name} {}
    Player() = delete;
    ~Player() = default;

    void newCard(Card card);
    void blind(int value);
    void calculateHandRank();
    Move makeMove(int standingBet, Act standingAct);
    void reset();
    void resetTotalBet() { totalBet = 0; }

    int getCash() const { return cash; }
    HandRank getHandRank() const { return handRank; }
    Status getStatus() const { return status; }
    void setStatus(Status s) { status = s; }
    Act getAction() const { return action; }
    int getHandValue() const { return handValue; }
    string getName() const { return name; }
    void givePot(int pot) { cash += pot; }
    int getTotalBet() const { return totalBet; }

    friend ostream& operator<<(ostream& os, const Player& rhs);
    bool operator<(const Player& rhs) const;
    bool operator==(const Player& rhs) const;
};

string getStringInput();
int getIntInput(int min, int max);