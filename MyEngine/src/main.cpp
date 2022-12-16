#include <iostream>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform2.hpp>
#include<glm/gtx/euler_angles.hpp>
#include "utils/GameObject.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "renderer/Texture2D.h"
#include "VertexData.h"
#include "renderer/MeshFilter.h"
#include "renderer/Shader.h"
#include "renderer/Material.h"
#include "renderer/MeshRenderer.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
using namespace std;

struct Foo
{
};

GLFWwindow* window;
GLuint program, vertexShader, fragmentShader;
GLuint VBO, EBO, VAO;
GLint mvp_location, vpos_location, vcol_location, vuv_location;
void errorCallback(int error_code, const char* description)
{
	std::cout << "Error occur!"<<description << std::endl;
}

void initOpenGL()
{

	glfwSetErrorCallback(errorCallback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(960, 640, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	glfwSwapInterval(1);
}




void createBufferObject(MeshFilter* mesh)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->getVertexNum() * sizeof(MeshFilter::Vertex), mesh->getVertexData(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexNum() * sizeof(unsigned short), mesh->getVertexIndexData(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(MeshFilter::Vertex), 0);
		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(vuv_location);
		glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * 7));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



int main()
{
	initOpenGL();
	Texture2D::compressImageFile("E:\\GoogleDownload\\mesh\\textures\\Diffuse_FishSoup_Pot_1.jpg", "./Images/diffuse_fishsoup_pot.qyhs");
	//Texture2D::compressImageFile("D:\\Image\\lifuWallpaper.jpg", "./Images/diffuse_fishsoup_pot.qyhs");
	removeDulplicateVertex();
	std::string shaderName = "./shader/Unlit";

	//创建material，其中的mat文件以及记载了shader文件、image文件的路径
	Material* material = new Material();
	material->Parse("./material/fishsoup_pot.mat");
	//meshRenderer.setMesh(&mesh);
	GameObject* gameObject = new GameObject();
	auto transform =dynamic_cast<Transform*>(gameObject->addComponent("Transform"));
	transform->set_position(glm::vec3(2, 0, 0));
	auto meshFilter = dynamic_cast<MeshFilter*>(gameObject->addComponent("MeshFilter"));
	meshFilter->loadMesh("./MeshFile/fishsoup_pot.mesh");
	auto meshRenderer = dynamic_cast<MeshRenderer*>(gameObject->addComponent("MeshRenderer"));
	meshRenderer->setMaterial(material);
	auto cameraComp = dynamic_cast<Camera*>(gameObject->addComponent("Camera"));


	
	
	glm::mat4 model, view, projection, mvp;
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);

		glm::mat4 trans = glm::translate(glm::vec3(0, 0, 0));
		static float rotate_eulerAngle = 0.f;
		//glm::mat4 rotation = glm::eulerAngleYXZ(glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle)); //ʹ��ŷ������ת;
		rotate_eulerAngle += 1.1f;
		glm::vec3 rotation = transform->rotation();
		rotation.y = rotate_eulerAngle;
		transform->set_rotation(rotation);
		glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));

		view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		cameraComp->setViewMatrix(view);
		projection = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);
		cameraComp->setProjectMatrix(projection);
		Camera::forEach([&](){
			meshRenderer->Renderer();
		});
		/*meshRenderer->setView(cameraComp->getViewMatrix());
		meshRenderer->setProjection(cameraComp->getProjectMatrix());
		meshRenderer->Renderer();*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);


	
	system("pause");
	return 0;
}