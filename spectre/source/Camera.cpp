#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

namespace Spectre
{
	Camera::Camera(const glm::vec3& position, float FOV, float aspectRatio, float nearClip, float farClip, bool isPrimary)
		: m_Position(position), m_FrontDirection(glm::vec3(0.0f, 0.0f, -1.0f)), m_UpDirection(glm::vec3(0.0f, 1.0f, 0.0f))
		, m_FOV(FOV), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
		, m_IsPrimary(isPrimary)
	{
		m_View       = glm::lookAt(m_Position, m_Position + m_FrontDirection, m_UpDirection);
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);

		m_Framebuffer.create(1280, 720);
	}
}
