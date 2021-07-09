#pragma once

#include <glm/glm.hpp>

namespace Spectre
{
	class Input
	{
	public:
		static bool isKeyPressed(int key);
		static bool isMouseButtonPressed(int mouse);

		static glm::vec2 getMousePosition();
	};
}
