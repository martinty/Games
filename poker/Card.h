#pragma once

#include <iostream>
#include <map>
#include <string>

#include "TextColor.h"

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

const map<Suit, string> suitSymbolMap{{Suit::clubs, "♣"},
                                      {Suit::diamonds, "♦"},
                                      {Suit::hearts, "♥"},
                                      {Suit::spades, "♠"}};

const map<Suit, ColorId> suitColorMap{{Suit::clubs, ColorId::green},
                                      {Suit::diamonds, ColorId::blue},
                                      {Suit::hearts, ColorId::red},
                                      {Suit::spades, ColorId::magenta}};

const map<Rank, string> rankSymbolMap{
    {Rank::two, "2"},   {Rank::three, "3"}, {Rank::four, "4"},
    {Rank::five, "5"},  {Rank::six, "6"},   {Rank::seven, "7"},
    {Rank::eight, "8"}, {Rank::nine, "9"},  {Rank::ten, "10"},
    {Rank::jack, "J"},  {Rank::queen, "Q"}, {Rank::king, "K"},
    {Rank::ace, "A"}};

class Card {
   private:
    Suit s;
    Rank r;

   public:
    Card(Suit s, Rank r) : s{s}, r{r} {}
    Card() = delete;
    ~Card() = default;
    Suit getSuit() const;
    Rank getRank() const;
    int value() const;
    bool ace() const;
    friend ostream& operator<<(ostream& os, const Card& rhs);
    bool operator<(const Card& rhs) const;
};
