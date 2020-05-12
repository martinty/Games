#pragma once

#include <FL/Fl_Widget.H>

#include <iostream>
#include <map>
#include <string>

#include "FL/Fl_PNG_Image.H"
#include "constants.h"

using namespace std;

enum class Suit { clubs = 1, diamonds, hearts, spades };
enum class Rank {
    two = 2,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace
};

const map<Suit, string> suitSymbolMap{{Suit::clubs, "C"},
                                      {Suit::diamonds, "D"},
                                      {Suit::hearts, "H"},
                                      {Suit::spades, "S"}};

const map<Rank, string> rankSymbolMap{
    {Rank::two, "2"},   {Rank::three, "3"}, {Rank::four, "4"},
    {Rank::five, "5"},  {Rank::six, "6"},   {Rank::seven, "7"},
    {Rank::eight, "8"}, {Rank::nine, "9"},  {Rank::ten, "10"},
    {Rank::jack, "J"},  {Rank::queen, "Q"}, {Rank::king, "K"},
    {Rank::ace, "A"}};

enum class CardSide { front, back, none };

class Card : public Fl_Widget {
   private:
    Suit s;
    Rank r;
    Fl_PNG_Image* card;
    Fl_PNG_Image* backSide;
    CardSide cardSide;
    void draw() override;

   public:
    Card(Suit s, Rank r, int x = 0, int y = 0, CardSide side = CardSide::none);
    Card() = delete;
    ~Card();
    void reset();
    void setCardSide(CardSide side);
    CardSide getCardSide() const;
    Suit getSuit() const;
    Rank getRank() const;
    int value() const;
    bool ace() const;
    string toString() const;
    friend ostream& operator<<(ostream& os, const Card& rhs);
    bool operator<(const Card& rhs) const;
};
