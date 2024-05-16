#pragma once
#include "Shader.h"
#include "vector"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model
{
public:
	Model(const char* path);
	void Draw(Shader& shader);
private:
	vector<Mesh> meshes;
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	unsigned TextureFromFile(const char* path, const string& directory, bool gamma);
};

