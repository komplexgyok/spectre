#pragma once

#include <glm/glm.hpp>

#include "event/Event.h"
#include "event/KeyPressEvent.h"
#include "event/MouseScrollEvent.h"

namespace Spectre
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera();

		void onEvent(Event& event);
		bool onKeyPress(KeyPressEvent& event);
		bool onMouseScroll(MouseScrollEvent& event);

	private:
		glm::vec3 m_Position;

		glm::mat4 m_View;
		glm::mat4 m_Projection;

		float m_ZoomLevel = 1.0f;
		float m_AspectRatio = 16.0f / 9.0f;
	};
}
