#pragma once
#include <list>
#include <memory>

#include "BeautyBird.h"
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Widget.H"
#include "bird.h"
#include "pipe.h"
#include "score.h"

class Track : public Fl_Widget {
   private:
    void draw() override;
    void update();
    void addPipe();
    bool collision() const;
    void restart();

    std::unique_ptr<Bird> bird =
        std::make_unique<BeautyBird>(' ', "sprites/bluebird-downflap.png");
    // std::unique_ptr<Bird> bird = std::make_unique<Bird>();

    std::unique_ptr<Score> score;
    static Fl_PNG_Image background;
    static Fl_PNG_Image base;
    Fl_PNG_Image game_over{"sprites/gameover.png"};
    Fl_PNG_Image background_night{"sprites/background-night.png"};
    Fl_PNG_Image initialize_game{"sprites/message.png"};
    Fl_PNG_Image restart_im{"sprites/Restart.png"};
    const int ground;
    std::list<Pipe> pipes;
    bool gameOver;
    bool initialized;

   public:
    Track(int w, int h);
};
