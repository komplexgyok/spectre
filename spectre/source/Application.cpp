#include "Application.h"

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Spectre
{
	Application::Application()
	{
		// GLFW initialization
		std::cout << "Initializing GLFW ... ";
		if (!glfwInit()) {
			std::cout << "Failed" << std::endl;
		}
		std::cout << "OK" << std::endl;

		// Create window
		std::cout << "Creating window ... ";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		window = glfwCreateWindow(1280, 720, "Windows 95", nullptr, nullptr);

		if (!window) {
			std::cout << "Failed" << std::endl;
		}
		std::cout << "OK" << std::endl;

		glfwMakeContextCurrent(window);

		// Glad initialization
		std::cout << "Initializing Glad ... ";
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed" << std::endl;
		}
		std::cout << "OK" << std::endl;

		// Vertex data
		float vertices[] = {
			// Position         // Color
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		// Vertex buffer
		glCreateBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

		std::string vertexShaderSource = R"(
			#version 330 core

			layout (location = 0) in vec3 position;
			layout (location = 1) in vec4 color;

			out vec4 vColor;

			void main()
			{
				vColor = color;
				gl_Position = vec4(position, 1.0f);
			}
		)";

		std::string fragmentShaderSource = R"(
			#version 330 core

			out vec4 color;

			in vec4 vColor;

			void main()
			{
				color = vColor;
			}
		)";

		// Vertex shader
		std::cout << "Creating vertex shader ... ";
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		if (!isCompiled) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);
			std::cout << "Failed" << std::endl;
			std::cout << "Error: " << infoLog.data() << std::endl;
		}
		std::cout << "OK" << std::endl;

		// Fragment shader
		std::cout << "Creating fragment shader ... ";
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		if (!isCompiled) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glCompileShader(vertexShader);
			std::cout << "Failed" << std::endl;
			std::cout << "Error: " << infoLog.data() << std::endl;
		}
		std::cout << "OK" << std::endl;

		// Shader program
		std::cout << "Creating shader program ... ";
		shader = glCreateProgram();

		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);

		glLinkProgram(shader);

		GLint isLinked = 0;
		glGetProgramiv(shader, GL_LINK_STATUS, &isLinked);

		if (!isLinked) {
			GLint maxLength = 0;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(shader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			std::cout << "Failed" << std::endl;
			std::cout << "Error: " << infoLog.data() << std::endl;
		}
		std::cout << "OK" << std::endl;

		glDetachShader(shader, vertexShader);
		glDetachShader(shader, fragmentShader);
	}

	Application::~Application()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Application::run()
	{
		while (!glfwWindowShouldClose(window)) {
			// Handle input
			glfwPollEvents();

			// Render
			float red = 1.0f / 255;
			float green = 130.0f / 255;
			float blue = 129.0f / 255;
			glClearColor(red, green, blue, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shader);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			
			// Swap buffers
			glfwSwapBuffers(window);
		}
	}
}
