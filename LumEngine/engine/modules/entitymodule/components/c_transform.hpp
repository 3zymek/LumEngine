#pragma once
#include "details/ecs_define.hpp"
#include "math/glm.hpp"
struct TransformComponent {

	LumComponentTag;

	glm::vec3 position;
	glm::vec3 scale;

};