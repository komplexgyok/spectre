#pragma once

#include "Mesh.h"

namespace Spectre
{
	struct MeshComponent
	{
		Mesh mesh;

		/*MeshComponent()
		{}*/

		MeshComponent(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
			: mesh(vertices, indices)
		{}
	};
}
