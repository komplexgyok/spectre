#include "Window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event/KeyPressEvent.h"
#include "event/KeyReleaseEvent.h"
#include "event/MouseButtonPressEvent.h"
#include "event/MouseButtonReleaseEvent.h"
#include "event/MouseMoveEvent.h"
#include "event/MouseScrollEvent.h"
#include "event/WindowCloseEvent.h"
#include "event/WindowResizeEvent.h"

namespace Spectre
{
	Window::Window(unsigned int width, unsigned int height, std::string_view title)
		: m_Window(nullptr), m_Width(width), m_Height(height), m_Title(title)
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
		// Disable V-Sync
		glfwSwapInterval(0);

		// Glad initialization
		std::cout << "Initializing Glad ... ";
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed" << std::endl;
		}
		std::cout << "OK" << std::endl;

		glfwSetWindowUserPointer(m_Window, &m_EventCallback);

		// Set GLFW callbacks
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			std::function<void(Event&)>& eventCallback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressEvent event(key, false);
					eventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressEvent event(key, true);
					eventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleaseEvent event(key);
					eventCallback(event);
					break;
				}

				default: break;
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			std::function<void(Event&)>& eventCallback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			
			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressEvent event(button);
					eventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleaseEvent event(button);
					eventCallback(event);
					break;
				}
			
				default: break;
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double positionX, double positionY) {
			std::function<void(Event&)>& eventCallback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			
			MouseMoveEvent event(positionX, positionY);
			eventCallback(event);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double offsetX, double offsetY) {
			std::function<void(Event&)>& eventCallback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			MouseScrollEvent event(offsetX, offsetY);
			eventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			std::function<void(Event&)>& eventCallback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);
			
			WindowCloseEvent event;
			eventCallback(event);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			std::function<void(Event&)>& eventCallback = *(std::function<void(Event&)>*)glfwGetWindowUserPointer(window);

			WindowResizeEvent event(width, height);
			eventCallback(event);
		});

		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}
