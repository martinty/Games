#include "Player.h"

#include <FL/fl_draw.H>

Player::Player(std::vector<Cell>& grid)
    : Fl_Widget{0, 0, cellSize, cellSize}, grid{grid} {
    init();
}

void Player::draw() {
    update();
    fl_color(FL_GREEN);
    fl_rectf(pos.getX() * cellSize, pos.getY() * cellSize, cellSize, cellSize);
    for (const Pos& p : tail) {
        fl_color(FL_BLUE);
        fl_rectf(p.getX() * cellSize + cellSize / 4,
                 p.getY() * cellSize + cellSize / 4, cellSize / 2,
                 cellSize / 2);
    }
}

void Player::update() {
    Pos dir = steer();
    Pos newPos = pos + dir;
    if (pos != newPos && newPos.valid() &&
        grid[newPos.getX() + newPos.getY() * XMAX].getMaze()) {
        if (!tail.empty()) {
            if (tail.back() == newPos) {
                tail.pop_back();
            } else {
                tail.push_back(pos);
            }
        } else {
            tail.push_back(pos);
        }
        pos = newPos;
    }
}

Pos Player::steer() const {
    if (Fl::event_key(FL_Left)) {
        return Pos{-1, 0};
    } else if (Fl::event_key(FL_Right)) {
        return Pos{1, 0};
    } else if (Fl::event_key(FL_Down)) {
        return Pos{0, 1};
    } else if (Fl::event_key(FL_Up)) {
        return Pos{0, -1};
    }
    return Pos{0, 0};
}

void Player::init() {
    for (const Cell& g : grid) {
        if (g.getMaze()) {
            pos = g.getPos();
            break;
        }
    }
}

void Player::reset(std::vector<Cell>& newGrid) {
    tail.clear();
    grid = newGrid;
    init();
}