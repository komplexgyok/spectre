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
}
