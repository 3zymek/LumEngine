//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Defines position, rotation and scale in 3D world
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component storing the 3D transform of an entity. */
	struct CTransform : public Component {

		/* @brief World position. */
		glm::vec3 mPosition = { 0.f, 0.f, 0.f };

		/* @brief Non-uniform scale. */
		glm::vec3 mScale = { 1.f, 1.f, 1.f };

		/* @brief Euler rotation in degrees. */
		glm::vec3 mRotation = { 0.f, 0.f, 0.f };

	};

} // namespace lum