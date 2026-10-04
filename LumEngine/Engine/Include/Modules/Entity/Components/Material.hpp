//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Material component for the ECS.
//
//=============================================================================//
#pragma once
#include "Render/Material.hpp"
#include "Entity/EcsCommon.hpp"
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

		rhi::TextureHandle m_AlbedoTex{};		/* @brief Albedo (base color) texture map. */
		rhi::TextureHandle m_NormalTex{};		/* @brief Normal map for surface detail. */
		rhi::TextureHandle m_RoughnessTex{};	/* @brief Roughness texture map. */
		rhi::TextureHandle m_MetallicTex{};		/* @brief Metallic texture map. */
		rhi::TextureHandle m_AmbientTex{};		/* @brief Ambient occlusion texture map. */

	};
	
	LUM_CLASS_EXTENSIONS( )

} // namespace lum