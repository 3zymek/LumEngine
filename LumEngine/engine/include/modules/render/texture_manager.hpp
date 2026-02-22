//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Creation, management and cache textures
//
//=============================================================================//
#pragma once

#include "rhi/core/rhi_device.hpp"

namespace lum {

	enum class ETexturePreset : uint8 {
		Albedo,
		Normal,
		Metalness,
		Roughness,
	};

	enum class EFallbackTexture : uint8 {
		Missing,
		Default
	};

	struct FCubemapDescriptor {
		
		ccharptr mFaces[6];
		
	};

	/* @brief Manages GPU texture resources and their lifecycle.
	*
	* Handles texture loading, caching, and creation through the RHI backend.
	* Textures are identified by path hash and accessed via RTextureHandle.
	* Provides preset configurations for common texture types (Albedo, Normal, etc.)
	* and fallback textures for missing or invalid assets.
	*
	* @note Must be initialized with a valid RDevice before use.
	*/
	class MTextureManager {
	public:
		
		MTextureManager( ) {}

		/* @brief Initializes the texture manager with a render device.
		* Automatically creates default and missing fallback textures.
		* @param device Valid pointer to an RDevice instance.
		*/
		void Initialize( rhi::RDevice* device );

		/* @brief Retrieves a cached texture by path.
		* Returns missing texture fallback if not found in cache.
		* @param path Path to the texture asset.
		* @param id Root directory identifier.
		*/
		rhi::RTextureHandle Get( ccharptr path, ERootID id = ERootID::External );

		/* @brief Loads a texture from disk or returns cached version.
		* Applies preset configuration (format, mipmaps) based on texture type.
		* Returns missing texture fallback on load failure.
		* @param path Path to the texture asset.
		* @param preset Texture type preset (Albedo, Normal, Metalness, Roughness).
		* @param id Root directory identifier.
		*/
		rhi::RTextureHandle Load( ccharptr path, ETexturePreset preset, ERootID id = ERootID::External );

		rhi::RTextureHandle LoadEquirectangularCubemap( ccharptr path, ERootID root = ERootID::External );

		/* @brief Returns a built-in fallback texture.
		* @param fallback Fallback type (Missing = checkered, Default = white 1x1).
		*/
		rhi::RTextureHandle GetFallbackTexture( EFallbackTexture fallback );


	private:

		rhi::RDevice*		mRenderDevice = nullptr;
		rhi::RTextureHandle mMissingTexture;
		rhi::RTextureHandle mDefaultTexture;
		
		std::unordered_map<uint64, rhi::RTextureHandle> mTextures;

		/* Init internal implementation */
		void init( );

		/* @brief Creates built-in fallback textures on initialization.
		*
		* Creates two fallback textures:
		* - Default: 1x1 white texture used as safe fallback.
		* - Missing: loaded from internal assets, used when requested
		*   text	ure cannot be found. Falls back to default if file is missing.
		*/
		void create_default_textures( );

		std::array<FTextureData, 6> convert_equirectangular_to_cubemap( const FTextureData& equirect, int32 faceSize = 4096 );

		static inline rhi::RTextureDescriptor sTexturePresetsLookup[] = {
			{ // ALBEDO
				.mInternalFormat = rhi::RInternalImageFormat::SRGB8_Alpha8,
				.mLoadedFormat = rhi::RLoadedImageFormat::RGBA,
				.mDataType = rhi::RTextureDataType::UnsignedByte,
				.bGenerateMipmaps = true,
			},
			{ // NORMAL
				.mInternalFormat = rhi::RInternalImageFormat::RGBA8,
				.mLoadedFormat = rhi::RLoadedImageFormat::RGBA,
				.mDataType = rhi::RTextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
			{ // METALNESS
				.mInternalFormat = rhi::RInternalImageFormat::R8,
				.mLoadedFormat = rhi::RLoadedImageFormat::R,
				.mDataType = rhi::RTextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
			{ // ROUGHNESS
				.mInternalFormat = rhi::RInternalImageFormat::R8,
				.mLoadedFormat = rhi::RLoadedImageFormat::R,
				.mDataType = rhi::RTextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
		};

		LUM_COMPILE_VARIABLE
		static uint16 skTextureBindingLookups[] = {
			LUM_TEX_ALBEDO,
			LUM_TEX_NORMAL,
			LUM_TEX_METALNESS,
			LUM_TEX_ROUGHNESS
		};

	};

}