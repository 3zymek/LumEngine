/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once
#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Utils/TypeID.hpp"
#include "Entity/EcsTypes.hpp"

#include "Entity/Components/PointLight.gen.hpp"

namespace lum {

	/* @brief Point light component — emits light in all directions from a single position.
	* Attenuates linearly based on distance and radius.
	*/
	LUM_CLASS( Category = "LIGHTNING" )
	struct CPointLight : public ComponentBase {

		LUM_GENERATED_BODY( )

		LUM_PROPERTY( )
		float32 m_Intensity = 1.0f; // Light intensity in linear space

		LUM_PROPERTY( )
		float32 m_Radius = 10.0f; // Maximum range of the light

		LUM_PROPERTY( )
		Vector3 m_Color = Vector3( 1.0f ); // Light color in linear RGB

	};

	LUM_CLASS_EXTENSIONS( )

} // namespace lum