//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Material data structures for PBR rendering.
//
//=============================================================================//
#pragma once

#include "Rhi/RhiCommon.hpp"
#include "Core/Utils/Optional.hpp"

namespace lum {

	/* @brief Shared PBR material definition used as a base template.
	* Defines default texture maps and parameters inherited by material instances.
	*/
	struct MaterialBase {

		/* @brief Albedo (base color) texture map. */
		rhi::TextureHandle m_AlbedoTex{};

		/* @brief Normal map for surface detail. */
		rhi::TextureHandle m_NormalTex{};

		/* @brief Roughness texture map. */
		rhi::TextureHandle m_RoughnessTex{};

		/* @brief Metallic texture map. */
		rhi::TextureHandle m_MetallicTex{};

		/* @brief Ambient occlusion texture map. */
		rhi::TextureHandle m_AmbientTex{};

	};

	/* @brief Descriptor for creating a material base.
	* All fields are optional — unset fields resolve to engine defaults at upload time.
	* Textures are specified as asset paths and resolved by the texture manager.
	*/
	struct MaterialDescriptor {

		Optional<String> m_AlbedoTex{};      // Path to albedo texture
		Optional<String> m_NormalTex{};      // Path to normal map
		Optional<String> m_RoughnessTex{};   // Path to roughness map
		Optional<String> m_MetallicTex{};    // Path to metallic map

	};

	/* @brief Strongly typed handle referencing a base material in the pool. */
	struct MaterialBaseHandle : cstd::BaseHandle<uint32> {};

	/* @brief Identifies which texture map to set on a material base. */
	enum class MaterialMember {
		Albedo,
		Normal,
		Metallic,
		Roughness,
	};

}