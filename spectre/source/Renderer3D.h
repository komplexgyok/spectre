#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"

namespace Spectre
{
	class Renderer3D
	{
	public:
		Renderer3D();
		~Renderer3D();

		void drawCube(const glm::vec3& position);
		void drawCube(const glm::vec3& position, int index);

		void renderMesh(const glm::vec3& transform, const Mesh& mesh, std::shared_ptr<Shader> shader);

	private:
		uint32_t vertexArray;
		uint32_t vertexBuffer;
	};
}
