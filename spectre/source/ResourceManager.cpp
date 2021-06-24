#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <stb_image.h>

namespace Spectre
{
	std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::shaders;
	std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::textures;

	void ResourceManager::addShader(const std::string& name, const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	{
		// If there is already a shader with this name, do nothing
		if (shaders.find(name) != shaders.end()) {
			return;
		}

		// Read the shader files into strings
		std::string vertexShaderSource   = readFile(vertexShaderFile);
		std::string fragmentShaderSource = readFile(fragmentShaderFile);

		// Store the new shader
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderSource, fragmentShaderSource);
		shaders[name] = shader;

		std::cout << "Shader added: " << name << std::endl;
	}

	std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name)
	{
		if (shaders.find(name) != shaders.end()) {
			return shaders[name];
		}

		return nullptr;
	}

	void ResourceManager::addTexture(const std::string& name, const std::string& textureFile)
	{
		// If there is already a texture with this name, do nothing
		if (textures.find(name) != textures.end()) {
			return;
		}

		ImageData imageData = loadImage(textureFile);

		// Store the new texture
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(imageData.width, imageData.height, imageData.data);
		textures[name] = texture;

		std::cout << "Texture added: " << name << std::endl;
	}

	std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& name)
	{
		if (textures.find(name) != textures.end()) {
			return textures[name];
		}

		return nullptr;
	}

	std::string ResourceManager::readFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::in | std::ios::binary);
		std::stringstream stream;

		stream << file.rdbuf();

		return stream.str();
	}

	ImageData ResourceManager::loadImage(const std::string& filepath)
	{
		ImageData imageData;

		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = stbi_load(filepath.c_str(), &imageData.width, &imageData.height, &imageData.channels, 0);

		if (!data) {
			std::cout << "Failed to load texture" << std::endl;
		}

		imageData.data = data;

		return imageData;
	}
}
