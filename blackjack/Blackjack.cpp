#include "Blackjack.h"

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <math.h>

#include <iomanip>
#include <sstream>

Blackjack::Blackjack() : Fl_Widget{0, 0, screenWidth, screenHeight} {
    gameInfo =
        make_unique<Fl_Text_Display>(40, (int)(cardHeight * 4.5), 250, 100);
    cashInfo = make_unique<Fl_Text_Display>(w() - 290, (int)(cardHeight * 4.5),
                                            250, 100);
    gameTextBuff = new Fl_Text_Buffer();
    cashTextBuff = new Fl_Text_Buffer();
    yesBtn = make_unique<Button>(screenWidth / 2 - 80, cardHeight * 5, 40, 20,
                                 "Yes");
    noBtn =
        make_unique<Button>(screenWidth / 2 + 40, cardHeight * 5, 40, 20, "No");
    actionBtns.push_back(make_unique<Button>(
        w() / 2 - 170, (int)(cardHeight * 4.5), 100, 20, "Hit"));
    actionBtns.push_back(make_unique<Button>(
        w() / 2 - 50, (int)(cardHeight * 4.5), 100, 20, "Stand"));
    actionBtns.push_back(make_unique<Button>(
        w() / 2 + 70, (int)(cardHeight * 4.5), 100, 20, "Double down"));
    betInput =
        make_unique<Fl_Int_Input>(w() / 2 - 30, cardHeight * 3, 60, 20, "Bet");
    betBtns.push_back(
        make_unique<Button>(w() / 2 - 100, cardHeight * 3 + 40, 40, 20, "50"));
    betBtns.push_back(
        make_unique<Button>(w() / 2 - 20, cardHeight * 3 + 40, 40, 20, "100"));
    betBtns.push_back(
        make_unique<Button>(w() / 2 + 60, cardHeight * 3 + 40, 40, 20, "200"));
    betBtns.push_back(
        make_unique<Button>(w() / 2 - 100, cardHeight * 3 + 80, 40, 20, "500"));
    betBtns.push_back(
        make_unique<Button>(w() / 2 - 20, cardHeight * 3 + 80, 40, 20, "1000"));
    betBtns.push_back(make_unique<Button>(w() / 2 + 60, cardHeight * 3 + 80, 40,
                                          20, "All in"));
    clock = make_unique<Fl_Round_Clock>(screenWidth - 100, 20, 80, 80);

    deck.shuffle();
    gameInfo->color(FL_GRAY, FL_GRAY);
    gameInfo->buffer(gameTextBuff);
    gameInfo->label("Blackjack game info");
    gameInfo->hide();
    cashInfo->color(FL_GRAY, FL_GRAY);
    cashInfo->buffer(cashTextBuff);
    cashInfo->label("Blackjack cash info");
    betInput->maximum_size(4);
    yesBtn->hide();
    noBtn->hide();
    updateCashInfo();
    hidePlayerBtns();
}

void Blackjack::init() {
    if (Fl::event_key(FL_Enter) && betInput->position()) {
        stringstream ss{betInput->value()};
        int number;
        ss >> number;
        if (number > 0 && number <= playerCash) {
            bet = number;
            playerCash -= bet;
            startRound();
        }
    } else {
        for (auto& b : betBtns) {
            if (b->click()) {
                string name = b->label();
                if (name == "All in") {
                    bet = playerCash;
                    playerCash -= bet;
                    startRound();
                } else {
                    int number = std::stoi(name);
                    if (number <= playerCash) {
                        bet = number;
                        playerCash -= bet;
                        startRound();
                    }
                }
            }
        }
    }
}

void Blackjack::startRound() {
    betInput->cut(0, betInput->maximum_size());
    hideBetBtns();
    updateCashInfo();
    rounds++;
    if (deck.size() < 20) newDeck();
    for (int i{0}; i < 2; i++) {
        player.newCard(deck.drawCard());
        dealer.newCard(deck.drawCard());
    }
    dealer.setCardSide(1, CardSide::back);
    showPlayerBtns();
    stage = Stage::player;
}

void Blackjack::playerTurn() {
    if (player.getBlackjack()) {
        stage = Stage::dealer;
    } else if (actionBtns[0]->click()) {
        player.newCard(deck.drawCard());
        if (player.getValue() == 21) {
            stage = Stage::dealer;
        } else if (player.getBusted()) {
            stage = Stage::showdown;
        }
    } else if (actionBtns[1]->click()) {
        stage = Stage::dealer;
    } else if (actionBtns[2]->click() && bet <= playerCash) {
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
        hidePlayerBtns();
        updateGameInfo();
        updateCashInfo();
        gameInfo->show();
        yesBtn->show();
        noBtn->show();
        dealer.setCardSide(1, CardSide::front);
    }
    fl_draw("New round?", w() / 2 - 30, cardHeight * 5 - 20);
    if (yesBtn->click()) {
        if (playerCash <= 0) {
#ifdef _WIN32
            endGame("You're out of cash! The game will now exit.");
#endif
            int choise = fl_choice("You're out of cash!", "Exit",
                                   "Buy more cash", "Restart");
            if (choise == 0) {
                endGame();
            } else if (choise == 1) {
                playerCash += playerStartCash;
                playerDebt += playerStartCash;
            } else if (choise == 2) {
                restartGame();
            }
        }
        dealer.reset();
        player.reset();
        gameInfo->hide();
        yesBtn->hide();
        noBtn->hide();
        showBetBtns();
        bet = 0;
        updateCashInfo();
        stage = Stage::init;
    } else if (noBtn->click()) {
        endGame();
    }
}

void Blackjack::updateGameInfo() {
    string info;
    if (player.getBusted()) {
        info = "Dealer won, player got busted!\n\n";
        dealerPoints += 1;
        dealerCash += bet;
    } else if (dealer.getBusted()) {
        info = "Player won, dealer got busted!\n\n";
        playerPoints += 1;
        playerCash += bet * 2;
        dealerCash -= bet;
    } else if (player.getBlackjack()) {
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
    string dealerPct = to_string((int)round(dealerPoints / rounds * 100));
    string playerPct = to_string((int)round(playerPoints / rounds * 100));
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
                  "Player's cash: " + to_string(playerCash) + "\n\n" +
                  "Player's debt: " + to_string(playerDebt);
    cashTextBuff->text(info.c_str());
}

void Blackjack::newDeck() {
    deck.reset();
    deck.shuffle();
}

void Blackjack::showPlayerBtns() {
    for (auto& b : actionBtns) {
        b->show();
    }
}

void Blackjack::hidePlayerBtns() {
    for (auto& b : actionBtns) {
        b->hide();
    }
}

void Blackjack::showBetBtns() {
    betInput->show();
    for (auto& b : betBtns) {
        b->show();
    }
}

void Blackjack::hideBetBtns() {
    betInput->hide();
    for (auto& b : betBtns) {
        b->hide();
    }
}

void Blackjack::endGame(string msg) {
    fl_message(msg.c_str());
    std::exit(0);
}

void Blackjack::restartGame() {
    newDeck();
    dealerPoints = 0;
    playerPoints = 0;
    rounds = 0;
    dealerCash = dealerStartCash;
    playerCash = playerStartCash;
    playerDebt = playerStartCash;
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