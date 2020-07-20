#pragma once

class Mesh2DSimple
{
public:
	Mesh2DSimple(float* vertices, int verticesSize, unsigned int* indices, int indicesSize);
	~Mesh2DSimple();

	void render();
private:
	unsigned int vaoId, vboId, eboId;

	int verticesSize;
	float* vertices;

	int indicesSize;
	unsigned int* indices;
};
