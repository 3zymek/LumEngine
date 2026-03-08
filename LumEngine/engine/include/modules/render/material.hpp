//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Material data structures for PBR rendering.
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_sampler.hpp"

namespace lum {

	struct FMaterialTexture {

		rhi::RTextureHandle mTexture;

		float32 mTextureScale = 1.0f;
		rhi::SamplerWrap mWrap = rhi::SamplerWrap::Repeat;

	};

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

	};

	/* @brief Descriptor for creating a material base.
	* All fields are optional — unset fields resolve to engine defaults at upload time.
	* Textures are specified as asset paths and resolved by the texture manager.
	*/
	struct FMaterialDescriptor {

		std::optional<String> mAlbedoTex;      // Path to albedo texture
		std::optional<String> mNormalTex;      // Path to normal map
		std::optional<String> mRoughnessTex;   // Path to roughness map
		std::optional<String> mMetallicTex;    // Path to metallic map

		std::optional<glm::vec3> mBaseColor;   // Base color multiplier, defaults to (1, 1, 1)

		std::optional<float32> mRoughnessValue = 0.5f; // Roughness multiplier, defaults to 0.5
		std::optional<float32> mMetallicValue = 0.0f;  // Metallic multiplier, defaults to 0.0
	
	};

	/* @brief Per-object instance of FMaterialBase with no additional overrides. */
	struct FMaterialInstance : FMaterialBase {};

}