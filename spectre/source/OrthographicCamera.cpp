#include "OrthographicCamera.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "EventDispatcher.h"
#include "Input.h"
#include "KeyCodes.h"
#include "ResourceManager.h"

namespace Spectre
{
	OrthographicCamera::OrthographicCamera()
	{
		m_Position = glm::vec3(0.0f, 0.0f, 1.0f);

		m_View = glm::mat4(1.0f);
		ResourceManager::getShader("flat-color")->use();
		ResourceManager::getShader("flat-color")->setUniformMat4("uView", m_View);

		glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1000.0f, 1000.0f);

		ResourceManager::getShader("flat-color")->setUniformMat4("uProjection", projection);
	}
	
	void OrthographicCamera::onEvent(Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressEvent>(std::bind(&OrthographicCamera::onKeyPress, this, std::placeholders::_1));
		dispatcher.dispatch<MouseScrollEvent>(std::bind(&OrthographicCamera::onMouseScroll, this, std::placeholders::_1));
	}

	bool OrthographicCamera::onKeyPress(KeyPressEvent& event)
	{
		if (event.getKey() == SPECTRE_KEY_A) {
			m_View = glm::translate(m_View, glm::vec3(10.0f, 0.0f, 0.0f));
			ResourceManager::getShader("flat-color")->use();
			ResourceManager::getShader("flat-color")->setUniformMat4("uView", m_View);
		}
		if (event.getKey() == SPECTRE_KEY_D) {
			m_View = glm::translate(m_View, glm::vec3(-10.0f, 0.0f, 0.0f));
			ResourceManager::getShader("flat-color")->use();
			ResourceManager::getShader("flat-color")->setUniformMat4("uView", m_View);
		}
		if (event.getKey() == SPECTRE_KEY_W) {
			m_View = glm::translate(m_View, glm::vec3(0.0f, -10.0f, 0.0f));
			ResourceManager::getShader("flat-color")->use();
			ResourceManager::getShader("flat-color")->setUniformMat4("uView", m_View);
		}
		if (event.getKey() == SPECTRE_KEY_S) {
			m_View = glm::translate(m_View, glm::vec3(0.0f, 10.0f, 0.0f));
			ResourceManager::getShader("flat-color")->use();
			ResourceManager::getShader("flat-color")->setUniformMat4("uView", m_View);
		}

		return true;
	}

	bool OrthographicCamera::onMouseScroll(MouseScrollEvent& event)
	{
		m_ZoomLevel += static_cast<float>(event.getOffset()) * 0.25f;
		m_Projection = glm::ortho(0.0f, 1280.0f * m_ZoomLevel, 0.0f, 720.0f * m_ZoomLevel, -1.0f, 1.0f);
		ResourceManager::getShader("flat-color")->use();
		ResourceManager::getShader("flat-color")->setUniformMat4("uProjection", m_Projection);

		return true;
	}
}
