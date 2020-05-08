#pragma once

#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Round_Clock.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Widget.H>

#include <memory>

#include "Button.h"
#include "CardDeck.h"
#include "Hand.h"

enum class Stage { init, player, dealer, showdown };

class Blackjack : public Fl_Widget {
   private:
    CardDeck deck;
    Hand dealer{cardHeight * 1};
    Hand player{cardHeight * 3};
    double dealerPoints = 0;
    double playerPoints = 0;
    int rounds = 0;
    int dealerCash = 10000;
    int playerCash = 3000;
    int bet = 0;
    Stage stage = Stage::init;

    void init();
    void startRound();
    void playerTurn();
    void dealerTurn();
    void showdown();
    void updateGameInfo();
    void updateCashInfo();
    void newDeck();
    void showPlayerBtns();
    void hidePlayerBtns();
    void showBetBtns();
    void hideBetBtns();
    void playGame();
    void draw() override;

    unique_ptr<Fl_Text_Display> gameInfo;
    unique_ptr<Fl_Text_Display> cashInfo;
    Fl_Text_Buffer *gameTextBuff = new Fl_Text_Buffer();
    Fl_Text_Buffer *cashTextBuff = new Fl_Text_Buffer();
    unique_ptr<Button> yesBtn;
    unique_ptr<Button> noBtn;
    vector<unique_ptr<Button>> actionBtns;
    unique_ptr<Fl_Int_Input> betInput;
    vector<unique_ptr<Button>> betBtns;
    unique_ptr<Fl_Round_Clock> clock;

   public:
    Blackjack();
    ~Blackjack() = default;
};