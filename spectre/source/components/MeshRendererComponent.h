#pragma once

#include <memory>

#include "Shader.h"

namespace Spectre
{
	struct MeshRendererComponent
	{
		std::shared_ptr<Shader> shader;

		MeshRendererComponent(std::shared_ptr<Shader> shader)
			: shader(shader)
		{}
	};
}
