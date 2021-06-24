#include "ResourceManager.h"

#include <fstream>
#include <sstream>

namespace Spectre
{
	std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::shaders;

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
	}

	std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name)
	{
		if (shaders.find(name) != shaders.end()) {
			return shaders[name];
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
}
