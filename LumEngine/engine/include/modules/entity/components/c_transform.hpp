#pragma once

#include "entity/ecs_common.hpp"
#include "core/core_pch.hpp"

namespace lum {
	struct CTransform {

		LUM_COMPONENT_TAG;

		glm::vec3 mPosition	= { 0.f, 0.f, 0.f };
		glm::vec3 mScale	= { 1.f, 1.f, 1.f };
		glm::vec3 mRotation	= { 0.f, 0.f, 0.f };

		bool bDirty = false;

	};
}