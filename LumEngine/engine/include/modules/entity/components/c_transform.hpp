#pragma once
#include "entity/ecs_common.hpp"
#include "core/core_pch.hpp"
namespace lum::ecs::components {
	struct TransformComponent {

		glm::vec3 position	= { 0.f, 0.f, 0.f };
		glm::vec3 scale		= { 1.f, 1.f, 1.f };
		glm::vec3 rotation	= { 0.f, 0.f, 0.f };

		glm::mat4 model		= glm::mat4(1.f);
		bool dirty			= false;

		LumComponentTag;

	};
}