#pragma once

#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Widget.H"

class Bird : public Fl_Widget {
   protected:
    double velocity = 0;
    bool stopBird;
    void update();

   private:
    static constexpr double acceleration = 0.4;
    void draw() override;
    virtual void steer();

   public:
    Bird();
    void setGameOver() { stopBird = true; }
    void initializeGame() { stopBird = false; }
    void reset();
};
