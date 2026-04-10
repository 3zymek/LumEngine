//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Camera component for the ECS.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component representing a camera.
	* Stores projection and view parameters used for rendering.
	*/
	LCLASS( ) struct CCamera : public Component {

		/* @brief Vertical field of view in degrees. */
		LPROPERTY( ) float32 mFov = 0;

		/* @brief Near clipping plane distance. */
		LPROPERTY( ) float32 mNear = 0;

		/* @brief Far clipping plane distance. */
		LPROPERTY( ) float32 mFar = 0;

		/* @brief Yaw angle in degrees, represents horizontal rotation. */
		LPROPERTY( ) float32 mYaw = -90.0f;

		/* @brief Pitch angle in degrees, represents vertical rotation. */
		LPROPERTY( ) float32 mPitch = 0.0f;

		/* @brief Point in world space the camera looks at. */
		LPROPERTY( ) glm::vec3 mTarget = glm::vec3( 0.0f );

		/* @brief Up vector defining camera orientation. */
		LPROPERTY( ) glm::vec3 mUp = glm::vec3( 0.0f, 1.0f, 0.0f );

	};

} // namespace lum