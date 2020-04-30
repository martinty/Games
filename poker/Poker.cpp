#include "Poker.h"

void Poker::play() {
    string input;
    for (;;) {
        cout << "New round? [yes / no]: ";
        input = getStringInput();
        if (input == "n" || input == "no") break;

        dealer.init();
        dealer.preFlop();
        dealer.flop();
        dealer.turn();
        dealer.river();
        dealer.showdown();
    }
}