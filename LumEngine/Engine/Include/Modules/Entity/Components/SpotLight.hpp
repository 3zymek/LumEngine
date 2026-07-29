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
	LUM_CLASS( Category = "LIGHTNING" ) 
	struct CSpotLight : public ComponentBase {

		LUM_GENERATED_BODY( )

		LUM_PROPERTY( ) 
		float32 mIntensity = 100.f;

		LUM_PROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mRadius = 10.f; // Maximum range of the light

		LUM_PROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mInnerAngle = 15.f;

		LUM_PROPERTY( DragSpeed = 0.2f, MinVal = 0.1f ) 
		float32 mOuterCone = 30.f;

		LUM_PROPERTY( ) 
		Vector3 mDirection = Vector3( 0.f, -1.f, 0.f );

		LUM_PROPERTY( DragSpeed = 0.2f, MaxVal = 1.0f )
		Vector3 mColor = Vector3( 1.f ); // Light color in linear RGB

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum