#pragma once

#include <glm/glm.hpp>

#include "Framebuffer.h"

namespace Spectre
{
	class Camera
	{
	public:
		Camera(const glm::vec3& position, float FOV, float aspectRatio, float nearClip, float farClip, bool isPrimary);

		inline glm::mat4 getView() const { return m_View; }
		inline glm::mat4 getProjection() const { return m_Projection; }
		inline Framebuffer& getFramebuffer() { return m_Framebuffer; }
		inline bool isPrimary() const { return m_IsPrimary; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_FrontDirection;
		glm::vec3 m_UpDirection;

		glm::mat4 m_View;
		glm::mat4 m_Projection;

		float m_FOV;
		float m_AspectRatio;
		float m_NearClip;
		float m_FarClip;

		float m_Yaw;
		float m_Pitch;

		Framebuffer m_Framebuffer;

		bool m_IsPrimary;
	};
}
