#pragma once

#include <FL/Fl_Widget.H>

#include <memory>
#include <vector>

#include "FL/Fl_PNG_Image.H"
#include "Snake.h"
#include "utilities.h"

class Track : public Fl_Widget {
   private:
    void draw() override;
    void update();
    void newFood();
    void restart();

    std::unique_ptr<Snake> snake = std::make_unique<Snake>();
    Pos food;
    bool gameOver = false;
    int bgIndex = 0;

    Fl_PNG_Image game_over{"sprites/gameover.png"};
    Fl_PNG_Image restart_game{"sprites/restart.png"};
    std::vector<Fl_PNG_Image*> background{
        new Fl_PNG_Image{"sprites/bluemoon.png"},
        new Fl_PNG_Image{"sprites/cityskyline.png"},
        new Fl_PNG_Image{"sprites/foggy.png"},
        new Fl_PNG_Image{"sprites/snowymountains.png"},
        new Fl_PNG_Image{"sprites/sunnyday.png"},
        new Fl_PNG_Image{"sprites/graveyard.png"}};

    Fl_PNG_Image heart{"sprites/heartcolored_fitted.png"};

   public:
    Track();
    ~Track();
};
