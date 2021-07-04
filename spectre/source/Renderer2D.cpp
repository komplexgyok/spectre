#include "Renderer2D.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

namespace Spectre
{
	Renderer2D::Renderer2D()
	{
		m_VertexDataBase = new QuadVertex[MAX_VERTEX_COUNT];
		m_IndexData = new uint32_t[MAX_INDEX_COUNT];

		uint32_t offset = 0;
		
		for (uint32_t i = 0; i < MAX_INDEX_COUNT; i += 6) {
			m_IndexData[i + 0] = offset + 0;
			m_IndexData[i + 1] = offset + 1;
			m_IndexData[i + 2] = offset + 2;
			m_IndexData[i + 3] = offset + 2;
			m_IndexData[i + 4] = offset + 3;
			m_IndexData[i + 5] = offset + 0;

			offset += 4;
		}
		
		// Vertex array
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex buffer
		glCreateBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), (void*)offsetof(QuadVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), (void*)offsetof(QuadVertex, color));

		// Index buffer
		glCreateBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDEX_COUNT * sizeof(uint32_t), m_IndexData, GL_STATIC_DRAW);

		ResourceManager::getShader("flat-color")->use();
		ResourceManager::getShader("flat-color")->setUniformMat4("uModel", glm::mat4(1.0f));
	}

	Renderer2D::~Renderer2D()
	{
		glDeleteBuffers(1, &m_IndexBuffer);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteVertexArrays(1, &m_VertexArray);

		delete[] m_IndexData;
		delete[] m_VertexDataBase;
	}

	void Renderer2D::beginScene()
	{
		m_VertexDataPtr = m_VertexDataBase;
	}

	void Renderer2D::endScene()
	{
		flush();
	}

	void Renderer2D::startBatch()
	{
		m_IndexCount = 0;
		m_VertexDataPtr = m_VertexDataBase;
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

	void Renderer2D::flush()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)m_VertexDataPtr - (uint8_t*)m_VertexDataBase);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, m_VertexDataBase);
		
		ResourceManager::getShader("flat-color")->use();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);

		m_DrawCallCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (m_IndexCount >= MAX_INDEX_COUNT) {
			nextBatch();
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x + position.x / 2.0f, position.y + position.y / 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));

		// Vertex data
		m_VertexDataPtr->position = model * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;

		m_VertexDataPtr++;

		m_QuadCount++;
		m_VertexCount += 4;
		m_IndexCount += 6;
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
		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex buffer
		glCreateBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

		// Index buffer
		glCreateBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		ResourceManager::getShader("basic")->use();
		ResourceManager::getTexture(textureName)->bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}
