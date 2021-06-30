#pragma once

#include <glm/glm.hpp>

namespace Spectre
{
	class Renderer3D
	{
	public:
		Renderer3D();
		~Renderer3D();

		void drawCube(const glm::vec3& position);
		void drawCube(const glm::vec3& position, int index);

	private:
		unsigned int vertexArray;
		unsigned int vertexBuffer;
	};
}
