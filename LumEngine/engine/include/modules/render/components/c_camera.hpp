#pragma once
#include "entity/ecs_common.hpp"
namespace lum {
	namespace ecs {

		struct CameraComponent {

			glm::vec3 front = { 0, 0, 1 };
			glm::vec3 right = { -1, 0, 0 };
			glm::vec3 up = { 0, 1, 0 };

			float fov;
			float aspect_ratio;
			float near_plane;
			float far_plane;

		};

	}
}