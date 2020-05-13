#pragma once

#include <FL/Fl_Widget.H>

#include <deque>
#include <vector>

#include "Hamiltonian.h"
#include "utilities.h"

class SnakeAI : public Fl_Widget {
   private:
    void makeHamiltonianPath(Pos start);
    bool recHamCycle();
    void newFood();
    void update();
    void draw() override;

    Pos food;
    std::deque<Pos> body;
    std::vector<Pos> path;
    int pathIndex;

   public:
    SnakeAI();
    ~SnakeAI() = default;
};