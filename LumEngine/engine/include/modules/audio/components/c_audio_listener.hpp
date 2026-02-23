#pragma once
#include "entity/ecs_common.hpp"

// TODO REMOVE
namespace lum {
	struct CAudioListener : Component {

		glm::vec3 forward	= { 0, 0, 1 };
		glm::vec3 up		= { 0, 1, 0 };

	};
}