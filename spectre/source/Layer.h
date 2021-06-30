#pragma once

namespace Spectre
{
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void onAttach();
		virtual void onDetach();

		virtual void onUpdate();
		virtual void onRender();
		virtual void onImGuiRender();

		virtual void begin() {}
		virtual void end() {}
	};
}
