#pragma once

#include "Mesh.h"

namespace Spectre
{
	struct MeshComponent
	{
		Mesh mesh;

		MeshComponent(const std::string& filepath)
			: mesh(filepath)
		{}
	};
}
