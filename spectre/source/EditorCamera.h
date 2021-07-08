#pragma once

#include <glm/glm.hpp>

#include "event/Event.h"
#include "event/KeyPressEvent.h"

namespace Spectre
{
	class EditorCamera
	{
	public:
		EditorCamera();

		void onEvent(Event& event);
		bool onKeyPress(KeyPressEvent& event);

		glm::mat4 getView() const { return m_View; }
		glm::mat4 getProjection() const { return m_Projection; }

		void setView(const glm::mat4& view);
		void setProjection(const glm::mat4& projection);

	private:
		float m_FOV;
		float m_AspectRatio;
		float m_NearClip;
		float m_FarClip;

		glm::vec3 m_Position;

		glm::mat4 m_View;
		glm::mat4 m_Projection;
	};
}
