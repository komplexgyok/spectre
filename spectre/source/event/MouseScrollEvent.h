#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(double offsetX, double offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY)
		{}

		virtual EventType getEventType() const override { return EventType::MouseScroll; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "MouseScroll (" << m_OffsetX << ", " << m_OffsetY << ")" << std::endl;

			return stream.str();
		}

		int getOffset() { return m_OffsetY; }

		static EventType getStaticType() { return EventType::MouseScroll; }

	private:
		double m_OffsetX;
		double m_OffsetY;
	};
}
