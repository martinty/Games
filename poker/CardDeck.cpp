#include "CardDeck.h"

CardDeck::CardDeck() {
    for (int suit = 1; suit <= 4; suit++) {
        for (int rank = 2; rank <= 14; rank++) {
            cards.push_back(Card{Suit{suit}, Rank{rank}});
        }
    }
}

void CardDeck::swap(int a, int b) {
    Card temp = cards[a];
    cards[a] = cards[b];
    cards[b] = temp;
}

void CardDeck::shuffle() {
    for (int n = 0; n < 3; n++) {
        for (int i = 0; i < size(); i++) {
            swap(i, rand() % size());
        }
    }
}

Card CardDeck::drawCard() {
    Card card = cards.back();
    cards.pop_back();
    return card;
}

bool CardDeck::empty() const { return cards.empty(); }

int CardDeck::size() const { return (int)cards.size(); }

ostream& operator<<(ostream& os, const CardDeck& rhs) {
    for (const Card& card : rhs.cards) {
        os << card << endl;
    }
    os << TextColor();
    return os;
}