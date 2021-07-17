#pragma once

#include <memory>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace Spectre
{
	class Model
	{
	public:
		Model(const std::string& filepath);

		std::vector<std::shared_ptr<Mesh>> getMeshes() const { return m_Meshes; }

	private:
		void loadModel(const std::string& filepath);
		void processNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<TextureM> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
		uint32_t textureFromFile(const char* path, const std::string& directory);

		std::vector<TextureM> m_LoadedTextures;
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::string m_Directory;
	};
}
