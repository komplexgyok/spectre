#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Spectre
{
	struct TransformComponent
	{
		glm::vec3 position;

		TransformComponent(const glm::vec3& position)
			: position(position)
		{}
	};
}
