//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Material data structures for PBR rendering.
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

namespace lum {

	/* @brief Shared PBR material definition used as a base template.
	* Defines default texture maps and parameters inherited by material instances.
	*/
	struct FMaterialBase {

		/* @brief Albedo (base color) texture map. */
		rhi::RTextureHandle mAlbedoTex;

		/* @brief Normal map for surface detail. */
		rhi::RTextureHandle mNormalTex;

		/* @brief Roughness texture map. */
		rhi::RTextureHandle mRoughnessTex;

		/* @brief Metallic texture map. */
		rhi::RTextureHandle mMetallicTex;

		/* @brief Ambient occlusion texture map. */
		rhi::RTextureHandle mAmbientTex;

		/* @brief Base color multiplier. */
		glm::vec3 mBaseColor = glm::vec3(1.0f);

		/* @brief Roughness scalar fallback when no map is bound. */
		float32 mRoughnessValue = 0.5f;

		/* @brief Metallic scalar fallback when no map is bound. */
		float32 mMetallicValue = 0.0f;

		/* @brief Ambient occlusion scalar fallback when no map is bound. */
		//float32 mAmbient = 1.0f;

	};

	/* @brief Per-object instance of FMaterialBase with no additional overrides. */
	struct FMaterialInstance : FMaterialBase {};

}