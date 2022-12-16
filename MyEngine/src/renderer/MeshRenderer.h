#pragma once
#include "Material.h"
#include "MeshFilter.h"
#include <glm/glm.hpp>
class MeshRenderer:public Component
{
public:
	void setMaterial(Material* material);
	void setMesh(MeshFilter* mesh);
	void setProjection(glm::mat4 projection);
	void setView(glm::mat4 view);
	void Renderer();
	GLuint getProgram()
	{
		return this->material->getShader()->getProgram();
	}
private:
	MeshFilter* mesh;
	Material* material;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void getModelMatrixFromGameObject();
	unsigned int VBO = 0;//顶点缓冲区对象
	unsigned int EBO= 0;//索引缓冲区对象
	unsigned int VAO= 0;//顶点数组对象
};