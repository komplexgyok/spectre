#include "Scene.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "components/CameraComponent.h"
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

	uint32_t Scene::createEntity()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Entity");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f));

		return static_cast<uint32_t>(entity);
	}

	uint32_t Scene::create3DObject()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "3D Object");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f));
		m_Entities.emplace<MeshComponent>(entity, "assets/meshes/crate.obj");
		m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("mesh"));

		ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformInt("u_Material.diffuse", 0);
		ResourceManager::getShader("mesh")->setUniformInt("u_Material.specular", 1);
		ResourceManager::getShader("mesh")->setUniformInt("u_Material.emission", 2);

		return static_cast<uint32_t>(entity);
	}

	uint32_t Scene::createLight()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Light");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		m_Entities.emplace<MeshComponent>(entity, "assets/meshes/cube.obj");
		m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("light"));

		ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.color", glm::vec3(1.0f, 1.0f, 1.0f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		ResourceManager::getShader("mesh")->setUniformFloat("u_Light.constant", 1.0f);
		ResourceManager::getShader("mesh")->setUniformFloat("u_Light.linear", 0.09f);
		ResourceManager::getShader("mesh")->setUniformFloat("u_Light.quadratic", 0.032f);

		return static_cast<uint32_t>(entity);
	}

	uint32_t Scene::createCamera()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Main Camera");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		m_Entities.emplace<CameraComponent>(entity, glm::vec3(0.0f, 0.0f, 6.0f), glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f, true);
		
		return static_cast<uint32_t>(entity);
	}
}
