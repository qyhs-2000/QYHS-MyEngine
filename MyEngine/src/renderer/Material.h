#pragma once
#include <string>
#include <vector>
#include "Texture2D.h"
#include "Shader.h"
class Texture2D;
class Material
{
public:
	Material()
	{

	}
	std::vector<std::pair<std::string, Texture2D*>> getTextures()
	{
		return this->textures;
	}
	void Parse(std::string materialPath);
	inline Shader* getShader() { return this->shader; }
private:
	Shader* shader;
	std::vector<std::pair<std::string, Texture2D*>> textures;
};