#include "SnakeAI.h"

#include <FL/fl_draw.H>

#include <iostream>

SnakeAI::SnakeAI() : Fl_Widget{0, 0, screenWidth, screenHeight}, routeIndex{1} {
    // Pos start{0, h() - snakeSize};
    // body.push_back(start);

    Hamiltonian pathGenerator;
    route = pathGenerator.getPath();

    body.push_back(route.front());
    newFood();

    // makeHamiltonianPath(start);
}

void SnakeAI::makeHamiltonianPath(Pos start) {
    route.push_back(start);
    end = Pos{w() - snakeSize, h() - snakeSize};
    if (!recHamCycle()) {
        std::cout << "Counter: " << counter << "\n";
        std::cout << "No solution!\n";
        std::cin.get();
        std::exit(1);
    }
    std::cout << "Counter: " << counter << "\n";
    return;
}

bool SnakeAI::recHamCycle() {
    if (route.size() < N) {
        int i = rand() % (int)directions.size();
        for (int d{0}; d < (int)directions.size(); d++) {
            Pos newPos = route.back() + directions[i];
            i++;
            if (i == 4) i = 0;
            counter++;
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