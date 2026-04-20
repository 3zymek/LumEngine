//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Lights component for the ECS.
//
//=============================================================================//
#pragma once
#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief Point light component — emits light in all directions from a single position.
	* Attenuates linearly based on distance and radius.
	*/
	LCLASS( Category = "LIGHTNING" ) struct CPointLight : public Component {

		LPROPERTY( ) float32 mIntensity = 1.f; // Light intensity in linear space

		LPROPERTY( ) float32 mRadius = 10.f; // Maximum range of the light

		LPROPERTY( ) glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB

	};

	/* @brief Spot light component — emits light in a cone shape from a single position. */
	LCLASS( Category = "LIGHTNING" ) struct CSpotLight : public Component {

		LPROPERTY( ) float32 mIntensity = 100.f;

		LPROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mRadius = 10.f; // Maximum range of the light

		LPROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mInnerAngle = 15.f;

		LPROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mOuterCone = 30.f;

		LPROPERTY( ) 
		glm::vec3 mDirection = glm::vec3( 0.f, -1.f, 0.f );

		LPROPERTY( DragSpeed = 0.2f, MaxVal = 1.0f ) 
		glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB

	};

} // namespace lum