#include <time.h>

#include "Poker.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

#ifdef __linux__
#else
    system("chcp 65001 > nul");  // For printing symbols
#endif

    Poker game;
    game.play();

    return 0;
}