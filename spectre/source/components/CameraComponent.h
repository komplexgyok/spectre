#pragma once

#include "Camera.h"

namespace Spectre
{
	struct CameraComponent
	{
		Camera camera;

		CameraComponent(const glm::vec3& position, float FOV, float aspectRatio, float nearClip, float farClip, bool isPrimary)
			: camera(position, FOV, aspectRatio, nearClip, farClip, isPrimary)
		{}
	};
}
