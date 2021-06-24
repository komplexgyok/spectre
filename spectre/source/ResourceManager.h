#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Texture.h"

namespace Spectre
{
	struct ImageData
	{
		int width;
		int height;
		int channels;
		unsigned char* data;
	};

	class ResourceManager
	{
	public:
		static void addShader(const std::string& name, const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
		static std::shared_ptr<Shader> getShader(const std::string& name);

		static void addTexture(const std::string& name, const std::string& textureFile);
		static std::shared_ptr<Texture> getTexture(const std::string& name);

	private:
		static std::string readFile(const std::string& filepath);
		static ImageData loadImage(const std::string& filepath);

		static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
		static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	};
}
