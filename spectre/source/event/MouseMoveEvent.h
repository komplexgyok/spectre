#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(double positionX, double positionY)
			: m_PositionX(positionX), m_PositionY(positionY)
		{}

		virtual EventType getEventType() const override { return EventType::MouseMove; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "MouseMove (" << m_PositionX << ", " << m_PositionY << ")" << std::endl;

			return stream.str();
		}

		static EventType getStaticType() { return EventType::MouseMove; }

	private:
		double m_PositionX;
		double m_PositionY;
	};
}
