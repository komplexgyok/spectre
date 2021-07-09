#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Layer.h"
#include "Renderer2D.h"
#include "Window.h"
#include "event/WindowCloseEvent.h"
#include "event/WindowResizeEvent.h"

namespace Spectre
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void close();

		void addLayer(std::unique_ptr<Layer> layer);

		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& event);
		bool onWindowResize(WindowResizeEvent& event);

		static Application& get() { return *s_Instance; }
		inline Window& getWindow() { return *m_Window; }

	private:
		std::unique_ptr<Window> m_Window;
		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		bool m_IsRunning;
		float m_DeltaTime;
		float m_LastFrame;

		static Application* s_Instance;
	};
}
