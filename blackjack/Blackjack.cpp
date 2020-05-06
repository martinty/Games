#include "Blackjack.h"

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>

#include <iomanip>
#include <sstream>

Blackjack::Blackjack() : Fl_Widget{0, 0, screenWidth, screenHeight} {
    deck.shuffle();
    gameInfo->color(FL_GRAY, FL_GRAY);
    gameInfo->buffer(gameTextBuff);
    gameInfo->label("Blackjack game info");
    gameInfo->hide();
    cashInfo->color(FL_GRAY, FL_GRAY);
    cashInfo->buffer(cashTextBuff);
    cashInfo->label("Blackjack cash info");
    updateCashInfo();
    cashInput->maximum_size(4);
    hidePlayerBtn();
    yesBtn->hide();
    noBtn->hide();
}

void Blackjack::init() {
    if (Fl::event_key(FL_Enter) && cashInput->position()) {
        stringstream ss{cashInput->value()};
        int number;
        ss >> number;
        cashInput->cut(0, cashInput->maximum_size());
        if (number > 0 && number <= playerCash) {
            bet = number;
            playerCash -= bet;
            cashInput->hide();
            updateCashInfo();
            startRound();
        }
    }
}

void Blackjack::startRound() {
    rounds++;
    if (deck.size() < 20) newDeck();
    for (int i{0}; i < 2; i++) {
        player.newCard(deck.drawCard());
        dealer.newCard(deck.drawCard());
    }
    dealer.setCardState(1, CardState::back);
    showPlayerBtn();
    stage = Stage::player;
}

void Blackjack::playerTurn() {
    if (player.getBlackjack()) {
        stage = Stage::dealer;
    } else if (hitBtn->click()) {
        player.newCard(deck.drawCard());
        if (player.getValue() == 21) {
            stage = Stage::dealer;
        } else if (player.getBusted()) {
            stage = Stage::showdown;
        }
    } else if (standBtn->click()) {
        stage = Stage::dealer;
    } else if (doubleDownBtn->click() && bet <= playerCash) {
        playerCash -= bet;
        bet += bet;
        player.newCard(deck.drawCard());
        if (player.getBusted()) {
            stage = Stage::showdown;
        } else {
            stage = Stage::dealer;
        }
    }
}

void Blackjack::dealerTurn() {
    while (dealer.getValue() < 17 ||
           (dealer.getSoft() && dealer.getValue() == 17)) {
        dealer.newCard(deck.drawCard());
    }
    stage = Stage::showdown;
}

void Blackjack::showdown() {
    if (!gameInfo->visible()) {
        hidePlayerBtn();
        updateGameInfo();
        updateCashInfo();
        gameInfo->show();
        yesBtn->show();
        noBtn->show();
        dealer.setCardState(1, CardState::front);
    }
    fl_draw("New round?", w() / 2 - 30, cardHeight * 5 - 20);
    if (yesBtn->click()) {
        dealer.reset();
        player.reset();
        gameInfo->hide();
        yesBtn->hide();
        noBtn->hide();
        cashInput->show();
        bet = 0;
        updateCashInfo();
        if (playerCash <= 0) {
            fl_message("You're out of cash... The game will now exit.");
            exit(0);
        }
        stage = Stage::init;
    } else if (noBtn->click()) {
        fl_message("Thanks for playing Blackjack! The game will now exit.");
        exit(0);
    }
}

void Blackjack::updateGameInfo() {
    string info;
    if (player.getBlackjack()) {
        if (dealer.getBlackjack()) {
            info = "Both got Blackjack, it's a push!\n\n";
            playerPoints += 0.5;
            dealerPoints += 0.5;
            playerCash += bet;
        } else {
            info = "Player won with Blackjack!\n\n";
            playerPoints += 1;
            playerCash += bet * 2;
            dealerCash -= bet;
        }
    } else if (dealer.getBlackjack()) {
        info = "Dealer won with Blackjack!\n\n";
        dealerPoints += 1;
        dealerCash += bet;
    } else if (player.getBusted()) {
        info = "Dealer won, player got busted!\n\n";
        dealerPoints += 1;
        dealerCash += bet;
    } else if (dealer.getBusted()) {
        info = "Player won, dealer got busted!\n\n";
        playerPoints += 1;
        playerCash += bet * 2;
        dealerCash -= bet;
    } else if (player.getValue() < dealer.getValue()) {
        info =
            "Dealer won with value " + to_string(dealer.getValue()) + "!\n\n";
        dealerPoints += 1;
        dealerCash += bet;
    } else if (player.getValue() > dealer.getValue()) {
        info =
            "Player won with value " + to_string(player.getValue()) + "!\n\n";
        playerPoints += 1;
        playerCash += bet * 2;
        dealerCash -= bet;
    } else {
        info = "Equal value, it's a push!\n\n";
        playerPoints += 0.5;
        dealerPoints += 0.5;
        playerCash += bet;
    }
    string dealerPct = to_string((int)std::round(dealerPoints / rounds * 100));
    string playerPct = to_string((int)std::round(playerPoints / rounds * 100));
    ostringstream ssPlayer, ssDealer;
    ssDealer << fixed << setprecision(1) << dealerPoints;
    ssPlayer << fixed << setprecision(1) << playerPoints;
    info += "Rounds: " + to_string(rounds) + "\n" +
            "Dealer's points: " + ssDealer.str() + "\t" + dealerPct + "%\n" +
            "Player's points: " + ssPlayer.str() + "\t" + playerPct + "%";
    gameTextBuff->text(info.c_str());
}

void Blackjack::updateCashInfo() {
    string info = "Current bet: " + to_string(bet) + "\n\n" +
                  "Dealer's cash: " + to_string(dealerCash) + "\n" +
                  "Player's cash: " + to_string(playerCash);
    cashTextBuff->text(info.c_str());
}

void Blackjack::newDeck() {
    deck.reset();
    deck.shuffle();
}

void Blackjack::showPlayerBtn() {
    hitBtn->show();
    standBtn->show();
    doubleDownBtn->show();
}

void Blackjack::hidePlayerBtn() {
    hitBtn->hide();
    standBtn->hide();
    doubleDownBtn->hide();
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