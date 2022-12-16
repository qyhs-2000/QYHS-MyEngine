#pragma once 
#include <string>
#include "../Component/Component.h"
#include <unordered_map>
class Component;
class GameObject
{
public:
	GameObject() {};
	Component *addComponent(std::string component_type_name);
	Component * getComponent(std::string component_type_name);
private:
	std::unordered_map<std::string, std::vector<Component*>> componentMap;
};