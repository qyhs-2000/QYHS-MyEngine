#include "GameObject.h"
#include <rttr/registration.h>
Component* GameObject::addComponent(std::string component_type_name)
{
	rttr::type t = rttr::type::get_by_name(component_type_name);
	rttr::variant var = t.create();
	Component* component = var.get_value<Component*>();
	component->set_game_object(this);
	this->componentMap[component_type_name].push_back(component);

	return component;
}

Component* GameObject::getComponent(std::string component_type_name)
{
	if (componentMap.find(component_type_name) == componentMap.end()) {
		return nullptr;
	}
	if (componentMap[component_type_name].size() == 0)
	{
		return nullptr;
	}
	return this->componentMap[component_type_name][0];
}
