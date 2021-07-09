#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Spectre
{
	struct TransformComponent
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		TransformComponent(const glm::vec3& position)
			: position(position)
		{
			rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			scale = glm::vec3(1.0f, 1.0f, 1.0f);
		}
	};
}
