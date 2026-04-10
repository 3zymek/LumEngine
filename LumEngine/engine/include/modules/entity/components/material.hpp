//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Material component for the ECS.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"
#include "render/material.hpp"

namespace lum {

	/* @brief ECS component holding a PBR material instance for an entity. */
	LCLASS( DisplayName = "material_instance" ) struct CMaterialInstance : public Component {
 
		LPROPERTY( ) String mBasePath; /* @brief Path to the .lmt file. */

		rhi::RTextureHandle mAlbedoTex; /* @brief Albedo (base color) texture map. */
		rhi::RTextureHandle mNormalTex; /* @brief Normal map for surface detail. */
		rhi::RTextureHandle mRoughnessTex; /* @brief Roughness texture map. */
		rhi::RTextureHandle mMetallicTex; /* @brief Metallic texture map. */
		rhi::RTextureHandle mAmbientTex; /* @brief Ambient occlusion texture map. */

		/* @brief Base color multiplier. */
		LPROPERTY() glm::vec3 mBaseColor = glm::vec3( 1.0f );

		/* @brief Roughness scalar fallback when no map is bound. */
		LPROPERTY() float32 mRoughnessValue = 0.5f;

		/* @brief Metallic scalar fallback when no map is bound. */
		LPROPERTY() float32 mMetallicValue = 0.0f;

	};

} // namespace lum