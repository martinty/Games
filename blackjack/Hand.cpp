#include "Hand.h"

void Hand::newCard(Card* card) {
    cards.push_back(card);

    int space = card->w() * 2;
    bool even = !(size() % 2);
    int offset;
    if (even) {
        offset = (size() / 2 - 1) * space + card->w();
    } else {
        offset = (size() / 2) * space;
    }
    int startX = screenWidth / 2 - card->w() / 2 - offset;
    for (int i{0}; i < size(); i++) {
        cards[i]->position(startX + space * i, y);
    }

    if (card->ace()) {
        if (value > 10) {
            value += 1;
        } else {
            value += 11;
            soft = true;
        }
    } else {
        value += (card->value() < 10 ? card->value() : 10);
        if (value > 21) {
            if (soft) {
                value -= 10;
                soft = false;
            } else {
                busted = true;
            }
        }
    }
    if (value == 21 && size() == 2) blackjack = true;
}

void Hand::reset() {
    for (auto& card : cards) {
        card->reset();
    }
    cards.clear();
    value = 0;
    busted = false;
    blackjack = false;
    soft = false;
}

void Hand::setCardSide(int i, CardSide side) {
    if (cards.empty()) {
        cout << "Hand is empty!\n";
    } else if (i < 0 || i >= (int)cards.size()) {
        cout << "Index is out of range!\n";
    } else {
        cards[i]->setCardSide(side);
    }
}

ostream& operator<<(ostream& os, const Hand& rhs) {
    for (const auto& card : rhs.cards) {
        os << *card << "  ";
    }
    return os;
}