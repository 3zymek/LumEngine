//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Camera component for the ECS.
//
//=============================================================================//
#pragma once
#include "Entity/EcsCommon.hpp"
#include "Entity/Components/Camera.gen.hpp"

namespace lum {

	/* @brief ECS component representing a camera.
	* Stores projection and view parameters used for rendering.
	*/
	LUM_CLASS( ) struct CCamera : public ComponentBase {

		LUM_GENERATED_BODY( )

		/* @brief Vertical field of view in degrees. */
		LUM_PROPERTY( ) 
		float32 mFov = 0;

		/* @brief Near clipping plane distance. */
		LUM_PROPERTY( ) 
		float32 mNear = 0;

		/* @brief Far clipping plane distance. */
		LUM_PROPERTY( ) 
		float32 mFar = 0;

		/* @brief Yaw angle in degrees, represents horizontal rotation. */
		LUM_PROPERTY( ) 
		float32 mYaw = -90.0f;

		/* @brief Pitch angle in degrees, represents vertical rotation. */
		LUM_PROPERTY( ) 
		float32 mPitch = 0.0f;

		/* @brief Point in world space the camera looks at. */
		LUM_PROPERTY( ) 
		Vector3 mTarget;

		/* @brief Up vector defining camera orientation. */
		LUM_PROPERTY( ) 
		Vector3 mUp = Vector3( 0.0f, 1.0f, 0.0f );

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum