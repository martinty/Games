#include "Maze.h"

#include <FL/fl_draw.H>

#include <iterator>
#include <list>

Maze::Maze() : Fl_Widget{0, 0, screenWidth, screenHeight} {
    init();
    player = std::make_unique<Player>(grid);
}

void Maze::init() {
    for (int y{YMIN}; y < YMAX; y++) {
        for (int x{XMIN}; x < XMAX; x++) {
            grid.push_back(Cell{x, y});
        }
    }
    generateMaze();
    for (auto it = grid.crbegin(); it != grid.crend(); it++) {
        if ((*it).getMaze()) {
            goal = (*it).getPos();
            break;
        }
    }
}

void Maze::reset() {
    grid.clear();
    init();
    player->reset(grid);
}

void Maze::generateMaze() {
    std::list<Cell*> cellList;

    int startX = rand() % (XMAX - 2) + 1;
    int startY = rand() % (YMAX - 2) + 1;
    Cell* startCell = &grid[startX + startY * XMAX];
    startCell->setVisited(true);
    cellList.push_back(startCell);

    while (!cellList.empty()) {
        int i = rand() % (int)cellList.size();
        auto it = cellList.begin();
        std::advance(it, i);
        Cell* cell = *it;

        std::vector<Pos> neighborsPos = cell->getNeighborsPos();
        std::vector<Pos> cornersPos = cell->getCornersPos();
        int neighbors = 0;
        int corners = 0;

        for (const Pos& n : neighborsPos) {
            Cell* c = &grid[n.getX() + n.getY() * XMAX];
            if (c->getMaze()) neighbors++;
        }

        for (const Pos& n : cornersPos) {
            Cell* c = &grid[n.getX() + n.getY() * XMAX];
            if (c->getMaze()) corners++;
        }

        if (neighbors <= 1 && corners <= 1) {
            cell->setMaze(true);
            for (const Pos& n : neighborsPos) {
                Cell* c = &grid[n.getX() + n.getY() * XMAX];
                if (!c->getVisited()) {
                    c->setVisited(true);
                    cellList.push_back(c);
                }
            }
        }
        cellList.erase(it);
    }
}

void Maze::update() {
    if (goal == player->getPos()) {
        reset();
    }
}

void Maze::draw() {
    update();
    fl_color(FL_WHITE);
    for (const Cell& cell : grid) {
        if (cell.getMaze()) {
            Pos pos = cell.getPos();
            fl_rectf(pos.getX() * cellSize, pos.getY() * cellSize, cellSize,
                     cellSize);
        }
    }
    fl_color(FL_RED);
    fl_rectf(goal.getX() * cellSize, goal.getY() * cellSize, cellSize,
             cellSize);
}