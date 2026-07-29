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
		LUM_PROPERTY( ) String mBasePath;

		/* @brief Base color multiplier. */
		LUM_PROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) Vector3 mBaseColor = Vector3( 1.0f );

		/* @brief Roughness scalar fallback when no map is bound. */
		LUM_PROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) float32 mRoughnessValue = 0.5f;

		/* @brief Metallic scalar fallback when no map is bound. */
		LUM_PROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) float32 mMetallicValue = 0.0f;

		rhi::TextureHandle mAlbedoTex; /* @brief Albedo (base color) texture map. */
		rhi::TextureHandle mNormalTex; /* @brief Normal map for surface detail. */
		rhi::TextureHandle mRoughnessTex; /* @brief Roughness texture map. */
		rhi::TextureHandle mMetallicTex; /* @brief Metallic texture map. */
		rhi::TextureHandle mAmbientTex; /* @brief Ambient occlusion texture map. */

	};
	
	LUM_CLASS_EXTENSIONS( )

} // namespace lum