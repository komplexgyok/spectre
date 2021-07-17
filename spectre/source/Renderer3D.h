#pragma once

#include "components/MeshComponent.h"
#include "components/MeshRendererComponent.h"
#include "components/TransformComponent.h"
#include "EditorCamera.h"

namespace Spectre
{
	class Renderer3D
	{
	public:
		Renderer3D() = default;
		~Renderer3D() = default;

		void renderMesh(const TransformComponent& transform, const MeshComponent& mesh, const MeshRendererComponent& meshRenderer, const EditorCamera& camera, bool isOutline);
	};
}
