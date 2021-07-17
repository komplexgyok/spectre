#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <entt.hpp>

#include "OrthographicCamera.h"

namespace Spectre
{
	struct SceneStatistics
	{
		uint32_t nodeCounter = 0;
		uint32_t meshCounter = 0;
		uint32_t vertexCounter = 0;
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		uint32_t createEntity();
		uint32_t create3DObject();
		uint32_t createLight();
		uint32_t createCamera();

		void loadScene(const std::string& path);

		entt::registry& getEntities() { return m_Entities; }

		inline SceneStatistics getStatistics() const { return m_Statistics; }

	private:
		void processNode(aiNode* node, const aiScene* scene);

		entt::registry m_Entities;
		OrthographicCamera camera;

		SceneStatistics m_Statistics;
	};
}
