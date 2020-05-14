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

constexpr int sX = 3;  // Need to be odd
constexpr int sY = 5;  // Need to be odd
constexpr int bX = 6;
constexpr int bY = 4;
constexpr int sN = sX * sY;  // Max 40
constexpr int bN = bX * bY;  // Need to be even, max 40

constexpr int snakeSize = 20;

#define AI
#ifdef AI
constexpr int screenWidth = sX * bX * snakeSize;   // 960;
constexpr int screenHeight = sY * bY * snakeSize;  // 540;
constexpr int FPS = 1000;
#else
constexpr int screenWidth = 960;
constexpr int screenHeight = 540;
constexpr int FPS = 60;
#endif

constexpr Pos startPos{10, 10};
constexpr int acc = 4;
