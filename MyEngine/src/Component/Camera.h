#pragma once
#include "Component/Component.h"
#include "glm/glm.hpp"
#include <functional>
#include <glfw/glfw3.h>
#include <vector>
class Camera :public Component
{
public:
	Camera();
	~Camera();
	inline glm::mat4 getViewMatrix() { return this->_viewMatrix; }
	inline void setViewMatrix(glm::mat4 matrix) { this->_viewMatrix = matrix; }
	inline void setProjectMatrix(glm::mat4 matrix) { this->_projectMatrix = matrix; }
	inline glm::mat4 getProjectMatrix() { return this->_projectMatrix; }
	void clear();
	void set_clear_flag(unsigned int flag);
	static Camera* currentCamera() { return _currentCamera; }
	static void forEach(std::function<void()> func);
private:
	glm::mat4 _viewMatrix;
	glm::mat4 _projectMatrix;
	glm::vec4 _clear_color;
	unsigned int _clear_flag;
	static std::vector<Camera*> _all_camera;
	static Camera* _currentCamera;
};