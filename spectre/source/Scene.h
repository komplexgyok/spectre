#pragma once

#include <entt.hpp>

#include "OrthographicCamera.h"

namespace Spectre
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		uint32_t createEntity();
		uint32_t create3DObject();
		uint32_t createLight();
		uint32_t createCamera();

		entt::registry& getEntities() { return m_Entities; }

	private:
		entt::registry m_Entities;
		OrthographicCamera camera;
	};
}
