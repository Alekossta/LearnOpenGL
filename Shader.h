#pragma once

#include <glad/glad.h>
#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix(const std::string& name, const glm::mat4& matrix) const;
	void setVector3(const std::string& name, const glm::vec3& vector) const;
};

