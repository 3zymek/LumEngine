#pragma once
#include "entity/ecs_common.hpp"
namespace lum::ecs::components {
	struct AudioListenerComponent : ecs::detail::UniqueComponent {

		glm::vec3 forward	= { 0, 0, 1 };
		glm::vec3 up		= { 0, 1, 0 };

		LumComponentTag;

	};
}