#include "EditorCamera.h"

#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "EventDispatcher.h"
#include "Input.h"
#include "KeyCodes.h"
#include "ResourceManager.h"

namespace Spectre
{
	EditorCamera::EditorCamera(float FOV, float aspectRatio, float nearClip, float farClip)
		: m_FOV(FOV), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
		, m_Position(glm::vec3(0.0f, 0.0f, 6.0f)), m_FrontDirection(glm::vec3(0.0f, 0.0f, -1.0f)), m_UpDirection(glm::vec3(0.0f, 1.0f, 0.0f))
		, m_Yaw(-90.0f), m_Pitch(0.0f)
		, m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f))
	{
		m_FrontDirection.x = cos(glm::radians(m_Yaw));
		m_FrontDirection.z = sin(glm::radians(m_Yaw));

		m_View       = glm::lookAt(m_Position, m_Position + m_FrontDirection, m_UpDirection);
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}
	
	void EditorCamera::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressEvent>(std::bind(&EditorCamera::onKeyPress, this, std::placeholders::_1));
		dispatcher.dispatch<MouseMoveEvent>(std::bind(&EditorCamera::onMouseMove, this, std::placeholders::_1));
	}

	void EditorCamera::onUpdate(float deltaTime)
	{
		float sensitivity = 0.1f;

		if (Input::isKeyPressed(SPECTRE_KEY_D)) {
			m_Yaw += 0.1f * sensitivity;
		}
		if (Input::isKeyPressed(SPECTRE_KEY_A)) {
			m_Yaw -= 0.1f * sensitivity;
		}

		m_FrontDirection.x = cos(glm::radians(m_Yaw));
		m_FrontDirection.z = sin(glm::radians(m_Yaw));
		m_FrontDirection = glm::normalize(m_FrontDirection);

		setView(glm::lookAt(m_Position, m_Position + m_FrontDirection, m_UpDirection));
		ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformMat4("u_View", m_View);
	}

	bool EditorCamera::onKeyPress(KeyPressEvent& event)
	{
		/*if (event.getKey() == SPECTRE_KEY_RIGHT) {
			m_Yaw += 0.1;
		}

		m_FrontDirection.x = cos(glm::radians(m_Yaw));
		m_FrontDirection.z = sin(glm::radians(m_Yaw));

		setView(glm::lookAt(m_Position, m_FrontDirection, m_UpDirection));
		ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformMat4("u_View", m_View);*/

		return true;
	}

	bool EditorCamera::onMouseMove(MouseMoveEvent& event)
	{
		return true;
	}

	void EditorCamera::setView(const glm::mat4& view)
	{
		m_View = view;
	}
}
