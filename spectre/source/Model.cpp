#include "Model.h"

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

namespace Spectre
{
	Model::Model(const std::string& filepath)
	{
		loadModel(filepath);
	}

	void Model::loadModel(const std::string& filepath)
	{
		Assimp::Importer importer;
		// Read the model file and triangulate the vertices
		const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate);

		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
			std::cout << "Assimp model loading error: " << importer.GetErrorString() << std::endl;
		}

		m_Directory = filepath.substr(0, filepath.find_last_of('/'));

		// Process the root node recursively
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		static uint32_t nodeId = -1;
		nodeId++;
		std::cout << "Node #" << nodeId << ":" << std::endl;

		// Process all the node's meshes
		for (uint32_t i = 0; i < node->mNumMeshes; i++) {
			std::cout << "\tMesh #" << i << ":" << std::endl;

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<TextureM> textures;

		// Process vertices
		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			// Position
			glm::vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			vertex.position = position;

			// Texture coordinate
			if (mesh->mTextureCoords[0]) {
				glm::vec2 textureCoordinate;
				textureCoordinate.x = mesh->mTextureCoords[0][i].x;
				textureCoordinate.y = mesh->mTextureCoords[0][i].y;
				vertex.textureCoordinate = textureCoordinate;
			}
			else {
				vertex.textureCoordinate = glm::vec2(0.0f, 0.0f);
			}

			// Normal
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.normal = normal;

			std::cout << "\t\tVertex #" << i << ": " <<
			"Position: (" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z <<
			"), Texture Coordinate: (" << vertex.textureCoordinate.x << ", " << vertex.textureCoordinate.y <<
			"), Normal: (" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << ")" << std::endl;
			vertices.push_back(vertex);
		}
		std::cout << std::endl;

		// Process indices
		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
			std::cout << "\t\tFace #" << i << " indices: ";
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
				std::cout << face.mIndices[j] << " ";
			}
			std::cout << std::endl;
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			
			std::vector<TextureM> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<TextureM> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return std::make_shared<Mesh>(vertices, indices/*, textures*/);
	}

	std::vector<TextureM> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<TextureM> textures;

		uint32_t c = material->GetTextureCount(type);
		std::cout << "Texture count: " << c << std::endl;

		for (uint32_t i = 0; i <= material->GetTextureCount(type); i++) {
			aiString string;
			material->GetTexture(type, i, &string);
			std::cout << string.C_Str() << std::endl;

			bool skip = false;
			for (uint32_t j = 0; j < m_LoadedTextures.size(); j++) {
				if (std::strcmp(m_LoadedTextures[j].path.data(), string.C_Str()) == 0) {
					textures.push_back(m_LoadedTextures[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				TextureM texture;
				texture.id = textureFromFile(string.C_Str(), m_Directory);
				texture.type = typeName;
				texture.path = string.C_Str();

				textures.push_back(texture);
				m_LoadedTextures.push_back(texture);
			}
		}

		return textures;
	}

	uint32_t Model::textureFromFile(const char* path, const std::string& directory)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		uint32_t id;
		glGenTextures(1, &id);

		int width, height, channels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
		
		if (data) {
			GLenum format;
			
			if (channels == 1) {
				format = GL_RED;
			}
			else if (channels == 3) {
				format = GL_RGB;
			}
			else if (channels == 4) {
				format = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else {
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return id;
	}
}
