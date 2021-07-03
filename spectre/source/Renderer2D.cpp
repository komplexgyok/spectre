#include "Renderer2D.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

namespace Spectre
{
	Renderer2D::Renderer2D()
	{
		//m_Camera = new OrthographicCamera(0.0f, 1280.0f, 0.0f, 720.0f);
	}

	Renderer2D::~Renderer2D()
	{
		//delete m_Camera;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		// Vertex data
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		// Index data
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Vertex array
		glCreateVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		// Vertex buffer
		glCreateBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// Index buffer
		glCreateBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x + position.x / 2.0f, position.y + position.y / 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));

		ResourceManager::getShader("flat-color")->use();
		ResourceManager::getShader("flat-color")->setUniformMat4("uModel", model);
		ResourceManager::getShader("flat-color")->setUniformVec4("uColor", color);
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteVertexArrays(1, &vertexArray);
	}

	void Renderer2D::drawQuad(float xPosition, float yPosition, float width, float height, const std::string& textureName)
	{
		// Vertex data
		float vertices[] = {
			// Position                                  // Color                // Texture coordinates
			xPosition, yPosition, 0.0f,                  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			xPosition + width, yPosition, 0.0f,          0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			xPosition + width, yPosition + height, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			xPosition, yPosition + height, 0.0f,         1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};

		// Index data
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Vertex array
		glCreateVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		// Vertex buffer
		glCreateBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

		// Index buffer
		glCreateBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		ResourceManager::getShader("basic")->use();
		ResourceManager::getTexture(textureName)->bind();
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}
