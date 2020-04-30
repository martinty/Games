#pragma once

#include <vector>

constexpr int screenWidth = 960;
constexpr int screenHeight = 540;
constexpr int FPS = 10;
constexpr int cellSize = 20;
constexpr int XMAX = screenWidth / cellSize;
constexpr int YMAX = screenHeight / cellSize;
constexpr int XMIN = 0;
constexpr int YMIN = 0;

class Pos;
class Wall;
class Cell;

class Pos {
   private:
    int x, y;

   public:
    Pos(int x, int y) : x{x}, y{y} {}
    Pos() : Pos{0, 0} {};
    ~Pos() = default;
    bool operator==(const Pos& rhs) const { return (x == rhs.x && y == rhs.y); }
    bool operator!=(const Pos& rhs) const { return !(*this == rhs); }
    Pos operator+(const Pos& rhs) const { return Pos{x + rhs.x, y + rhs.y}; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool valid(int xMin = XMIN + 1, int xMax = XMAX - 1, int yMin = YMIN + 1,
               int yMax = YMAX - 1) const {
        return (x >= xMin && x < xMax && y >= yMin && y < yMax);
    }
};

class Cell {
   private:
    Pos pos;
    std::vector<Pos> neighborsPos;
    std::vector<Pos> cornersPos;
    bool visited = false;
    bool maze = false;

   public:
    Cell(int x, int y) : pos{x, y} {
        for (int i{-1}; i <= 1; i += 2) {
            Pos n = Pos{x + i, y};
            if (n.valid()) neighborsPos.push_back(n);
            n = Pos{x, y + i};
            if (n.valid()) neighborsPos.push_back(n);
        }
        for (int dy{-1}; dy <= 1; dy += 2) {
            for (int dx{-1}; dx <= 1; dx += 2) {
                Pos n = Pos{x + dx, y + dy};
                if (n.valid() && pos != n) {
                    cornersPos.push_back(n);
                }
            }
        }
    }
    Cell() : Cell{0, 0} {};
    ~Cell() = default;
    bool operator==(const Cell& rhs) const { return (pos == rhs.pos); }
    Pos getPos() const { return pos; }
    std::vector<Pos> getNeighborsPos() const { return neighborsPos; }
    std::vector<Pos> getCornersPos() const { return cornersPos; }
    bool getVisited() const { return visited; }
    void setVisited(bool value) { visited = value; }
    bool getMaze() const { return maze; }
    void setMaze(bool value) { maze = value; }
};