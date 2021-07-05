#pragma once

#include <string>

namespace Spectre
{
	struct NameComponent
	{
		std::string name;

		NameComponent(const std::string& name)
			: name(name)
		{}
	};
}
