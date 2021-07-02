#pragma once

#include <string>

namespace Spectre
{
	enum class EventType
	{
		KeyPress,
		KeyRelease,
		MouseButtonPress,
		MouseButtonRelease,
		MouseMove,
		MouseScroll,
		WindowClose,
		WindowResize
	};

	class Event
	{
	public:
		virtual EventType getEventType() const = 0;
		virtual std::string toString() const = 0;

		bool m_IsHandled = false;
	};
}
