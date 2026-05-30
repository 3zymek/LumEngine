//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines position, rotation and scale in 3D world
//
//=============================================================================//
#pragma once
#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component storing the 3D transform of an entity. */
	LCLASS( Category = "TRANSFORM" ) struct CTransform : public ComponentBase {

        /* @brief Local position relative to parent. */
        LPROPERTY( ) Vector3 mPosition = Vector3( 0.0f );

        /* @brief Local non-uniform scale relative to parent. */
        LPROPERTY( ) Vector3 mScale = Vector3( 1.0f );

        /* @brief Local Euler rotation in degrees relative to parent. */
        LPROPERTY( ) Vector3 mRotation = Vector3( 0.0f );

        /* @brief World space transform matrix, computed by TransformSystem. */
        Matrix4 mWorldMatrix = Matrix4( 1.0f );

	};

} // namespace lum