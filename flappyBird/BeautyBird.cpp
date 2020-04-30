#include "BeautyBird.h"

#include <algorithm>
#include <iostream>

#include "constants.h"
#include "rotate.h"

BeautyBird::BeautyBird(char key, const char* spritePath)
    : Bird(), sprite(spritePath), key(key){};

void BeautyBird::draw() {
    if (!stopBird) {
        steer();
        update();
    }

    drawRotatedImage(sprite, x(), y(),
                     static_cast<int>(std::max(-5 * velocity, -90.0)));
}

void BeautyBird::steer() {
    if (Fl::event_key(key)) {
        velocity = -flapVelocity;
    }
}