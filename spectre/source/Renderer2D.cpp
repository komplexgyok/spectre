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
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4) + sizeof(glm::vec2), (void*)offsetof(QuadVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4) + sizeof(glm::vec2), (void*)offsetof(QuadVertex, color));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4) + sizeof(glm::vec2), (void*)offsetof(QuadVertex, textureCoordinate));

		// Index buffer
		glCreateBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDEX_COUNT * sizeof(uint32_t), m_IndexData, GL_STATIC_DRAW);

		ResourceManager::getShader("texture")->use();
		ResourceManager::getShader("texture")->setUniformMat4("uModel", glm::mat4(1.0f));
		ResourceManager::getShader("texture")->setUniformInt("uTexture", 0);
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
		
		ResourceManager::getShader("texture")->use();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);

		m_DrawCallCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (m_IndexCount >= MAX_INDEX_COUNT) {
			nextBatch();
		}

		ResourceManager::getTexture("white-texture")->bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
		model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));

		// Vertex data
		m_VertexDataPtr->position = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;
		m_VertexDataPtr->textureCoordinate = glm::vec2(0.0f, 0.0f);

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;
		m_VertexDataPtr->textureCoordinate = glm::vec2(1.0f, 0.0f);

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;
		m_VertexDataPtr->textureCoordinate = glm::vec2(1.0f, 1.0f);

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = color;
		m_VertexDataPtr->textureCoordinate = glm::vec2(0.0f, 1.0f);

		m_VertexDataPtr++;

		m_QuadCount++;
		m_VertexCount += 4;
		m_IndexCount += 6;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const std::string& textureName)
	{
		if (m_IndexCount >= MAX_INDEX_COUNT) {
			nextBatch();
		}

		ResourceManager::getTexture(textureName)->bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
		model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));

		// Vertex data
		m_VertexDataPtr->position = model * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_VertexDataPtr->textureCoordinate = glm::vec2(0.0f, 0.0f);

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_VertexDataPtr->textureCoordinate = glm::vec2(1.0f, 0.0f);

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_VertexDataPtr->textureCoordinate = glm::vec2(1.0f, 1.0f);

		m_VertexDataPtr++;

		m_VertexDataPtr->position = model * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
		m_VertexDataPtr->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_VertexDataPtr->textureCoordinate = glm::vec2(0.0f, 1.0f);

		m_VertexDataPtr++;

		m_QuadCount++;
		m_VertexCount += 4;
		m_IndexCount += 6;
	}
}
