#pragma once
#include "Mesh.h"
class MeshTextured : public Mesh
{
public:
	MeshTextured(vector<Vertex> vertices, vector<unsigned> indices, bool bSetupMesh);

	virtual void setupMesh(GLsizei additionalData) override;
};

