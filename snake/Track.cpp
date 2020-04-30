#include "Track.h"

#include <FL/fl_draw.H>

Track::Track() : Fl_Widget{0, 0, screenWidth, screenHeight} {
    newFood();
    for (auto& bg : background) {
        bg->scale(screenWidth, screenHeight);
    }
    heart.scale(snakeSize, snakeSize);
}

Track::~Track() {
    for (auto& bg : background) {
        delete bg;
    }
}

void Track::draw() {
    background[bgIndex]->draw(0, 0);

    if (!gameOver) {
        update();
    } else {
        game_over.draw(this->w() / 2 - game_over.data_w() / 2, this->h() / 2);
        restart_game.draw(this->w() / 2 - restart_game.data_w() / 2,
                          2 * this->h() / 3);
        if (Fl::event_key(FL_Enter)) {
            restart();
        }
    }

    heart.draw(food.x * snakeSize, food.y * snakeSize, snakeSize, snakeSize);
}

void Track::update() {
    const auto body = snake->getBody();
    const auto head = body.front();

    if (head.x < 0 || head.x >= (screenWidth / snakeSize) || head.y < 0 ||
        head.y >= (screenHeight / snakeSize)) {
        snake->kill();
        gameOver = true;
        return;
    }

    for (auto it = body.cbegin() + 1; it != body.cend(); it++) {
        if (*it == head) {
            snake->kill();
            gameOver = true;
            return;
        }
    }

    if (head == food) {
        snake->addPart();
        newFood();
        if (snake->getSize() % acc == 0) bgIndex++;
        if (bgIndex == (int)background.size()) bgIndex = 0;
    }
}

void Track::newFood() {
    auto body = snake->getBody();
    int foodCount = 0;
    while (foodCount < 1) {
        food.x = rand() % (screenWidth / snakeSize);
        food.y = rand() % (screenHeight / snakeSize);
        foodCount++;
        for (const auto& b : body) {
            if (food == b) {
                foodCount--;
                continue;
            }
        }
    }
}

void Track::restart() {
    snake->restart();
    newFood();
    bgIndex = 0;
    gameOver = false;
}