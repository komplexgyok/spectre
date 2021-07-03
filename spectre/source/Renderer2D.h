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
	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		void drawQuad(float xPosition, float yPosition, float width, float height, const std::string& textureName);

	private:
		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int indexBuffer;

		//OrthographicCamera* m_Camera;
	};
}
