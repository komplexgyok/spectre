#pragma once

#include <string>

namespace Spectre
{
	class Shader
	{
	public:
		Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		~Shader();

		void use();

	private:
		unsigned int compile(unsigned int shaderType, const std::string& shaderSource);
		unsigned int link(unsigned int vertexShader, unsigned int fragmentShader);

		unsigned int id;
	};
}
