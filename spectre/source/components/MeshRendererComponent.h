#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Shader.h"

namespace Spectre
{
	struct MeshRendererComponent
	{
		std::shared_ptr<Shader> shader;
		glm::vec3 materialAmbient;
		glm::vec3 materialDiffuse;
		glm::vec3 materialSpecular;
		float materialShininess;

		MeshRendererComponent(std::shared_ptr<Shader> shader)
			: shader(shader)
			, materialAmbient(glm::vec3(0.0f, 0.1f, 0.06f)), materialDiffuse(glm::vec3(0.0f, 0.50980392f, 0.50980392f)), materialSpecular(glm::vec3(0.50196078f, 0.50196078f, 0.50196078f))
			, materialShininess(32.0f)
		{}
	};
}
