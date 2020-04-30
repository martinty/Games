#pragma once

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
    Stage stage = Stage::init;
    void init();
    void newRound();
    void playerTurn();
    void dealerTurn();
    void showdown();
    void showGameInfo();
    void newDeck();
    void playGame();
    void draw() override;

    unique_ptr<Button> yesBtn = make_unique<Button>(
        screenWidth / 2 - 80, cardHeight * 5, 40, 20, "Yes");
    unique_ptr<Button> noBtn =
        make_unique<Button>(screenWidth / 2 + 40, cardHeight * 5, 40, 20, "No");
    unique_ptr<Fl_Text_Display> gameInfo =
        make_unique<Fl_Text_Display>(40, (int)(cardHeight * 4.5), 250, 110);
    Fl_Text_Buffer *textBuff = new Fl_Text_Buffer();
    unique_ptr<Fl_Round_Clock> clock =
        make_unique<Fl_Round_Clock>(screenWidth - 100, 20, 80, 80);

   public:
    Blackjack();
    ~Blackjack() = default;
};