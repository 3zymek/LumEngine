#pragma once
#include "entity/ecs_common.hpp"
#include "core/core_pch.hpp"
namespace lum::ecs::components {
	struct TransformComponent {

		LumComponentTag;

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

	};
}