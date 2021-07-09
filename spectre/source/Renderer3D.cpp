#include "Renderer3D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

namespace Spectre
{
	Renderer3D::Renderer3D()
	{
		//// Load shaders
		//Spectre::ResourceManager::addShader("basic", "../spectre/resources/shaders/basic.vert", "../spectre/resources/shaders/basic.frag");
		//Spectre::ResourceManager::addShader("3d", "../spectre/resources/shaders/3d.vert", "../spectre/resources/shaders/3d.frag");

		//// Load textures
		//Spectre::ResourceManager::addTexture("grass-top", "../spectre/resources/textures/grass-top.png");
		//Spectre::ResourceManager::addTexture("grass-side", "../spectre/resources/textures/grass-side.png");
		//Spectre::ResourceManager::addTexture("grass-bottom", "../spectre/resources/textures/grass-bottom.png");

		//ResourceManager::getShader("3d")->use();
		//ResourceManager::getShader("3d")->setUniformInt("uTexture1", 0);
		//ResourceManager::getShader("3d")->setUniformInt("uTexture2", 1);
		//ResourceManager::getShader("3d")->setUniformInt("uTexture3", 2);

		//glBindTextureUnit(0, Spectre::ResourceManager::getTexture("grass-top")->getId());
		//glBindTextureUnit(1, Spectre::ResourceManager::getTexture("grass-side")->getId());
		//glBindTextureUnit(2, Spectre::ResourceManager::getTexture("grass-bottom")->getId());
		//glBindVertexArray(vertexArray);

		//glm::mat4 projection = glm::mat4(1.0f);
		//projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		//Spectre::ResourceManager::getShader("3d")->setUniformMat4("uProjection", projection);
	}

	Renderer3D::~Renderer3D()
	{}

	void Renderer3D::drawCube(const glm::vec3& position)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);

		glBindVertexArray(vertexArray);
		ResourceManager::getShader("3d")->use();
		ResourceManager::getShader("3d")->setUniformMat4("uModel", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void Renderer3D::drawCube(const glm::vec3& position, int index)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * (index + 1)), glm::vec3(0.5f, 0.3f, 0.5f));

		glBindVertexArray(vertexArray);
		ResourceManager::getShader("3d")->use();
		ResourceManager::getShader("3d")->setUniformMat4("uModel", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void Renderer3D::renderMesh(const TransformComponent& transform, const Mesh& mesh, std::shared_ptr<Shader> shader)
	{
		shader->use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.position);
		model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, transform.scale);
		shader->setUniformMat4("u_Model", model);
		
		glBindVertexArray(mesh.getVertexArray());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
		glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}
