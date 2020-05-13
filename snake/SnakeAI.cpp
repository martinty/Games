#include "SnakeAI.h"

#include <FL/fl_draw.H>

SnakeAI::SnakeAI() : Fl_Widget{0, 0, screenWidth, screenHeight}, pathIndex{1} {
    Hamiltonian pathGenerator;
    path = pathGenerator.getPath();
    body.push_back(path.front());
    newFood();
}

void SnakeAI::newFood() {
    int foodCount = 0;
    while (foodCount < 1) {
        food.x = (rand() % (screenWidth / snakeSize)) * snakeSize;
        food.y = (rand() % (screenHeight / snakeSize)) * snakeSize;
        foodCount++;
        for (const auto& b : body) {
            if (food == b) {
                foodCount--;
                continue;
            }
        }
    }
}

void SnakeAI::update() {
    body.push_front(path[pathIndex++]);
    if (pathIndex == (int)path.size()) pathIndex = 0;
    if (body.front() == food) {
        newFood();
    } else {
        body.pop_back();
    }
}

void SnakeAI::draw() {
    update();
    for (auto it = body.cbegin() + 1; it != body.cend(); it++) {
        draw_box(Fl_Boxtype::FL_BORDER_BOX, it->x, it->y, snakeSize, snakeSize,
                 FL_GREEN);
    }
    draw_box(Fl_Boxtype::FL_BORDER_BOX, body.front().x, body.front().y,
             snakeSize, snakeSize, FL_BLUE);
    draw_box(Fl_Boxtype::FL_BORDER_BOX, food.x, food.y, snakeSize, snakeSize,
             FL_RED);
}