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
#include "Rhi/Interfaces/Core/RhiImage.hpp"

#include "Entity/Components/Material.gen.hpp"

namespace lum {

	/* @brief ECS component holding a PBR material instance for an entity. */
	LUM_CLASS( Category = "RENDERING" ) 
	struct CMaterialInstance : public ComponentBase {

		LUM_GENERATED_BODY()

		/* @brief Path to the .lmt file. */
		LUM_PROPERTY( ) 
		String m_BasePath{};

		/* @brief Base color multiplier. */
		LUM_PROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) 
		Vector3 m_BaseColor = Vector3( 1.0f );

		/* @brief Roughness scalar fallback when no map is bound. */
		LUM_PROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) 
		float32 m_RoughnessValue = 0.0f;

		/* @brief Metallic scalar fallback when no map is bound. */
		LUM_PROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) 
		float32 m_MetallicValue = 0.0f;

		rhi::ImageHandle m_AlbedoImg{};		/* @brief Albedo (base color) texture map. */
		rhi::ImageHandle m_NormalImg{};		/* @brief Normal map for surface detail. */
		rhi::ImageHandle m_RoughnessImg{};	/* @brief Roughness texture map. */
		rhi::ImageHandle m_MetallicImg{};	/* @brief Metallic texture map. */
		rhi::ImageHandle m_AmbientImg{};	/* @brief Ambient occlusion texture map. */

	};
	
	LUM_CLASS_EXTENSIONS( )

} // namespace lum