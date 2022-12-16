#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <rttr/registration.h>

RTTR_REGISTRATION
{
	rttr::registration::class_<Camera>("Camera")
	.constructor<>()(rttr::policy::ctor::as_raw_ptr)
	.property("viewMatrix",&Camera::getViewMatrix,&Camera::setViewMatrix)
	.property("projectMatrix" ,&Camera::getProjectMatrix,&Camera::setProjectMatrix);
}

std::vector<Camera*> Camera::_all_camera;
Camera* Camera::_currentCamera;

Camera::Camera()
	:_clear_color(49.f / 255, 77.f / 255, 121.f / 255, 1.f), _clear_flag(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
{
	_all_camera.push_back(this);
}

Camera::~Camera()
	
{

}

void Camera::clear()
{
	glClear(_clear_flag);
}

void Camera::forEach(std::function<void()> func)
{
	for (auto iter = _all_camera.begin(); iter != _all_camera.end(); ++iter)
	{
		_currentCamera = *iter;
		_currentCamera->clear();
		func();
	}
}
