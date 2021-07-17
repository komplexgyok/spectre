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
		, m_IsFirstMouse(true), m_LastMouseX(0.0f), m_LastMouseY(0.0f)
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
		float cameraSpeed = 4.5f * deltaTime;
		float mouseSensitivity = 0.2f;

		if (Input::isMouseButtonPressed(1)) {
			if (Input::isKeyPressed(SPECTRE_KEY_W)) {
				m_Position += m_FrontDirection * cameraSpeed;
			}
		
			if (Input::isKeyPressed(SPECTRE_KEY_S)) {
				m_Position -= m_FrontDirection * cameraSpeed;
			}

			if (Input::isKeyPressed(SPECTRE_KEY_A)) {
				m_Position -= glm::normalize(glm::cross(m_FrontDirection, m_UpDirection)) * cameraSpeed;
			}

			if (Input::isKeyPressed(SPECTRE_KEY_D)) {
				m_Position += glm::normalize(glm::cross(m_FrontDirection, m_UpDirection)) * cameraSpeed;
			}

			float mouseX = Input::getMousePosition().x;
			float mouseY = Input::getMousePosition().y;

			if (m_IsFirstMouse) {
				std::cout << "First" << std::endl;
				m_LastMouseX = mouseX;
				m_LastMouseY = mouseY;
				m_IsFirstMouse = false;
			}

			float mouseOffsetX = m_LastMouseX - mouseX;
			float mouseOffsetY = mouseY - m_LastMouseY;
			m_LastMouseX = mouseX;
			m_LastMouseY = mouseY;

			mouseOffsetX *= mouseSensitivity;
			mouseOffsetY *= mouseSensitivity;

			m_Yaw -= mouseOffsetX;
			m_Pitch -= mouseOffsetY;

			if (m_Pitch > 89.0f) {
				m_Pitch = 89.0f;
			}
			if (m_Pitch < -89.0f) {
				m_Pitch = -89.0f;
			}

			m_FrontDirection.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_FrontDirection.y = sin(glm::radians(m_Pitch));
			m_FrontDirection.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_FrontDirection = glm::normalize(m_FrontDirection);

			setView(glm::lookAt(m_Position, m_Position + m_FrontDirection, m_UpDirection));
			ResourceManager::getShader("model")->use();
			ResourceManager::getShader("model")->setUniformMat4("u_View", m_View);
			ResourceManager::getShader("light")->use();
			ResourceManager::getShader("light")->setUniformMat4("u_View", m_View);
			ResourceManager::getShader("solid")->use();
			ResourceManager::getShader("solid")->setUniformMat4("u_View", m_View);
		}
		else {
			m_IsFirstMouse = true;
		}
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
