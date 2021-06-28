#include "Window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Spectre
{
	Window::Window(unsigned int width, unsigned int height, std::string_view title)
		: m_Width(width), m_Height(height), m_Title(title)
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
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

		if (!m_Window) {
			std::cout << "Failed" << std::endl;
		}
		std::cout << "OK" << std::endl;

		glfwMakeContextCurrent(m_Window);

		// Glad initialization
		std::cout << "Initializing Glad ... ";
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed" << std::endl;
		}
		std::cout << "OK" << std::endl;

		glEnable(GL_DEPTH_TEST);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}
