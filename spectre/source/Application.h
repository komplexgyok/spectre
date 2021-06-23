#pragma once

namespace Spectre
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void run();
	};
}
