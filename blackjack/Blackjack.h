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
    void showPlayerBtn();
    void hidePlayerBtn();
    void playGame();
    void draw() override;

    unique_ptr<Fl_Text_Display> gameInfo =
        make_unique<Fl_Text_Display>(40, (int)(cardHeight * 4.5), 250, 100);
    unique_ptr<Fl_Text_Display> cashInfo = make_unique<Fl_Text_Display>(
        w() - 290, (int)(cardHeight * 4.5), 250, 100);
    Fl_Text_Buffer *gameTextBuff = new Fl_Text_Buffer();
    Fl_Text_Buffer *cashTextBuff = new Fl_Text_Buffer();
    unique_ptr<Button> yesBtn = make_unique<Button>(
        screenWidth / 2 - 80, cardHeight * 5, 40, 20, "Yes");
    unique_ptr<Button> noBtn =
        make_unique<Button>(screenWidth / 2 + 40, cardHeight * 5, 40, 20, "No");
    unique_ptr<Button> hitBtn =
        make_unique<Button>(w() / 2 - 170, cardHeight * 5, 100, 20, "Hit");
    unique_ptr<Button> standBtn =
        make_unique<Button>(w() / 2 - 50, cardHeight * 5, 100, 20, "Stand");
    unique_ptr<Button> doubleDownBtn = make_unique<Button>(
        w() / 2 + 70, cardHeight * 5, 100, 20, "Double down");
    unique_ptr<Fl_Int_Input> cashInput = make_unique<Fl_Int_Input>(
        w() / 2 - 30, cardHeight * 5 - 20, 60, 20, "Bet");
    unique_ptr<Fl_Round_Clock> clock =
        make_unique<Fl_Round_Clock>(screenWidth - 100, 20, 80, 80);

   public:
    Blackjack();
    ~Blackjack() = default;
};