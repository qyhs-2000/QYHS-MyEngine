#include "MeshFilter.h"
#include <fstream>
#include <iostream>
#include <rttr/registration.h>
RTTR_REGISTRATION
{
	rttr::registration::class_<MeshFilter>("MeshFilter")
				.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void MeshFilter::loadMesh(const std::string& meshPath)
{
	std::ifstream input_file_path(meshPath, std::ios::in | std::ios::binary);
	MeshFileHeader *meshHeader=new MeshFileHeader();
	std::cout << meshHeader->vertexNum<< std::endl;
	input_file_path.read((char*)meshHeader, sizeof(meshHeader));
	std::cout << meshHeader->vertexNum << std::endl;

	char* vertexData = (char*)malloc(meshHeader->vertexNum * sizeof(Vertex));
	unsigned short*indexData = (unsigned short*)malloc(meshHeader->indexNum * sizeof(unsigned short));

	input_file_path.read(vertexData, meshHeader->vertexNum * sizeof(Vertex));
	input_file_path.read((char*)indexData, meshHeader->indexNum * sizeof(unsigned short));
	input_file_path.close();

	this->vertexNum = meshHeader->vertexNum;
	this->indexNum = meshHeader->indexNum;
	this->vertexData = (Vertex*)vertexData;
	this->vertexIndexData = indexData;

    
}
