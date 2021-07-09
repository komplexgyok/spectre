#include "Scene.h"

#include <iostream>

#include <glm/glm.hpp>

#include "components/NameComponent.h"
#include "components/TransformComponent.h"

namespace Spectre
{
	Scene::Scene()
	{}
	
	Scene::~Scene()
	{}

	void Scene::addEntity()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Entity");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
