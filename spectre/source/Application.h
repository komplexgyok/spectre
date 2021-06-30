#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Layer.h"
#include "Renderer2D.h"
#include "Window.h"

namespace Spectre
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void addLayer(Layer* layer);

		static Application& get() { return *s_Instance; }
		Window& getWindow() { return *m_Window; }

	private:
		std::unique_ptr<Window> m_Window;
		Renderer2D renderer;
		std::vector<Layer*> layerStack;
		unsigned int texture;

		
		unsigned int indexBuffer;

		glm::vec4 backgroundColor;

		static Application* s_Instance;
	};
}
