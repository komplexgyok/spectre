#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Spectre
{
	struct SpriteRendererComponent
	{
		glm::vec4 color;

		SpriteRendererComponent(const glm::vec4& color)
			: color(color)
		{}
	};
}
