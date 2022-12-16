#pragma once
#include <glm/glm.hpp>
#include <string>
#include <Component/Component.h>
class MeshFilter:public Component
{
public:
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 uv;
	};
	class MeshFileHeader
	{
	public:
		MeshFileHeader():vertexNum(0),indexNum(0){}
		char type[4];
		unsigned short vertexNum;
		unsigned short indexNum;

	};

	unsigned int getVertexNum()
	{
		return this->vertexNum;
	}
	unsigned int getIndexNum()
	{
		return this->indexNum;
	}
	Vertex* getVertexData()
	{
		return this->vertexData;
	}

	unsigned short* getVertexIndexData()
	{
		return vertexIndexData;
	}
	void loadMesh(const std::string& meshPath);
private:
	unsigned short vertexNum;
	unsigned short indexNum;
	Vertex* vertexData;
	unsigned short* vertexIndexData;

};