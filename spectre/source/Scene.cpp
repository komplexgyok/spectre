#include "Scene.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "components/NameComponent.h"
#include "components/TransformComponent.h"
#include "components/MeshComponent.h"
#include "components/MeshRendererComponent.h"
#include "ResourceManager.h"

namespace Spectre
{
	Scene::Scene()
	{}
	
	Scene::~Scene()
	{}

	void Scene::createEntity()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Entity");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void Scene::create3DObject()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "3D Object");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f));
		m_Entities.emplace<MeshComponent>(entity, "assets/meshes/cube.obj");
		m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("mesh"));
	}

	void Scene::createLight()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Light");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		m_Entities.emplace<MeshComponent>(entity, "assets/meshes/cube.obj");
		m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("light"));

		ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.color", glm::vec3(1.0f, 1.0f, 1.0f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	}
}
