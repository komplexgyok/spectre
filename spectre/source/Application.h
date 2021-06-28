#pragma once

#include <memory>

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

	private:
		std::unique_ptr<Window> m_Window;
		Renderer2D renderer;
		unsigned int texture;

		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int indexBuffer;
	};
}
