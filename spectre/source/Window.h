#pragma once

#include <string>
#include <string_view>

struct GLFWwindow;

namespace Spectre
{
	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, std::string_view title);
		~Window();

		inline GLFWwindow* getNativeWindow() const { return m_Window; }

	private:
		GLFWwindow* m_Window;
		unsigned int m_Width;
		unsigned int m_Height;
		std::string m_Title;
	};
}
