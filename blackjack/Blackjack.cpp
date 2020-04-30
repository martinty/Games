#include "Blackjack.h"

#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include <iomanip>
#include <sstream>

Blackjack::Blackjack() : Fl_Widget{0, 0, screenWidth, screenHeight} {
    deck.shuffle();
    gameInfo->color(FL_GRAY, FL_GRAY);
    gameInfo->buffer(textBuff);
    gameInfo->label("Blackjack game info");
    gameInfo->hide();
}

void Blackjack::init() {
    fl_draw("New round?", w() / 2 - 30, cardHeight * 5 - 20);
    if (yesBtn->click()) {
        newRound();
    } else if (noBtn->click()) {
        exit(0);
    }
}

void Blackjack::newRound() {
    rounds++;
    if (deck.size() < 20) newDeck();
    for (int i{0}; i < 2; i++) {
        player.newCard(deck.drawCard());
        dealer.newCard(deck.drawCard());
    }
    dealer.setCardState(1, CardState::back);
    stage = Stage::player;
}

void Blackjack::playerTurn() {
    if (player.getBlackjack()) {
        stage = Stage::dealer;
        return;
    }
    fl_draw("New card?", w() / 2 - 30, cardHeight * 5 - 20);
    if (yesBtn->click()) {
        player.newCard(deck.drawCard());
        if (player.getValue() == 21) {
            stage = Stage::dealer;
        } else if (player.getBusted()) {
            showGameInfo();
            stage = Stage::showdown;
        }
    } else if (noBtn->click()) {
        stage = Stage::dealer;
    }
}

void Blackjack::dealerTurn() {
    while (dealer.getValue() < 17 ||
           (dealer.getSoft() && dealer.getValue() == 17)) {
        dealer.newCard(deck.drawCard());
    }
    showGameInfo();
    stage = Stage::showdown;
}

void Blackjack::showdown() {
    dealer.setCardState(1, CardState::front);
    fl_draw("Continue?", w() / 2 - 30, cardHeight * 5 - 20);
    if (yesBtn->click()) {
        dealer.reset();
        player.reset();
        gameInfo->hide();
        stage = Stage::init;
    } else if (noBtn->click()) {
        exit(0);
    }
}

void Blackjack::showGameInfo() {
    string info;
    if (player.getBlackjack()) {
        if (dealer.getBlackjack()) {
            info = "Both got Blackjack, it's a push!\n\n";
            playerPoints += 0.5;
            dealerPoints += 0.5;
        } else {
            info = "Player won with Blackjack!\n\n";
            playerPoints += 1;
        }
    } else if (dealer.getBlackjack()) {
        info = "Dealer won with Blackjack!\n\n";
        dealerPoints += 1;
    } else if (player.getBusted()) {
        info = "Dealer won, player got busted!\n\n";
        dealerPoints += 1;
    } else if (dealer.getBusted()) {
        info = "Player won, dealer got busted!\n\n";
        playerPoints += 1;
    } else if (player.getValue() < dealer.getValue()) {
        info =
            "Dealer won with value " + to_string(dealer.getValue()) + "!\n\n";
        dealerPoints += 1;
    } else if (player.getValue() > dealer.getValue()) {
        info =
            "Player won with value " + to_string(player.getValue()) + "!\n\n";
        playerPoints += 1;
    } else {
        info = "Equal value, it's a push!\n\n";
        playerPoints += 0.5;
        dealerPoints += 0.5;
    }

    string dealerPct = to_string((int)std::round(dealerPoints / rounds * 100));
    string playerPct = to_string((int)std::round(playerPoints / rounds * 100));
    ostringstream ssPlayer, ssDealer;
    ssDealer << fixed << setprecision(1) << dealerPoints;
    ssPlayer << fixed << setprecision(1) << playerPoints;
    info += "Rounds: " + to_string(rounds) + "\n";
    info += "Dealer's points: " + ssDealer.str() + "\t" + dealerPct + "%\n";
    info += "Player's points: " + ssPlayer.str() + "\t" + playerPct + "%\n";
    textBuff->text(info.c_str());

    gameInfo->show();
}

void Blackjack::newDeck() {
    deck.reset();
    deck.shuffle();
}

void Blackjack::playGame() {
    switch (stage) {
        case Stage::init: {
            init();
            break;
        }
        case Stage::player: {
            playerTurn();
            break;
        }
        case Stage::dealer: {
            dealerTurn();
            break;
        }
        case Stage::showdown: {
            showdown();
            break;
        }
        default: {
            break;
        }
    }
}

void Blackjack::draw() {
    string dealerStr = "Dealer: ";
    if (stage != Stage::player) dealerStr += to_string(dealer.getValue());
    string playerStr = "Player: " + to_string(player.getValue());
    fl_color(FL_BLACK);
    fl_draw(dealerStr.c_str(), w() / 2 - 20, cardHeight * 1 - 20);
    fl_draw(playerStr.c_str(), w() / 2 - 20, cardHeight * 3 - 20);

    playGame();
}