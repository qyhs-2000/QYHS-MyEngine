#pragma once
#include <string>
#include <glad/glad.h>
#include <unordered_map>
class Shader
{
public:

	void Parse(std::string shaderName);

	static Shader* find(std::string shaderName);
	inline GLuint getProgram() { return this->program; }
private:
	std::string  shaderName;
	GLuint program;
	void createGPUProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	std::unordered_map<std::string, GLuint> vertexShaderMap;
};