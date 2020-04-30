#include "Card.h"

Card::Card(Suit s, Rank r, int x, int y)
    : Fl_Widget{x, y, cardWidth, cardHeight}, s{s}, r{r} {
    string fileName = "cards_png/" + toString() + ".png";
    card = new Fl_PNG_Image{fileName.c_str()};
    card->scale(cardWidth, cardHeight);
    backSide.scale(cardWidth, cardHeight);
}

Card::~Card() { delete card; }

void Card::reset() {
    drawable = false;
    state = CardState::back;
    hide();
}

void Card::draw() {
    if (drawable) {
        switch (state) {
            case CardState::front:
                card->draw(x(), y());
                break;
            case CardState::back:
                backSide.draw(x(), y());
                break;
            default:
                break;
        }
    }
}

void Card::setDrawable(bool value) { drawable = value; }

void Card::setState(CardState state) { this->state = state; }

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