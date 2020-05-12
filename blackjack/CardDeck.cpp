#include "CardDeck.h"

CardDeck::CardDeck() {
    for (int suit = 1; suit <= 4; suit++) {
        for (int rank = 2; rank <= 14; rank++) {
            cards.push_back(new Card{Suit{suit}, Rank{rank}});
        }
    }
}

CardDeck::~CardDeck() {
    for (auto& card : cards) {
        delete card;
    }
}

void CardDeck::swap(int a, int b) {
    Card* temp = move(cards[a]);
    cards[a] = move(cards[b]);
    cards[b] = move(temp);
}

void CardDeck::shuffle() {
    for (int n = 0; n < 3; n++) {
        for (int i = 0; i < size(); i++) {
            swap(i, rand() % size());
        }
    }
}

void CardDeck::reset() {
    index = 0;
    for (auto& card : cards) {
        card->reset();
    }
}

Card* CardDeck::drawCard(CardSide side) {
    Card* card = cards[index++];
    card->setCardSide(side);
    card->show();
    return card;
}

bool CardDeck::empty() const { return size() < 1; }

int CardDeck::size() const { return deckSize - index; }

ostream& operator<<(ostream& os, const CardDeck& rhs) {
    for (const auto& card : rhs.cards) {
        os << *card << endl;
    }
    return os;
}