#include "Snake.h"

#include <FL/fl_draw.H>

Snake::Snake() : Fl_Widget{0, 0, snakeSize, snakeSize} {}

void Snake::draw() {
    if (alive) {
        frames++;
        if (frames > FPS / speed) {
            steer();
            update();
            frames = 0;
        }
    }

    bool first = true;
    for (const auto& b : body) {
        fl_color(FL_GREEN);
        if (first) {
            int a1, a2;
            switch (dir) {
                case Direction::left:
                    a1 = -180 + 45;
                    a2 = 180 - 45;
                    break;
                case Direction::right:
                    a1 = 0 + 45;
                    a2 = 360 - 45;
                    break;
                case Direction::down:
                    a1 = -90 + 45;
                    a2 = 270 - 45;
                    break;
                case Direction::up:
                    a1 = -270 + 45;
                    a2 = 90 - 45;
                    break;
                default:
                    break;
            }
            fl_pie(b.x * snakeSize, b.y * snakeSize, snakeSize, snakeSize, a1,
                   a2);
            first = false;
        } else {
            fl_begin_polygon();
            fl_circle(b.x * snakeSize + snakeSize / 2,
                      b.y * snakeSize + snakeSize / 2, snakeSize / 2);
            fl_end_polygon();
        }
    }
}

void Snake::update() {
    Pos movement = body.front();
    if (grow) {
        grow = false;
        if (getSize() % acc == 0) speed += acc;
    } else {
        body.pop_back();
    }
    switch (dir) {
        case Direction::left:
            movement.x--;
            break;
        case Direction::right:
            movement.x++;
            break;
        case Direction::down:
            movement.y++;
            break;
        case Direction::up:
            movement.y--;
            break;
        default:
            break;
    }
    body.push_front(movement);
}

void Snake::steer() {
    if (Fl::event_key(FL_Left)) {
        if (dir != Direction::right) dir = Direction::left;
    } else if (Fl::event_key(FL_Right)) {
        if (dir != Direction::left) dir = Direction::right;
    } else if (Fl::event_key(FL_Down)) {
        if (dir != Direction::up) dir = Direction::down;
    } else if (Fl::event_key(FL_Up)) {
        if (dir != Direction::down) dir = Direction::up;
    }
}

void Snake::restart() {
    body.clear();
    body.push_front(startPos);
    frames = 0;
    speed = 20;
    grow = false;
    dir = Direction::right;
    alive = true;
}