#pragma once

#include <memory>

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
		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int indexBuffer;
	};
}
