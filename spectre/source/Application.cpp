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
		m_Window = std::make_unique<Window>(1280, 720, "Spectre Engine");

		// Vertex data
		float vertices[] = {
			// Position         // Color
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		// Index data
		unsigned int indices[] = {
			0, 1, 2
		};

		// Vertex array
		glCreateVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		// Vertex buffer
		glCreateBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

		// Index buffer
		glCreateBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		// Load shaders
		ResourceManager::addShader("basic", "../spectre/resources/shaders/basic.vert", "../spectre/resources/shaders/basic.frag");
	}

	Application::~Application()
	{}

	void Application::run()
	{
		while (!glfwWindowShouldClose(m_Window->getNativeWindow())) {
			// Handle input
			glfwPollEvents();

			// Render
			glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			ResourceManager::getShader("basic")->use();
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			
			// Swap buffers
			glfwSwapBuffers(m_Window->getNativeWindow());
		}
	}
}
