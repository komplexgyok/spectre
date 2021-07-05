#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height)
		{}

		virtual EventType getEventType() const override { return EventType::WindowResize; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "WindowResize (" << m_Width << ", " << m_Height << ")" << std::endl;

			return stream.str();
		}

		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }

		static EventType getStaticType() { return EventType::WindowResize; }

	private:
		int m_Width;
		int m_Height;
	};
}
