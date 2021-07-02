#pragma once

#include <functional>

namespace Spectre
{
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{}

		template<typename T>
		void dispatch(std::function<bool(T&)> function)
		{
			if (m_Event.getEventType() == T::getStaticType()) {
				m_Event.m_IsHandled = function(*(T*)&m_Event);
			}
		}

	private:
		Event& m_Event;
	};
}
