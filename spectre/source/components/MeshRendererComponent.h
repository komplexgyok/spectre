#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Shader.h"

namespace Spectre
{
	struct MeshRendererComponent
	{
		std::shared_ptr<Shader> shader;
		glm::vec3 materialDiffuse;
		glm::vec3 materialSpecular;

		MeshRendererComponent(std::shared_ptr<Shader> shader)
			: shader(shader), materialDiffuse(glm::vec3(0.5f, 0.5f, 0.5f)), materialSpecular(glm::vec3(0.5f, 0.5f, 0.5f))
		{}

		MeshRendererComponent(std::shared_ptr<Shader> shader, const glm::vec3& materialDiffuse, const glm::vec3& materialSpecular)
			: shader(shader), materialDiffuse(materialDiffuse), materialSpecular(materialSpecular)
		{}
	};
}
