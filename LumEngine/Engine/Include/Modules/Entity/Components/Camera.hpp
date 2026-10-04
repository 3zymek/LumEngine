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
		float32 m_Fov = 0.0f;

		/* @brief Near clipping plane distance. */
		LUM_PROPERTY( ) 
		float32 m_Near = 0.0f;

		/* @brief Far clipping plane distance. */
		LUM_PROPERTY( ) 
		float32 m_Far = 0.0f;

		/* @brief Yaw angle in degrees, represents horizontal rotation. */
		LUM_PROPERTY( ) 
		float32 m_Yaw = -90.0f;

		/* @brief Pitch angle in degrees, represents vertical rotation. */
		LUM_PROPERTY( ) 
		float32 m_Pitch = 0.0f;

		/* @brief Point in world space the camera looks at. */
		LUM_PROPERTY( ) 
		Vector3 m_Target = Vector3( 1.0f, 0.0f, 0.0f);

		/* @brief Up vector defining camera orientation. */
		LUM_PROPERTY( ) 
		Vector3 m_Up = Vector3( 0.0f, 1.0f, 0.0f );

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum