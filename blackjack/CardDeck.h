#pragma once
#include <vector>

#include "Card.h"

class CardDeck {
   private:
    vector<Card*> cards;
    void swap(int a, int b);
    int index = 0;
    static constexpr int deckSize = 52;

   public:
    CardDeck();
    ~CardDeck();
    void shuffle(int N = 3);
    void reset();
    Card* drawCard(CardSide side = CardSide::front);
    bool empty() const;
    int size() const;
    friend ostream& operator<<(ostream& os, const CardDeck& rhs);
};