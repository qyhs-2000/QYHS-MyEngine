#pragma once
#include <string>

class GameObject;
class Component
{
public:
	virtual void set_game_object(GameObject* gameObject);
	virtual GameObject* gameObject() { return this->_gameObject; }
private:
	GameObject* _gameObject;
};