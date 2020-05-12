#include "Card.h"

Card::Card(Suit s, Rank r, int x, int y, CardSide side)
    : Fl_Widget{x, y, cardWidth, cardHeight},
      s{s},
      r{r},
      backSide{new Fl_PNG_Image{"cards_png/blue_back.png"}},
      cardSide{side} {
    string fileName{"cards_png/" + toString() + ".png"};
    card = new Fl_PNG_Image{fileName.c_str()};
    card->scale(cardWidth, cardHeight);
    backSide->scale(cardWidth, cardHeight);
    hide();
}

Card::~Card() {
    delete card;
    delete backSide;
}

void Card::reset() {
    cardSide = CardSide::none;
    hide();
}

void Card::draw() {
    switch (cardSide) {
        case CardSide::front:
            card->draw(x(), y());
            break;
        case CardSide::back:
            backSide->draw(x(), y());
            break;
        default:
            break;
    }
}

void Card::setCardSide(CardSide side) { cardSide = side; }

CardSide Card::getCardSide() const { return cardSide; }

Suit Card::getSuit() const { return s; }

Rank Card::getRank() const { return r; }

int Card::value() const { return (int)r; }

bool Card::ace() const { return (r == Rank::ace); }

string Card::toString() const {
    return rankSymbolMap.at(r) + suitSymbolMap.at(s);
}

ostream& operator<<(ostream& os, const Card& rhs) {
    return os << rhs.toString();
}

bool Card::operator<(const Card& rhs) const {
    if (r == rhs.r) {
        return s < rhs.s;
    } else {
        return r < rhs.r;
    }
}

bool Card::operator==(const Card& rhs) const {
    return (r == rhs.r && s == rhs.s);
}