#include "Scene.h"

#include <iostream>

#include "components/NameComponent.h"

namespace Spectre
{
	Scene::Scene()
	{
		struct TransformComponent
		{
			glm::mat4 transform;

			TransformComponent() = default;
			TransformComponent(const glm::mat4& transform)
				: transform(transform)
			{}
		};

		//entt::entity entity = m_Entities.create();
		//m_Entities.emplace<TransformComponent>(entity);
	}
	
	Scene::~Scene()
	{}

	void Scene::listEntities()
	{
		m_Entities.each([](auto entity) {
			std::cout << (uint32_t)entity << std::endl;
		});
	}

	void Scene::addEntity()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Entity");
	}
}
