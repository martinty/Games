#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

#include <chrono>
#include <memory>
#include <thread>

#include "SnakeAI.h"
#include "Track.h"
#include "utilities.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    auto win =
        std::make_unique<Fl_Double_Window>(screenWidth, screenHeight, "Snake");
    win->color(FL_BLACK);

    // Track* track = new Track();
    auto ai = std::make_unique<SnakeAI>();

    win->end();
    win->show();

    auto nextDrawing = std::chrono::steady_clock::now();

    while (win->shown()) {
        std::this_thread::sleep_until(nextDrawing);
        nextDrawing += std::chrono::microseconds(1'000'000 / FPS);

        Fl::check();
        Fl::redraw();
    }
}