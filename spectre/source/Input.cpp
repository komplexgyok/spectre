#include "Input.h"

#include <GLFW/glfw3.h>

#include "Application.h"

namespace Spectre
{
	bool Input::isKeyPressed(int key)
	{
		GLFWwindow* window = Application::get().getWindow().getNativeWindow();
		int state = glfwGetKey(window, key);

		return state == GLFW_PRESS;
	}

	bool Input::isMouseButtonPressed(int button)
	{
		GLFWwindow* window = Application::get().getWindow().getNativeWindow();
		int state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	glm::vec2 Input::getMousePosition()
	{
		double x, y;

		GLFWwindow* window = Application::get().getWindow().getNativeWindow();
		glfwGetCursorPos(window, &x, &y);
		
		return glm::vec2(static_cast<float>(x), static_cast<float>(y));
	}
}
