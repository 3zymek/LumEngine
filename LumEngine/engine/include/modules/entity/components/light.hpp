//========= Copyright (C) 2026 3zymek, MIT License ============//
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
	LCLASS( DisplayName = "point_light" ) struct CPointLight : public Component {

		LPROPERTY( ) float32 mIntensity = 1.f; // Light intensity in linear space

		LPROPERTY( ) float32 mRadius = 10.f; // Maximum range of the light

		LPROPERTY( ) glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB

	};

	/* @brief Spot light component — emits light in a cone shape from a single position. */
	LCLASS( DisplayName = "spot_light" ) struct CSpotLight : public Component {

		LPROPERTY( ) float32 mIntensity = 100.f;

		LPROPERTY( ) float32 mRadius = 10.f; // Maximum range of the light

		LPROPERTY( ) float32 mInnerConeAngle = 15.f;

		LPROPERTY( ) float32 mOuterConeAngle = 30.f;

		LPROPERTY( ) glm::vec3 mDirection = glm::vec3( 0.f, -1.f, 0.f );

		LPROPERTY( ) glm::vec3 mColor = { 1.f, 1.f, 1.f }; // Light color in linear RGB

	};

} // namespace lum