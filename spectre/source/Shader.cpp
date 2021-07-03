#include "Shader.h"

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Spectre
{
	Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
		: id(0)
	{
		// Compile shaders
		unsigned int vertexShader   = compile(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentShaderSource);

		// Create shader program
		id = link(vertexShader, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(id);
	}

	void Shader::use()
	{
		glUseProgram(id);
	}

	void Shader::setUniformInt(const std::string& name, int value)
	{
		int location = getUniformLocation(name);
		glUniform1i(location, value);
	}

	void Shader::setUniformVec4(const std::string& name, const glm::vec4& vector)
	{
		int location = getUniformLocation(name);
		glUniform4fv(location, 1, glm::value_ptr(vector));
	}

	void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	unsigned int Shader::compile(unsigned int shaderType, const std::string& shaderSource)
	{
		std::cout << "Creating shader ... ";
		unsigned int shader = glCreateShader(shaderType);

		const char* source = shaderSource.c_str();
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);

		int isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

		if (!isCompiled) {
			int maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			std::cout << "Failed" << std::endl;
			std::cout << "Shader compile error info log: " << infoLog.data() << std::endl;
		}
		std::cout << "OK" << std::endl;

		return shader;
	}

	unsigned int Shader::link(unsigned int vertexShader, unsigned int fragmentShader)
	{
		std::cout << "Creating shader program ... ";
		unsigned int program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

		if (!isLinked) {
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			std::cout << "Failed" << std::endl;
			std::cout << "Shader link error log info: " << infoLog.data() << std::endl;
		}
		std::cout << "OK" << std::endl;

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		return program;
	}

	int Shader::getUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(id, name.c_str());
	}
}
