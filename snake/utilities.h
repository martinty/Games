#pragma once

enum class Direction { left, right, up, down };

struct Pos {
    int x = 0;
    int y = 0;
    bool operator==(const Pos& rhs) const { return (x == rhs.x && y == rhs.y); }
    bool operator!=(const Pos& rhs) const { return (x != rhs.x || y != rhs.y); }
    Pos operator-(const Pos& rhs) const { return Pos{x - rhs.x, y - rhs.y}; }
    Pos operator+(const Pos& rhs) const { return Pos{x + rhs.x, y + rhs.y}; }
    Pos& operator*=(const int& n) {
        x *= n;
        y *= n;
        return *this;
    }
};

constexpr int sX = 5;
constexpr int sY = 5;
constexpr int bX = 8;
constexpr int bY = 4;
constexpr int sN = sX * sY;
constexpr int bN = bX * bY;

constexpr int snakeSize = 20;
constexpr int screenWidth = sX * bX * snakeSize;   // 960;
constexpr int screenHeight = sY * bY * snakeSize;  // 540;
constexpr int FPS = 1000;
constexpr Pos startPos{10, 10};
constexpr int acc = 4;
