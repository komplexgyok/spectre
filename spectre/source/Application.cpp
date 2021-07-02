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

#include "EventDispatcher.h"
#include "Renderer2D.h"
#include "ResourceManager.h"

namespace Spectre
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		m_Window = std::make_unique<Window>(1280, 720, "Spectre Engine");
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		s_Instance = this;
	}

	Application::~Application()
	{}

	void Application::run()
	{
		while (m_IsRunning) {
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

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));

		for (auto it = layerStack.end(); it != layerStack.begin(); ) {
			(*--it)->onEvent(event);

			if (event.m_IsHandled) {
				break;
			}
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& event)
	{
		m_IsRunning = false;
		return true;
	}
}
