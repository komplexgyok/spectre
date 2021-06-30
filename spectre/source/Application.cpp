#include "Application.h"

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Renderer2D.h"
#include "ResourceManager.h"

namespace Spectre
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		m_Window = std::make_unique<Window>(1280, 720, "Spectre Engine");
		s_Instance = this;
	}

	Application::~Application()
	{}

	void Application::run()
	{
		while (!glfwWindowShouldClose(m_Window->getNativeWindow())) {
			// Handle input
			glfwPollEvents();

			for (auto layer : layerStack) {
				layer->onUpdate();
				layer->onRender();

				layer->begin();
				layer->onImGuiRender();
				layer->end();
			}
			
			// Swap buffers
			glfwSwapBuffers(m_Window->getNativeWindow());
		}
	}

	void Application::addLayer(Layer* layer)
	{
		layerStack.push_back(layer);
		layer->onAttach();
	}
}
