#include "Material.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "utils/Application.h"
#include <iostream>
void Material::Parse(std::string materialPath)
{
	rapidxml::file<> xmlFile((Application::dataPath()+materialPath).c_str());
	rapidxml::xml_document<> document;
	document.parse<0>(xmlFile.data());

	rapidxml::xml_node<>* materialNode = document.first_node("material");
	if (materialNode == nullptr)
	{
		std::cout << "can't load the xml node!" << std::endl;
		assert(false);
	}

	rapidxml::xml_attribute<>* materialShaderAttribute = materialNode->first_attribute("shader");
	if (materialShaderAttribute == nullptr)
	{
		std::cout << "can't load the material attribute!" << std::endl;
		assert(false);
	}

	shader = Shader::find(materialShaderAttribute->value());

	this->shader = Shader::find(materialShaderAttribute->value());

	rapidxml::xml_node<>* materialTextureNode = materialNode->first_node("texture");
	while (materialTextureNode != nullptr)
	{
		rapidxml::xml_attribute<>* textureNameAttribute = materialTextureNode->first_attribute("name");
		if (textureNameAttribute == nullptr)
		{
			std::cout << "can't load the texture name!" << std::endl;
			assert(false);
		}
		rapidxml::xml_attribute<>* textureImageAttribute = materialTextureNode->first_attribute("image");
		if (textureImageAttribute == nullptr)
		{
			std::cout << "can't load the texture image!" << std::endl;
			assert(false);
		}
		std::string imagePath = textureImageAttribute->value();
		textures.push_back(std::make_pair(textureNameAttribute->value(), Texture2D::loadFromFile(imagePath)));
		materialTextureNode = materialTextureNode->next_sibling("texture");
	}
}
