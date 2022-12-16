#include "MeshRenderer.h"
#include "Component/Transform.h"
#include "utils/GameObject.h"
#include <rttr/registration.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform2.hpp>
#include<glm/gtx/euler_angles.hpp>
#include "Component/Camera.h"
RTTR_REGISTRATION
{
	rttr::registration::class_<MeshRenderer>("MeshRenderer")
	.constructor<>()(rttr::policy::ctor::as_raw_ptr);
};

void MeshRenderer::setMaterial(Material* material)
{
	this->material = material;
}

void MeshRenderer::setMesh(MeshFilter* mesh)
{
	this->mesh = mesh;
}

void MeshRenderer::setView(glm::mat4 view)
{
	this->view = view;
}

void MeshRenderer::setProjection(glm::mat4 projection)
{
	this->projection = projection;
}

void MeshRenderer::getModelMatrixFromGameObject()
{

}

void MeshRenderer::Renderer()
{
	auto transform = dynamic_cast<Transform*>(gameObject()->getComponent("Transform"));
	if (!transform) 
	{
		return;
	}
	Camera* camera = Camera::currentCamera();
	if (camera == nullptr)
	{
		return;
	}
	this->setView(camera->getViewMatrix());
	this->setProjection(camera->getProjectMatrix());
	glm::mat4 trans = glm::translate(transform->position());
	auto rotation = transform->rotation();
	glm::mat4 eulerAngleYXZ = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
	glm::mat4 scale = glm::scale(transform->scale()); //缩放;
	glm::mat4 model = trans * scale * eulerAngleYXZ;
	glm::mat4 mvp = projection * view * model;
	GLuint programId = this->material->getShader()->getProgram();
	
	this->mesh = dynamic_cast<MeshFilter*>(gameObject()->getComponent("MeshFilter"));

	if (VAO == 0)
	{
		GLint vpos_location = glGetAttribLocation(programId, "a_pos");
		GLint vcol_location = glGetAttribLocation(programId, "a_color");
		GLint vuv_location = glGetAttribLocation(programId, "a_uv");

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->getVertexNum() * sizeof(MeshFilter::Vertex), mesh->getVertexData(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexNum() * sizeof(unsigned short), mesh->getVertexIndexData(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		{
			//指定当前使用的VBO
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//将Shader变量(a_pos)和顶点坐标VBO句柄进行关联，最后的0表示数据偏移量。
			glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(MeshFilter::Vertex), 0);
			//启用顶点Shader属性(a_color)，指定与顶点颜色数据进行关联
			glVertexAttribPointer(vcol_location, 4, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * 3));
			//将Shader变量(a_uv)和顶点UV坐标VBO句柄进行关联，最后的0表示数据偏移量。
			glVertexAttribPointer(vuv_location, 2, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * (3 + 4)));

			glEnableVertexAttribArray(vpos_location);
			glEnableVertexAttribArray(vcol_location);
			glEnableVertexAttribArray(vuv_location);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glUseProgram(programId);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUniformMatrix4fv(glGetUniformLocation(programId, "u_mvp"), 1, GL_FALSE, &mvp[0][0]);
	std::vector<std::pair<std::string, Texture2D*>> textures = material->getTextures();
	for (unsigned int index = 0; index < textures.size(); index++)
	{
		GLint u_diffuse_texture_location = glGetUniformLocation(programId, textures[index].first.c_str());
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textures[index].second->getTextureID());
		glUniform1i(u_diffuse_texture_location, index);

	}

	glBindVertexArray(VAO);
	{
		glDrawElements(GL_TRIANGLES, mesh->getIndexNum(), GL_UNSIGNED_SHORT, 0);
	}
	glBindVertexArray(0);
}

