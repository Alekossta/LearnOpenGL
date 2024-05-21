#pragma once
#include "Model.h"

class ModelTextured : public Model
{
private:
	unsigned TextureFromFile(const char* path, const string& directory, bool gamma);

};

