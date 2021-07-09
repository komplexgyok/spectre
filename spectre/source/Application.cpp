#include "Application.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EventDispatcher.h"

namespace Spectre
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_IsRunning(true), m_DeltaTime(0.0f), m_LastFrame(0.0f)
	{
		m_Window = std::make_unique<Window>(1280, 720, "Spectre Engine");
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		s_Instance = this;
	}

	Application::~Application()
	{
		while (!m_LayerStack.empty()) {
			m_LayerStack.pop_back();
		}
	}

	void Application::run()
	{
		while (m_IsRunning) {
			float currentFrame = static_cast<float>(glfwGetTime());
			m_DeltaTime = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;

			// Handle input
			glfwPollEvents();

			for (auto const& layer : m_LayerStack) {
				layer->onUpdate(m_DeltaTime);
				layer->onRender();

				layer->imGuiBegin();
				layer->onImGuiRender();
				layer->imGuiEnd();
			}
			
			glfwSwapBuffers(m_Window->getNativeWindow());
		}
	}

	void Application::close()
	{
		m_IsRunning = false;
	}

	void Application::addLayer(std::unique_ptr<Layer> layer)
	{
		m_LayerStack.push_back(std::move(layer));
		m_LayerStack.back()->onAttach();
	}

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
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

	bool Application::onWindowResize(WindowResizeEvent& event)
	{
		glViewport(0, 0, event.getWidth(), event.getHeight());
		return true;
	}
}
