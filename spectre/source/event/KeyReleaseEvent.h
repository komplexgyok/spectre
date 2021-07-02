#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(int key)
			: m_Key(key)
		{}

		virtual EventType getEventType() const override { return EventType::KeyRelease; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "KeyRelease (key: " << m_Key << ")" << std::endl;

			return stream.str();
		}

		static EventType getStaticType() { return EventType::KeyRelease; }

	private:
		int m_Key;
	};
}
