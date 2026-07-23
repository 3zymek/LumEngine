//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Lights component for the ECS.
//
//=============================================================================//
#pragma once
#include "Entity/EcsCommon.hpp"
#include "Entity/Components/SpotLight.gen.hpp"

namespace lum {

	/* @brief Spot light component — emits light in a cone shape from a single position. */
	LCLASS( Category = "LIGHTNING" ) 
	struct CSpotLight : public ComponentBase {

		LUM_GENERATED_BODY( )

		LPROPERTY( ) 
		float32 mIntensity = 100.f;

		LPROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mRadius = 10.f; // Maximum range of the light

		LPROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mInnerAngle = 15.f;

		LPROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mOuterCone = 30.f;

		LPROPERTY( ) 
		Vector3 mDirection = Vector3( 0.f, -1.f, 0.f );

		LPROPERTY( DragSpeed = 0.2f, MaxVal = 1.0f )
		Vector3 mColor = Vector3( 1.f ); // Light color in linear RGB

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum