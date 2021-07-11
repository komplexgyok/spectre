#pragma once

#include <functional>
#include <string>
#include <string_view>

#include "event/Event.h"

struct GLFWwindow;

namespace Spectre
{
	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, std::string_view title);
		~Window();

		inline GLFWwindow* getNativeWindow() const { return m_Window; }

		void setEventCallback(std::function<void(Event& event)> eventCallback) { m_EventCallback = eventCallback; }

		bool m_IsVsync;

	private:
		GLFWwindow* m_Window;
		unsigned int m_Width;
		unsigned int m_Height;
		std::string m_Title;
		std::function<void(Event&)> m_EventCallback;
	};
}
