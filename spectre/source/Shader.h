#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Spectre
{
	class Shader
	{
	public:
		Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		~Shader();

		void use();

		inline unsigned int getId() const { return id; }

		void setUniformInt(const std::string& name, int value);
		void setUniformIntArray(const std::string& name, uint32_t count, int* values);
		void setUniformVec3(const std::string& name, const glm::vec3& vector);
		void setUniformVec4(const std::string& name, const glm::vec4& vector);
		void setUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		unsigned int compile(unsigned int shaderType, const std::string& shaderSource);
		unsigned int link(unsigned int vertexShader, unsigned int fragmentShader);

		int getUniformLocation(const std::string& name);

		unsigned int id;
	};
}
