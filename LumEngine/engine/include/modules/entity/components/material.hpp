//========= Copyright (C) 2025-present 3zymek, MIT License ============//
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

		/* @brief Path to the .lmt file. */
		LPROPERTY( ) String mBasePath;

		/* @brief Base color multiplier. */
		LPROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) glm::vec3 mBaseColor = glm::vec3( 1.0f );

		/* @brief Roughness scalar fallback when no map is bound. */
		LPROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) float32 mRoughnessValue = 0.5f;

		/* @brief Metallic scalar fallback when no map is bound. */
		LPROPERTY( DragSpeed = 0.01, MaxVal = 1.0f ) float32 mMetallicValue = 0.0f;

		rhi::RTextureHandle mAlbedoTex; /* @brief Albedo (base color) texture map. */
		rhi::RTextureHandle mNormalTex; /* @brief Normal map for surface detail. */
		rhi::RTextureHandle mRoughnessTex; /* @brief Roughness texture map. */
		rhi::RTextureHandle mMetallicTex; /* @brief Metallic texture map. */
		rhi::RTextureHandle mAmbientTex; /* @brief Ambient occlusion texture map. */

	};

} // namespace lum