#pragma once

struct GLFWwindow;

namespace Spectre
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

	private:
		GLFWwindow* window;
		unsigned int vertexBuffer;
	};
}
