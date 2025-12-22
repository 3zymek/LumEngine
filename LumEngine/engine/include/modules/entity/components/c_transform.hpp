#pragma once
#include "include/modules/entity/ecs_common.hpp"
#include "include/core/math/glm.hpp"
struct TransformComponent {

	LumComponentTag;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

};