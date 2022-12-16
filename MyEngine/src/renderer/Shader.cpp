#include "Shader.h"
#include <fstream>
#include <iostream>
std::unordered_map<std::string, Shader*> shaderMap;

void Shader::Parse( std::string shaderName)
{
	this->shaderName = shaderName;
	std::string vertexShader = shaderName + ".vs";
	std::string fragmentShader = shaderName + ".fs";

	std::ifstream vertex_input_file_stream(vertexShader, std::ios::in | std::ios::binary);
	std::string vertexShaderSource((std::istreambuf_iterator<char>(vertex_input_file_stream)), std::istreambuf_iterator<char>());

	std::ifstream fragment_input_file_stream(fragmentShader, std::ios::in | std::ios::binary);
	std::string fragmentShaderSource((std::istreambuf_iterator<char>(fragment_input_file_stream)), std::istreambuf_iterator<char>());

	createGPUProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

}

void Shader::createGPUProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint compileState = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileState);

	if (compileState == GL_FALSE)
	{
		GLchar message[256];
		glGetShaderInfoLog(vertexShader, sizeof(message), 0, message);
		std::cout << "compile vs error:" << message << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	compileState = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileState);

	if (compileState == GL_FALSE)
	{
		GLchar message[256];
		glGetShaderInfoLog(fragmentShader, sizeof(message), 0, message);
		std::cout << "compile vs error:" << message << std::endl;
	}


	//链接program和shader
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	//查看程序链接的结果
	GLint link_status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE)
	{
		GLchar message[256];
		glGetProgramInfoLog(program, sizeof(message), 0, message);
		std::cout << "link error:" << message << std::endl;
	}

}

Shader* Shader::find(std::string shaderName)
{
	std::unordered_map<std::string, Shader*>::iterator iter = shaderMap.find(shaderName);
	if (iter != shaderMap.end())
	{
		return iter->second;
	}
	Shader* shader = new Shader();

	shader->Parse(shaderName);

	shaderMap.insert(std::pair<std::string, Shader*>(shaderName, shader));
	return shader;

}
