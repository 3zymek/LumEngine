//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines position, rotation and scale in 3D world
//
//=============================================================================//
#pragma once
#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component storing the 3D transform of an entity. */
	LCLASS( ) struct CTransform : public Component {

		/* @brief World position. */
		LPROPERTY( ) glm::vec3 mPosition = { 0.f, 0.f, 0.f };

		/* @brief Non-uniform scale. */
		LPROPERTY( ) glm::vec3 mScale = { 1.f, 1.f, 1.f };

		/* @brief Euler rotation in degrees. */
		LPROPERTY( ) glm::vec3 mRotation = { 0.f, 0.f, 0.f };

	};

} // namespace lum