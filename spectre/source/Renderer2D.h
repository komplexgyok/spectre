#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OrthographicCamera.h"
#include "ResourceManager.h"
#include "Texture.h"

namespace Spectre
{
	struct QuadVertex
	{
		glm::vec4 position;
		glm::vec4 color;
		glm::vec2 textureCoordinate;
	};

	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

		void beginScene();
		void endScene();
		void startBatch();
		void nextBatch();
		void flush();

		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		void drawQuad(const glm::vec2& position, const glm::vec2& size, const std::string& textureName);

		void resetStatistics()
		{
			m_DrawCallCount = 0;
			m_QuadCount = 0;
			m_VertexCount = 0;
		}

		int getDrawCallCount() { return m_DrawCallCount; }
		int getQuadCount() { return m_QuadCount; }
		int getVertexCount() { return m_VertexCount; }

	private:
		const uint32_t MAX_QUAD_COUNT = 25000;
		const uint32_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
		const uint32_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;

		QuadVertex* m_VertexDataBase = nullptr;
		QuadVertex* m_VertexDataPtr = nullptr;

		uint32_t* m_IndexData = nullptr;

		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;

		int m_DrawCallCount = 0;
		int m_QuadCount = 0;
		int m_VertexCount = 0;

		int m_IndexCount = 0;
	};
}
