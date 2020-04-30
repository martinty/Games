#pragma once
#include <vector>

#include "Card.h"

class CardDeck {
   private:
    vector<Card> cards;
    void swap(int a, int b);

   public:
    CardDeck();
    ~CardDeck() = default;
    void shuffle();
    Card drawCard();
    bool empty() const;
    int size() const;

    friend ostream& operator<<(ostream& os, const CardDeck& rhs);
};