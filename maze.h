#pragma once

#include "mesh.h"
#include <vector>

struct Grid
{
    int value;
    bool* walls;
};

class Maze
{
public:
    float scale = 10, offsetX = 0, offsetY = 0;

    Maze(int size);
    ~Maze();

    void init();

    void setStartPoint(int x, int y);
    void setEndPoint(int x, int y);

    void update();
    void render();
private:
    int size;
    Grid** maze;

    int startX = -1, startY = -1;
    int endX = -1, endY = -1;

    Mesh2DSimple* mesh = nullptr;

    void generateMesh();
    void generateGridMesh(int x, int y, Grid grid, std::vector<unsigned int>* indices);

    void path();
    bool outOfBound(int x, int y);
    void clearValue();
};
