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
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned id;
	string type;
	string path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned> indices, vector<Texture> textures);
	void Draw(Shader& shader);

private:
	unsigned VAO, VBO, EBO;

	void setupMesh();
};

