#include "SnakeAI.h"

#include <FL/fl_draw.H>

SnakeAI::SnakeAI() : Fl_Widget{0, 0, screenWidth, screenHeight}, pathIndex{1} {
    try {
        Hamiltonian pathGenerator;
        pathGenerator.convertSize(snakeSize);
        path = pathGenerator.getPath();
        body.push_back(path.front());
        newFood();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << " Exit ...\n";
        std::cin.get();
        std::exit(1);
    }
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

void SnakeAI::simpleDraw() {
    for (auto it = body.cbegin() + 1; it != body.cend(); it++) {
        draw_box(Fl_Boxtype::FL_BORDER_BOX, it->x, it->y, snakeSize, snakeSize,
                 FL_GREEN);
    }
    draw_box(Fl_Boxtype::FL_BORDER_BOX, body.front().x, body.front().y,
             snakeSize, snakeSize, FL_BLUE);
    draw_box(Fl_Boxtype::FL_BORDER_BOX, food.x, food.y, snakeSize, snakeSize,
             FL_RED);
}

void SnakeAI::advancedDraw() {
    fl_color(FL_GREEN);
    fl_line_style(0, snakeSize / 2);
    for (int i{0}; i < (int)body.size() - 1; i++) {
        fl_line(body[i].x + snakeSize / 2, body[i].y + snakeSize / 2,
                body[i + 1].x + snakeSize / 2, body[i + 1].y + snakeSize / 2);
    }
    draw_box(Fl_Boxtype::FL_BORDER_BOX, body.front().x, body.front().y,
             snakeSize, snakeSize, FL_BLUE);
    draw_box(Fl_Boxtype::FL_BORDER_BOX, food.x, food.y, snakeSize, snakeSize,
             FL_RED);
}

void SnakeAI::draw() {
    update();
    // simpleDraw();
    advancedDraw();
}