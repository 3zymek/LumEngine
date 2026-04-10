//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Manages shared material bases and per-entity instances.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "core/limits.hpp"
#include "core/utils/handle_pool.hpp"
#include "render/render_common.hpp"
#include "render/material.hpp"

namespace lum {

	/* @brief Manages material base assets and per-entity material instances. 
	*
	* Base materials (FMaterialBase) are shared and stored by handle.
	* Instances (FMaterialInstance) are created from a base and resolve
	* missing textures to fallbacks automatically.
	*/
	class MMaterialManager {
	public:

		MMaterialManager( ) = default;

		/* @brief Initializes the manager with the given device and texture manager.
		* @param device  Pointer to the active render device.
		* @param texMgr  Pointer to the texture manager for fallback resolution.
		*/
		void Initialize( render::FRendererContext* ctx );

		/* @brief Uploads a material base to the pool and returns its handle.
		* @param base Material base data to upload.
		* @return Handle to the uploaded base material.
		*/
		LUM_NODISCARD
		MaterialBaseHandle UploadBase( const FMaterialBase& base );

		/* @brief Resolves a material descriptor and uploads it to the base material pool.
		* Loads textures from provided paths and automatically applies fallbacks for unset fields.
		* @param desc Descriptor containing optional texture paths and material parameters.
		* @return Handle to the uploaded base material.
		*/
		LUM_NODISCARD
		MaterialBaseHandle UploadBase( const FMaterialDescriptor& base );

		/* @brief Creates a material instance from a base handle.
		* Resolves invalid texture handles to fallback textures.
		* Falls back to the default material if the handle is invalid.
		* @param base Handle to the base material.
		* @return Fully resolved material instance ready for rendering.
		*/
		LUM_NODISCARD
		CMaterialInstance CreateInstance( MaterialBaseHandle base );

		/* @brief Returns a material instance created from the built-in default material. */
		LUM_NODISCARD
		CMaterialInstance GetDefaultInstance( );

		/* @brief Sets a specific texture map on a base material.
		* @param material Handle to the base material to modify.
		* @param mem      Which map to set (Albedo, Normal, Metallic, Roughness).
		* @param tex      Texture handle to assign.
		*/
		void SetBaseMap( MaterialBaseHandle material, MaterialMember mem, rhi::RTextureHandle tex );

	private:

		render::FRendererContext* mContext = nullptr;

		/* @brief Handle to the built-in default material, used as fallback for invalid handles. */
		MaterialBaseHandle mDefaultMaterial;

		/* @brief Pool storing all uploaded base materials, indexed by MaterialBaseHandle. */
		cstd::HandlePool<MaterialBaseHandle, FMaterialBase> mBaseMaterials{ limits::gMaxMaterials };

		/* @brief Internal initialization — sets up device and texture manager references, then creates the default material. */
		void init();

		/* @brief Returns the texture if valid, otherwise returns the default fallback. */
		rhi::RTextureHandle validate_texture( rhi::RTextureHandle tex );

		/* @brief Creates and uploads the built-in default material. */
		void create_defaults( );

	};

} // namespace lum