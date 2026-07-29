//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Lights component for the ECS.
//
//=============================================================================//
#pragma once
#include "Entity/EcsCommon.hpp"
#include "Entity/Components/PointLight.gen.hpp"

namespace lum {

	/* @brief Point light component — emits light in all directions from a single position.
	* Attenuates linearly based on distance and radius.
	*/
	LUM_CLASS( Category = "LIGHTNING" )
	struct CPointLight : public ComponentBase {

		LUM_GENERATED_BODY( )

		LUM_PROPERTY( )
		float32 mIntensity = 1.0f; // Light intensity in linear space

		LUM_PROPERTY( )
		float32 mRadius = 10.0f; // Maximum range of the light

		LUM_PROPERTY( )
		Vector3 mColor = Vector3( 1.0f ); // Light color in linear RGB

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum