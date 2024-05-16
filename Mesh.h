#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"

using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned> indices;

	Mesh(vector<Vertex> vertices, vector<unsigned> indices);
	void Draw();

private:
	unsigned VAO, VBO, EBO;

	void setupMesh();
};

