#pragma once

#include <memory>
#include <string>

#include "Texture.h"

namespace Spectre
{
	class Renderer2D
	{
	public:
		void drawQuad(float xPosition, float yPosition, float width, float height, const std::string& textureName);

	private:
		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int indexBuffer;
	};
}
