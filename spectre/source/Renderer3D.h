#pragma once

#include <memory>

#include "components/TransformComponent.h"
#include "Mesh.h"
#include "Shader.h"

namespace Spectre
{
	class Renderer3D
	{
	public:
		Renderer3D() = default;
		~Renderer3D() = default;

		void renderMesh(const TransformComponent& transform, const Mesh& mesh, std::shared_ptr<Shader> shader);
	};
}
