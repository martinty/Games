#include "Card.h"

Suit Card::getSuit() const { return s; }

Rank Card::getRank() const { return r; }

int Card::value() const { return (int)r; }

bool Card::ace() const { return (r == Rank::ace); }

ostream& operator<<(ostream& os, const Card& rhs) {
    return os << TextColor(suitColorMap.at(rhs.s))
              << rankSymbolMap.at(rhs.r) + suitSymbolMap.at(rhs.s)
              << TextColor();
}

bool Card::operator<(const Card& rhs) const {
    if (getRank() == rhs.getRank()) {
        return getSuit() < rhs.getSuit();
    } else {
        return getRank() < rhs.getRank();
    }
}