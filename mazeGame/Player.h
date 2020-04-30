#pragma once

#include <FL/Fl_Widget.H>

#include "Cell.h"

class Player : public Fl_Widget {
   private:
    void init();
    void draw() override;
    void update();
    Pos steer() const;
    std::vector<Cell>& grid;
    Pos pos;
    std::vector<Pos> tail;

   public:
    Player(std::vector<Cell>& grid);
    Player() = delete;
    ~Player() = default;
    void reset(std::vector<Cell>& grid);
    Pos getPos() const { return pos; }
};