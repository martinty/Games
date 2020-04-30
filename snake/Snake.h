#pragma once

#include <FL/Fl_Widget.H>

#include <deque>

#include "utilities.h"

class Snake : public Fl_Widget {
   private:
    void draw() override;
    void update();
    void steer();
    std::deque<Pos> body{startPos};
    Direction dir = Direction::right;
    int frames = 0;
    int speed = 20;
    bool alive = true;
    bool grow = false;

   public:
    Snake();
    const auto getBody() const { return body; }
    int getSize() const { return (int)body.size(); }
    void kill() { alive = false; }
    void addPart() { grow = true; }
    void restart();
};