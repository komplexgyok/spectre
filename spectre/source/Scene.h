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

		void addEntity();

		entt::registry& getEntities() { return m_Entities; }

	private:
		entt::registry m_Entities;
		OrthographicCamera camera;
	};
}
