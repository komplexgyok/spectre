#include "Scene.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "components/CameraComponent.h"
#include "components/LightComponent.h"
#include "components/MeshComponent.h"
#include "components/MeshRendererComponent.h"
#include "components/NameComponent.h"
#include "components/TransformComponent.h"

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
		//m_Entities.emplace<MeshComponent>(entity, "assets/models/Survival_BackPack_2.fbx");
		//m_Entities.emplace<MeshComponent>(entity, "assets/models/kettohatvany.fbx");
		m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("model"));

		/*ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformInt("u_Material.diffuse", 0);
		ResourceManager::getShader("mesh")->setUniformInt("u_Material.specular", 1);
		ResourceManager::getShader("mesh")->setUniformInt("u_Material.emission", 2);*/

		return static_cast<uint32_t>(entity);
	}

	uint32_t Scene::createLight()
	{
		entt::entity entity = m_Entities.create();
		m_Entities.emplace<NameComponent>(entity, "Light");
		m_Entities.emplace<TransformComponent>(entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		//m_Entities.emplace<MeshComponent>(entity, "assets/models/crate.obj");
		m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("light"));

		/*ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.color", glm::vec3(1.0f, 1.0f, 1.0f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		ResourceManager::getShader("mesh")->setUniformFloat("u_Light.constant", 1.0f);
		ResourceManager::getShader("mesh")->setUniformFloat("u_Light.linear", 0.09f);
		ResourceManager::getShader("mesh")->setUniformFloat("u_Light.quadratic", 0.032f);*/

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

	void Scene::loadScene(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			std::cout << "Failed to load scene: " << importer.GetErrorString() << std::endl;
		}

		processNode(scene->mRootNode, scene);
	}

	void Scene::processNode(aiNode* node, const aiScene* scene)
	{
		m_Statistics.nodeCounter++;

		// New entity, not a container node
		if (!node->mNumChildren) {
			std::cout << node->mName.C_Str() << " is an entity" << std::endl;
			entt::entity entity = m_Entities.create();
			
			// Name component
			m_Entities.emplace<NameComponent>(entity, node->mName.C_Str());

			// Transform component
			aiVector3D aiPosition, aiRotation, aiScale;
			node->mTransformation.Decompose(aiScale, aiRotation, aiPosition);
			
			glm::vec3 position = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
			glm::vec3 rotation = glm::vec3(aiRotation.x, aiRotation.y, aiRotation.z);
			glm::vec3 scale = glm::vec3(aiScale.x, aiScale.y, aiScale.z);
			m_Entities.emplace<TransformComponent>(entity, position, rotation, scale);
			
			// Process meshes
			for (uint32_t i = 0; i < node->mNumMeshes; i++) {
				m_Statistics.meshCounter++;

				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

				std::vector<Vertex> vertices;
				std::vector<uint32_t> indices;

				// Process vertices
				for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
					m_Statistics.vertexCounter++;

					Vertex vertex;

					// Position
					vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

					// Texture coordinate
					if (mesh->mTextureCoords[0]) {
						vertex.textureCoordinate = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
					}
					else {
						vertex.textureCoordinate = glm::vec2(0.0f, 0.0f);
					}

					// Normal
					vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

					vertices.push_back(vertex);
				}

				// Process indices
				for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
					aiFace face = mesh->mFaces[i];

					for (uint32_t j = 0; j < face.mNumIndices; j++) {
						indices.push_back(face.mIndices[j]);
					}
				}

				// Mesh component
				m_Entities.emplace<MeshComponent>(entity, vertices, indices);

				// Process materials
				if (mesh->mMaterialIndex >= 0) {
					aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
					aiColor3D ambient;
					material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
					aiColor3D diffuse;
					material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
					aiColor3D specular;
					material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
					float specularFactor;
					material->Get(AI_MATKEY_SPECULAR_FACTOR, specularFactor);

					// Mesh renderer component
					m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("model"), glm::vec3(diffuse.r, diffuse.g, diffuse.b), glm::vec3(specular.r, specular.g, specular.b));
				}
				else {
					// Mesh renderer component
					m_Entities.emplace<MeshRendererComponent>(entity, ResourceManager::getShader("model"));
				}
			}

			// TODO: handle multiple lights
			if (scene->mNumLights) {
				if (node->mName == scene->mLights[0]->mName) {
					m_Entities.emplace<LightComponent>(entity);
				}
			}

			// TODO: handle multiple cameras
			if (scene->mNumCameras) {
				if (node->mName == scene->mCameras[0]->mName) {
					std::cout << node->mName.C_Str() << " is a camera" << std::endl;
				}
			}
		}

		// Process other nodes
		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}
}
