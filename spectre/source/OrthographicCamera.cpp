#include "OrthographicCamera.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "EventDispatcher.h"
#include "Input.h"
#include "KeyCodes.h"
#include "ResourceManager.h"

namespace Spectre
{
	OrthographicCamera::OrthographicCamera()
	{
		m_View = glm::mat4(1.0f);
		ResourceManager::getShader("texture")->use();
		ResourceManager::getShader("texture")->setUniformMat4("uView", m_View);

		m_Projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
		ResourceManager::getShader("texture")->setUniformMat4("uProjection", m_Projection);
	}
	
	void OrthographicCamera::onEvent(Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressEvent>(std::bind(&OrthographicCamera::onKeyPress, this, std::placeholders::_1));
		dispatcher.dispatch<MouseScrollEvent>(std::bind(&OrthographicCamera::onMouseScroll, this, std::placeholders::_1));
	}

	bool OrthographicCamera::onKeyPress(KeyPressEvent& event)
	{
		float cameraSpeed = 50.0f;

		if (event.getKey() == SPECTRE_KEY_A) {
			m_View = glm::translate(m_View, glm::vec3(cameraSpeed, 0.0f, 0.0f));
			ResourceManager::getShader("texture")->use();
			ResourceManager::getShader("texture")->setUniformMat4("uView", m_View);
		}
		if (event.getKey() == SPECTRE_KEY_D) {
			m_View = glm::translate(m_View, glm::vec3(-cameraSpeed, 0.0f, 0.0f));
			ResourceManager::getShader("texture")->use();
			ResourceManager::getShader("texture")->setUniformMat4("uView", m_View);
		}
		if (event.getKey() == SPECTRE_KEY_W) {
			m_View = glm::translate(m_View, glm::vec3(0.0f, -cameraSpeed, 0.0f));
			ResourceManager::getShader("texture")->use();
			ResourceManager::getShader("texture")->setUniformMat4("uView", m_View);
		}
		if (event.getKey() == SPECTRE_KEY_S) {
			m_View = glm::translate(m_View, glm::vec3(0.0f, cameraSpeed, 0.0f));
			ResourceManager::getShader("texture")->use();
			ResourceManager::getShader("texture")->setUniformMat4("uView", m_View);
		}

		return true;
	}

	bool OrthographicCamera::onMouseScroll(MouseScrollEvent& event)
	{
		m_ZoomLevel += static_cast<float>(-event.getOffset()) * 0.25f;
		m_Projection = glm::ortho(0.0f, 1280.0f * m_ZoomLevel, 0.0f, 720.0f * m_ZoomLevel, -1.0f, 1.0f);

		ResourceManager::getShader("texture")->use();
		ResourceManager::getShader("texture")->setUniformMat4("uProjection", m_Projection);

		glViewport(0, 0, static_cast<int32_t>(1280.0f * m_ZoomLevel), static_cast<int32_t>(720.0f * m_ZoomLevel));

		return true;
	}
}
