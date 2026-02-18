#pragma once
#include "entity/ecs_common.hpp"
namespace lum {
	struct CAudioListener : ecs::detail::CUnique {

		glm::vec3 forward	= { 0, 0, 1 };
		glm::vec3 up		= { 0, 1, 0 };

		LUM_COMPONENT_TAG;

	};
}