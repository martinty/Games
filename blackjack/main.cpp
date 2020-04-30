#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

#include <chrono>
#include <memory>
#include <thread>

#include "Blackjack.h"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    auto win = std::make_unique<Fl_Double_Window>(screenWidth, screenHeight,
                                                  "Blackjack");
    win->color(FL_DARK_GREEN);

    auto game = std::make_unique<Blackjack>();

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