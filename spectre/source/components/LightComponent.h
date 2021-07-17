#pragma once

#include <memory>

#include "ResourceManager.h"
#include "Shader.h"

namespace Spectre
{
	enum class LightType
	{
		Point
	};
	
	struct LightComponent
	{
		std::shared_ptr<Shader> shader;
		LightType type;

		LightComponent()
			: type(LightType::Point)
		{
			shader = ResourceManager::getShader("light");
		}
	};
}
