#include "EditorCamera.h"

#include "EventDispatcher.h"
#include "KeyCodes.h"

namespace Spectre
{
	EditorCamera::EditorCamera()
		: m_Position(glm::vec3(0.0f, 0.0f, 10.0f))
	{}
	
	void EditorCamera::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressEvent>(std::bind(&EditorCamera::onKeyPress, this, std::placeholders::_1));
	}

	bool EditorCamera::onKeyPress(KeyPressEvent& event)
	{
		if (event.getKey() == SPECTRE_KEY_W) {

		}

		return true;
	}
	void EditorCamera::setView(const glm::mat4& view)
	{
		m_View = view;
	}
}
