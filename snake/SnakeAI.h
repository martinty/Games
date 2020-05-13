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
    std::vector<Pos> route;
    int routeIndex;

    static constexpr int N = screenWidth / snakeSize * screenHeight / snakeSize;
    Pos end;
    std::vector<Pos> directions{Pos{snakeSize, 0}, Pos{-snakeSize, 0},
                                Pos{0, snakeSize}, Pos{0, -snakeSize}};

    double counter = 0;

   public:
    SnakeAI();
    ~SnakeAI() = default;
};