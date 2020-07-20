#include "maze.h"
#include "glfw3.h"
#include "window.h"

#include <glm.hpp>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int getOpDir(int dir)
{
    switch (dir)
    {
    case UP: return DOWN;
    case DOWN: return UP;
    case LEFT: return RIGHT;
    case RIGHT: return LEFT;
    }
    return -1;
}

const int FOUR[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

Maze::Maze(int size)
{
    this->size = size;
    maze = new Grid * [size];
    for (int x = 0; x < size; x++)
    {
        maze[x] = new Grid[size];
        for (int y = 0; y < size; y++)
        {
            maze[x][y] = { 0, new bool[4] { true, true, true, true } };
        }
    }

    offsetX = Window::width / 2;
    offsetY = Window::height / 2;

    init();
    generateMesh();
}

Maze::~Maze()
{
    delete mesh;

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            delete[] maze[x][y].walls;
        }
        delete[] maze[x];
    }
    delete[] maze;
}

void Maze::generateMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    for (int x = 0; x <= size; x++)
    {
        for (int y = 0; y <= size; y++)
        {
            vertices.push_back(x - size / 2);
            vertices.push_back(y - size / 2);
            if (x < size && y < size)
                generateGridMesh(x, y, maze[x][y], &indices);
        }
    }
    mesh = new Mesh2DSimple(vertices.data(), vertices.size(), indices.data(), indices.size());
}

void Maze::generateGridMesh(int x, int y, Grid grid, std::vector<unsigned int>* indices)
{
    unsigned int topLeft = x * (size + 1) + y;
    unsigned int topRight = topLeft + size + 1;
    unsigned int bottomLeft = topLeft + 1;
    unsigned int bottomRight = topRight + 1;
    if (grid.value > 0 || (x == startX && y == startY) || (x == endX && y == endY))
    {
        // vertices->push_back(sf::Vertex(sf::Vector2f(sx, sy)));
        // vertices->push_back(sf::Vertex(sf::Vector2f(ex, ey)));
        // vertices->push_back(sf::Vertex(sf::Vector2f(sx, ey)));
        // vertices->push_back(sf::Vertex(sf::Vector2f(ex, sy)));
    }
    if (y == 0 && grid.walls[0])
    {
        indices->push_back(topLeft);
        indices->push_back(topRight);
    }
    if (grid.walls[1])
    {
        indices->push_back(bottomLeft);
        indices->push_back(bottomRight);
    }
    if (x == 0 && grid.walls[2])
    {
        indices->push_back(topLeft);
        indices->push_back(bottomLeft);
    }
    if (grid.walls[3])
    {
        indices->push_back(topRight);
        indices->push_back(bottomRight);
    }
}

bool Maze::outOfBound(int x, int y)
{
    return x < 0 || x >= size || y < 0 || y >= size;
}

void Maze::clearValue()
{
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            maze[x][y].value = 0;
}

void Maze::init()
{
    int x = rand() % size;
    int y = rand() % size;
    int distance = 0;
    while (true) {
        maze[x][y].value = --distance;
        bool traceBack = false;
        bool d[4] = { false, false, false, false };
        while (!traceBack) {
            const int dir = rand() % 4;
            if (!d[dir])
            {
                d[dir] = true;
                const int nextX = x + FOUR[dir][0];
                const int nextY = y + FOUR[dir][1];
                if (!outOfBound(nextX, nextY) && maze[nextX][nextY].value == 0)
                {
                    maze[x][y].walls[dir] = false;
                    maze[nextX][nextY].walls[getOpDir(dir)] = false;
                    x = nextX;
                    y = nextY;
                    break;
                }
            }
            traceBack = d[0] && d[1] && d[2] && d[3];
        }
        if (traceBack) {
            for (int i = 0; i < 4; i++) {
                const int nextX = x + FOUR[i][0];
                const int nextY = y + FOUR[i][1];
                if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value != 0 && maze[nextX][nextY].value > maze[x][y].value) {
                    x = nextX;
                    y = nextY;
                    distance = maze[nextX][nextY].value + 1;
                    break;
                }
                if (i >= 3) {
                    return;
                }
            }
        }
    }
}

void Maze::path()
{
    const int valueDir = -1;
    int value = valueDir;
    maze[startX][startY].value = value;
    std::vector<glm::ivec2> heads;
    std::vector<glm::ivec2> newHeads;
    heads.push_back(glm::ivec2(startX, startY));
    while (true) {
        for (int h = 0; h < heads.size(); h++) {
            int x = heads[h].x;
            int y = heads[h].y;
            if (x == endX && y == endY) {
                // retrace
                while (true) {
                    if (x == startX && y == startY) {
                        break;
                    }
                    for (int i = 0; i < 4; i++) {
                        const int nextX = x + FOUR[i][0];
                        const int nextY = y + FOUR[i][1];
                        if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value != 0 && abs(maze[nextX][nextY].value) < abs(maze[x][y].value)) {
                            maze[x][y].value = abs(maze[x][y].value);
                            x = nextX;
                            y = nextY;
                            break;
                        }
                    }
                }
                return;
            }
            for (int i = 0; i < 4; i++) {
                const int nextX = x + FOUR[i][0];
                const int nextY = y + FOUR[i][1];
                if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value == 0) {
                    maze[nextX][nextY].value = value + valueDir;
                    newHeads.push_back(glm::ivec2(nextX, nextY));
                }
            }
        }
        heads.clear();
        heads.insert(heads.end(), newHeads.begin(), newHeads.end());
        newHeads.clear();
        value += valueDir;
    }
}

void Maze::setStartPoint(int x, int y)
{
    startX = x;
    startY = y;
    clearValue();
}

void Maze::setEndPoint(int x, int y)
{
    endX = x;
    endY = y;
    if (!outOfBound(startX, startY) && !outOfBound(endX, endY))
    {
        clearValue();
        path();
    }
}

void Maze::update()
{
    if (glfwGetKey(Window::window, GLFW_KEY_S) == GLFW_PRESS) offsetY += 0.1f;
    if (glfwGetKey(Window::window, GLFW_KEY_W) == GLFW_PRESS) offsetY -= 0.1f;
    if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS) offsetX += 0.1f;
    if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS) offsetX -= 0.1f;
    if (glfwGetKey(Window::window, GLFW_KEY_E) == GLFW_PRESS) scale += 0.001f;
    if (glfwGetKey(Window::window, GLFW_KEY_Q) == GLFW_PRESS) scale -= 0.001f;
}

void Maze::render()
{
    mesh->render();
}
