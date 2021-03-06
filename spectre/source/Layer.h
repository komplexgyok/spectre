#pragma once

#include <iostream>
#include "event/Event.h"

namespace Spectre
{
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void onAttach();
		virtual void onDetach();

		virtual void onEvent(Event& event);
		virtual void onUpdate(float deltaTime);
		virtual void onRender();
		virtual void onImGuiRender();

		virtual void imGuiBegin();
		virtual void imGuiEnd();
	};
}
