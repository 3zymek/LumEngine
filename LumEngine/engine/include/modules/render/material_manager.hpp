//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Manages shared material bases and per-entity instances.
//
//=============================================================================//
#pragma once

#include "core/utils/handle_pool.hpp"
#include "core/core_common.hpp"
#include "render/material.hpp"
#include "core/limits.hpp"

namespace lum {

	class MTextureManager;

	namespace rhi {
		class RDevice;
	} // lum::rhi

	struct MaterialBaseHandle : cstd::BaseHandle<uint32> {};

	enum class EMaterialMember {
		Albedo,
		Normal,
		Metallic,
		Roughness,
	};

	/* @brief Manages material base assets and per-entity material instances.
	*
	* Base materials (FMaterialBase) are shared and stored by handle.
	* Instances (FMaterialInstance) are created from a base and resolve
	* missing textures to fallbacks automatically.
	*/
	class MMaterialManager {
	public:

		MMaterialManager( ) {}

		/* @brief Initializes the manager with the given device and texture manager.
		* @param device  Pointer to the active render device.
		* @param texMgr  Pointer to the texture manager for fallback resolution.
		*/
		void Initialize( rhi::RDevice* device, MTextureManager* texMgr );

		/* @brief Uploads a material base to the pool and returns its handle.
		* @param base Material base data to upload.
		* @return Handle to the uploaded base material.
		*/
		LUM_NODISCARD
		MaterialBaseHandle UploadBase( const FMaterialBase& base );

		/* @brief Creates a material instance from a base handle.
		* Resolves invalid texture handles to fallback textures.
		* Falls back to the default material if the handle is invalid.
		* @param base Handle to the base material.
		* @return Fully resolved material instance ready for rendering.
		*/
		LUM_NODISCARD
		FMaterialInstance CreateInstance( MaterialBaseHandle base );

		LUM_NODISCARD
		FMaterialInstance GetDefaultInstance();

		/* @brief Sets a specific texture map on a base material.
		* @param material Handle to the base material to modify.
		* @param mem      Which map to set (Albedo, Normal, Metallic, Roughness).
		* @param tex      Texture handle to assign.
		*/
		void SetBaseMap( MaterialBaseHandle material, EMaterialMember mem, rhi::RTextureHandle tex );

	private:

		MaterialBaseHandle	mDefaultMaterial;
		MTextureManager* mTextureManager = nullptr;
		rhi::RDevice* mRenderDevice = nullptr;

		cstd::HandlePool<MaterialBaseHandle, FMaterialBase> mBaseMaterials{ limits::gMaxMaterials };

		void init( );

		/* @brief Returns the texture if valid, otherwise returns the default fallback. */
		rhi::RTextureHandle validate_texture( rhi::RTextureHandle tex );

		/* @brief Creates and uploads the built-in default material. */
		void create_materials( );

	};

} // namespace lum