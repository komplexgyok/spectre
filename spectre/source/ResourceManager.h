#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Shader.h"

namespace Spectre
{
	class ResourceManager
	{
	public:
		static void addShader(const std::string& name, const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
		static std::shared_ptr<Shader> getShader(const std::string& name);

	private:
		static std::string readFile(const std::string& filepath);

		static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
	};
}
