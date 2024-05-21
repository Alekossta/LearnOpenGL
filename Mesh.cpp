#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned> indices, bool bSetupMesh)
{
	this->vertices = vertices;
	this->indices = indices;

	if (bSetupMesh)
	{
		setupMesh(0);
	}

}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh(GLsizei additionalData)
{
	// GENERATE OBJECTS
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	// DATA FOR VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// DATA FOR EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	// SETUP ATTRIBUTES

	// // POSITION ATTRIBUTE
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) + additionalData, (void*)0);
	glEnableVertexAttribArray(0);

	// // NORMAL ATTRIBUTE
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) + additionalData, (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
}
