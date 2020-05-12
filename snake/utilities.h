#pragma once

enum class Direction { left, right, up, down };

struct Pos {
    int x = 0;
    int y = 0;
    bool operator==(const Pos& rhs) const { return (x == rhs.x && y == rhs.y); }
    bool operator!=(const Pos& rhs) const { return (x != rhs.x || y != rhs.y); }
    Pos operator-(const Pos& rhs) const { return Pos{x - rhs.x, y - rhs.y}; }
    Pos operator+(const Pos& rhs) const { return Pos{x + rhs.x, y + rhs.y}; }
};

constexpr int screenWidth = 120;   // 960;
constexpr int screenHeight = 120;  // 540;
constexpr int FPS = 60;
constexpr int snakeSize = 20;
constexpr Pos startPos{10, 10};
constexpr int acc = 4;
