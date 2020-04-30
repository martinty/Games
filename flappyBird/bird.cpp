#include "bird.h"

#include <FL/fl_draw.H>

#include <cmath>

#include "constants.h"

Bird::Bird()
    : Fl_Widget(birdPosition, birdPosition * 2, 34, 24), stopBird{true} {}

void Bird::draw() {
    if (!stopBird) {
        steer();
        update();
    }

    fl_color(0xFF000000);
    fl_line_style(0, 5);
    fl_circle(x() + w() / 2, y() + h() / 2, this->w() / 2);
    double angle = 6 * velocity * 3.1415 / 180;
    if (angle > 1.57) {
        angle = 1.57;
    }
    fl_line(x() + w() / 2, y() + h() / 2, x() + w() / 2 + 30 * cos(angle),
            y() + h() / 2 + 30 * sin(angle));
}

void Bird::steer() {
    // Handles user input
    if (Fl::event_key(' ')) {
        velocity = -flapVelocity;
    }
    // Det finnes en handle()-funksjon i Fl_Widget som kan implementeres.
    // Dette ville vært en bedre løsning, men er ikke gjort i øvingen.
}

void Bird::update() {
    // Handles physics
    velocity += birdAcceleration;
    double position = y() + velocity;
    if (position < 0) {
        // Not allowed outside of screen
        position = 0;
        velocity = 0;
    }
    y(position);
}

void Bird::reset() {
    y(birdPosition);
    velocity = 0;
    stopBird = true;
}
