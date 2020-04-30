#pragma once

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>

#include <string>

class Button : public Fl_Widget {
   private:
    std::string name;
    bool clicked = false;
    void draw() override {
        if (clicked) {
            fl_draw_box(Fl_Boxtype::FL_BORDER_BOX, x(), y(), w(), h(),
                        FL_GREEN);
        } else {
            fl_draw_box(Fl_Boxtype::FL_BORDER_BOX, x(), y(), w(), h(), FL_GRAY);
        }
        this->draw_label();
    }

   public:
    Button(int x, int y, int w, int h, const std::string& str)
        : Fl_Widget{x, y, w, h}, name{str} {
        label(name.c_str());
    }
    ~Button() = default;
    bool click() {
        if (!visible()) return false;
        int x = Fl::event_x();
        int y = Fl::event_y();
        if (x > this->x() && x < this->x() + w() && y > this->y() &&
            y < this->y() + h()) {
            if (Fl::event_state(FL_BUTTON1)) {
                if (!clicked) {
                    clicked = true;
                    return true;
                }
            } else {
                clicked = false;
            }
        } else {
            clicked = false;
        }
        return false;
    }
};