#pragma once

namespace Spectre
{
	class Application
	{
	public:
		virtual ~Application() = default;

		void run();
	};
}
