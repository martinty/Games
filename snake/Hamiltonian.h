#pragma once

#include <iostream>
#include <vector>

#include "utilities.h"

class Hamiltonian {
   private:
    Pos sStart, sEnd;
    const Pos bStart{0, 0};
    const Pos bEnd{0, 1};
    std::vector<Pos> sGrid;
    std::vector<Pos> bGrid{bStart};
    std::vector<Pos> path;
    const std::vector<Pos> dir{Pos{1, 0}, Pos{0, 1}, Pos{-1, 0}, Pos{0, -1}};
    const int dirSize = 4;

    bool sValidPos(const Pos& p) const {
        return (p.x >= 0 && p.x < sX && p.y >= 0 && p.y < sY);
    }
    bool bValidPos(const Pos& p) const {
        return (p.x >= 0 && p.x < bX && p.y >= 0 && p.y < bY);
    }

    bool sRecursive() {
        if (sGrid.size() < sN) {
            int i = rand() % dirSize;
            for (int d{0}; d < dirSize; d++) {
                Pos newPos = sGrid.back() + dir[i];
                i++;
                if (i == dirSize) i = 0;
                if (newPos == sEnd && sGrid.size() < sN - 1) {
                    continue;
                } else if (!sValidPos(newPos)) {
                    continue;
                }
                bool good = true;
                for (const auto& p : sGrid) {
                    if (p == newPos) {
                        good = false;
                        break;
                    }
                }
                if (good) {
                    sGrid.push_back(newPos);
                    if (!sRecursive()) {
                        sGrid.pop_back();
                    } else {
                        return true;
                    }
                }
            }
        } else {
            return true;
        }
        return false;
    }
    bool bRecursive() {
        if (bGrid.size() < bN) {
            int i = rand() % dirSize;
            for (int d{0}; d < dirSize; d++) {
                Pos newPos = bGrid.back() + dir[i];
                i++;
                if (i == dirSize) i = 0;
                if (newPos == bEnd && bGrid.size() < bN - 1) {
                    continue;
                } else if (!bValidPos(newPos)) {
                    continue;
                }
                bool good = true;
                for (const auto& p : bGrid) {
                    if (p == newPos) {
                        good = false;
                        break;
                    }
                }
                if (good) {
                    bGrid.push_back(newPos);
                    if (!bRecursive()) {
                        bGrid.pop_back();
                    } else {
                        return true;
                    }
                }
            }
        } else {
            return true;
        }
        return false;
    }

    void sInit(const Pos& inDir, const Pos& outDir) {
        if (inDir.x > 0 || inDir.y > 0) {
            sStart = Pos{0, 0};
        } else {
            sStart = Pos{sX - 1, sY - 1};
        }

        if (outDir.x > 0 || outDir.y < 0) {
            sEnd = Pos{sX - 1, 0};
        } else {
            sEnd = Pos{0, sY - 1};
        }
    }

    void buildTotalGrid(const int& i) {
        const int x = bGrid[i].x * sX;
        const int y = bGrid[i].y * sY;
        for (const auto& p : sGrid) {
            path.push_back(Pos{p.x + x, p.y + y});
        }
        sGrid.clear();
    }

   public:
    Hamiltonian() {
        if (!bRecursive()) {
            std::cout << "No solution for big grid! Exit...\n";
            std::exit(1);
        }

        Pos inDir = bStart - bEnd;
        Pos outDir = bGrid[1] - bStart;
        sInit(inDir, outDir);
        sGrid.push_back(sStart);
        if (!sRecursive()) {
            std::cout << 0 << ": No solution for small grid! Exit...\n";
            std::cin.get();
            std::exit(1);
        }
        buildTotalGrid(0);

        for (int i{1}; i < bN - 1; i++) {
            inDir = bGrid[i] - bGrid[i - 1];
            outDir = bGrid[i + 1] - bGrid[i];
            sInit(inDir, outDir);
            sGrid.push_back(sStart);
            if (!sRecursive()) {
                std::cout << i << ": No solution for small grid! Exit...\n";
                std::cin.get();
                std::exit(1);
            }
            buildTotalGrid(i);
        }

        inDir = bEnd - bGrid[bN - 2];
        outDir = bStart - bEnd;
        sInit(inDir, outDir);
        sGrid.push_back(sStart);
        if (!sRecursive()) {
            std::cout << bN - 1 << ": No solution for small grid!\n";
            std::cin.get();
            std::exit(1);
        }
        buildTotalGrid(bN - 1);
    }
    ~Hamiltonian() = default;

    std::vector<Pos> getPath() const { return path; }

    bool validPath() const {
        Pos zero{0, 0};
        Pos d = path.front() - path.back();
        if (d.x * d.x + d.y * d.y > 1) return false;
        if (d == zero) return false;
        for (int i{0}; i < (int)path.size() - 1; i++) {
            d = path[i] - path[i + 1];
            if (d.x * d.x + d.y * d.y > 1) return false;
            if (d == zero) return false;
        }
        return true;
    }

    void convertSize(int size) {
        for (auto& pos : path) {
            pos *= size;
        }
    }
};