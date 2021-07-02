#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class WindowCloseEvent : public Event
	{
	public:
		virtual EventType getEventType() const override { return EventType::WindowClose; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "WindowClose" << std::endl;

			return stream.str();
		}

		static EventType getStaticType() { return EventType::WindowClose; }
	};
}
