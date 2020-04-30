#pragma once

#include "Dealer.h"

class Poker {
   private:
    Dealer dealer;

   public:
    Poker() = default;
    ~Poker() = default;
    void play();
};