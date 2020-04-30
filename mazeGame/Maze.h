#pragma once

#include <FL/Fl_Widget.H>

#include <memory>

#include "Cell.h"
#include "FL/Fl_PNG_Image.H"
#include "Player.h"

class Maze : public Fl_Widget {
   private:
    std::vector<Cell> grid;
    void init();
    void generateMaze();
    void draw() override;
    void update();
    void reset();
    std::unique_ptr<Player> player;
    Pos goal;

   public:
    Maze();
    ~Maze() = default;
};
