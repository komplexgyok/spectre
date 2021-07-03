#pragma once

#include <sstream>

#include "Event.h"

namespace Spectre
{
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(int key, bool isRepeated)
			: m_Key(key), m_IsRepeated(isRepeated)
		{}

		virtual EventType getEventType() const override { return EventType::KeyPress; }

		virtual std::string toString() const override
		{
			std::stringstream stream;
			stream << "KeyPress (key: " << m_Key << (m_IsRepeated ? ", repeated" : "") << ")" << std::endl;

			return stream.str();
		}

		int getKey() { return m_Key; }

		static EventType getStaticType() { return EventType::KeyPress; }

	private:
		int m_Key;
		bool m_IsRepeated;
	};
}
