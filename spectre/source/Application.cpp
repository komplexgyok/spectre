#include "Application.h"

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"

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

		window = glfwCreateWindow(1280, 720, "Spectre Engine", nullptr, nullptr);

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

		// Load shaders
		ResourceManager::addShader("basic", "../spectre/resources/shaders/basic.vert", "../spectre/resources/shaders/basic.frag");
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
			glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			ResourceManager::getShader("basic")->use();
			glDrawArrays(GL_TRIANGLES, 0, 3);
			
			// Swap buffers
			glfwSwapBuffers(window);
		}
	}
}
