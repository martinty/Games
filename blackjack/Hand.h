#pragma once

#include <vector>

#include "Card.h"

class Hand {
   private:
    vector<Card*> cards;
    int value = 0;
    bool busted = false;
    bool blackjack = false;
    bool soft = false;
    const int y;

   public:
    Hand(int y = 0) : y{y} {};
    ~Hand() = default;
    int getValue() const { return value; }
    bool getBusted() const { return busted; }
    bool getBlackjack() const { return blackjack; }
    bool getSoft() const { return soft; }
    int getSize() const { return (int)cards.size(); }
    void newCard(Card* card);
    void reset();
    void setCardSide(int i, CardSide side);

    friend ostream& operator<<(ostream& os, const Hand& rhs);
};