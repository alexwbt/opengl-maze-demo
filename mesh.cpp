#include "mesh.h"

#include <glad/glad.h>

Mesh2DSimple::Mesh2DSimple(float* vertices, int verticesSize, unsigned int* indices, int indicesSize)
{
    this->vertices = vertices;
    this->verticesSize = verticesSize;
    this->indices = indices;
    this->indicesSize = indicesSize;

    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);
    glGenBuffers(1, &eboId);

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

Mesh2DSimple::~Mesh2DSimple()
{
    glDeleteVertexArrays(1, &vaoId);
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &eboId);
}

void Mesh2DSimple::render()
{
    glBindVertexArray(vaoId);
    glDrawElements(GL_LINES, indicesSize, GL_UNSIGNED_INT, 0);
}