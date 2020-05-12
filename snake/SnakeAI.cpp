#include "SnakeAI.h"

#include <FL/fl_draw.H>

#include <iostream>

SnakeAI::SnakeAI() : Fl_Widget{0, 0, screenWidth, screenHeight}, routeIndex{0} {
    newFood();
    Pos start;
    for (;;) {
        start.x = (rand() % (screenWidth / snakeSize)) * snakeSize;
        start.y = (rand() % (screenHeight / snakeSize)) * snakeSize;
        if (start != food) break;
    }
    body.push_back(start);
    makeHamiltonianPath(start);
}

void SnakeAI::makeHamiltonianPath(Pos start) {
    route.push_back(start);
    for (const auto& d : directions) {
        Pos p = start - d;
        if (p.x >= 0 && p.x < w() && p.y >= 0 && p.y < h()) {
            end = p;
            break;
        }
    }
    recHamCycle();
    return;
}

bool SnakeAI::recHamCycle() {
    if (route.size() < N) {
        for (const auto& d : directions) {
            Pos newPos = route.back() + d;
            if (newPos == end && route.size() < N - 1) {
                continue;
            } else if (newPos.x < 0 || newPos.x >= w() || newPos.y < 0 ||
                       newPos.y >= h()) {
                continue;
            }
            bool good = true;
            for (const auto& p : route) {
                if (p == newPos) {
                    good = false;
                    break;
                }
            }
            if (good) {
                route.push_back(newPos);
                if (!recHamCycle()) {
                    route.pop_back();
                } else {
                    return true;
                }
            }
        }
    } else {
        return true;
    }
    return false;
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
    body.push_front(route[routeIndex++]);
    if (routeIndex == (int)route.size()) routeIndex = 0;
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