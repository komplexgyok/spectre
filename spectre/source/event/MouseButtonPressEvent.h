#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class MouseButtonPressEvent : public Event
	{
	public:
		MouseButtonPressEvent(int button)
			: m_Button(button)
		{}

		virtual EventType getEventType() const override { return EventType::MouseButtonPress; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "MouseButtonPress (button: " << m_Button << ")" << std::endl;

			return stream.str();
		}

		static EventType getStaticType() { return EventType::MouseButtonPress; }

	private:
		int m_Button;
	};
}
