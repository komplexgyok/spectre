#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class MouseButtonReleaseEvent : public Event
	{
	public:
		MouseButtonReleaseEvent(int button)
			: m_Button(button)
		{}

		virtual EventType getEventType() const override { return EventType::MouseButtonRelease; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "MouseButtonRelease (button: " << m_Button << ")" << std::endl;

			return stream.str();
		}

		static EventType getStaticType() { return EventType::MouseButtonRelease; }

	private:
		int m_Button;
	};
}
