#pragma once

#include <glm/glm.hpp>

#include "event/Event.h"
#include "event/KeyPressEvent.h"
#include "event/MouseMoveEvent.h"

namespace Spectre
{
	class EditorCamera
	{
	public:
		EditorCamera(float FOV, float aspectRatio, float nearClip, float farClip);

		void onEvent(Event& event);
		void onUpdate(float deltaTime);

		bool onKeyPress(KeyPressEvent& event);
		bool onMouseMove(MouseMoveEvent& event);

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
		glm::vec3 m_FrontDirection;
		glm::vec3 m_UpDirection;

		float m_Yaw;
		float m_Pitch;

		glm::mat4 m_View;
		glm::mat4 m_Projection;
	};
}
