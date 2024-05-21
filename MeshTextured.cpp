#include "MeshTextured.h"

MeshTextured::MeshTextured(vector<Vertex> vertices, vector<unsigned> indices, bool bSetupMesh) : Mesh(vertices,indices, false)
{
	if (bSetupMesh)
	{
		setupMesh(sizeof(float) * 2);
	}
}

void MeshTextured::setupMesh(GLsizei additionalData)
{
	Mesh::setupMesh(additionalData);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex) + additionalData, (void*)sizeof(Vertex));
	glEnableVertexAttribArray(2);
}