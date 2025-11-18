#pragma once
#include "math/vec3f.hpp"
#include "details/ecs_define.hpp"
struct TransformComponent {

	LumComponentTag;

	Vec3f position;
	Vec3f scale;

};