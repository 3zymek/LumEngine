//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: GPU texture loading, caching and lifecycle management.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"

namespace lum {

	namespace rhi {
		class RDevice;
		struct RTextureHandle;
	} // lum::rhi

	struct FTextureData;

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

		/* @brief Loads an equirectangular HDR image and converts it to a cubemap.
		* @param path Path to the equirectangular texture asset.
		* @param faceSize Resolution of each cubemap face in pixels.
		* @param root Root directory identifier.
		*/
		rhi::RTextureHandle LoadEquirectangularCubemap( ccharptr path, int32 faceSize, ERootID root = ERootID::External );


		/* @brief Returns a built-in fallback texture.
		* @param fallback Fallback type (Missing = checkered, Default = white 1x1).
		*/
		rhi::RTextureHandle GetFallbackTexture( EFallbackTexture fallback );


	private:

		rhi::RDevice*		mRenderDevice = nullptr;

		/* @brief Fallback texture displayed when a requested asset cannot be found. */
		rhi::RTextureHandle mMissingTexture;

		/* @brief Default 1x1 white texture used as a safe neutral fallback. */
		rhi::RTextureHandle mDefaultTexture;

		/* @brief Cache mapping texture path hashes to their GPU handles. */
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

		/* @brief Converts an equirectangular texture into six cubemap faces.
		* @param equirect Source equirectangular texture data.
		* @param faceSize Resolution of each output face in pixels.
		* @return Array of six FTextureData, one per cubemap face.
		*/
		std::array<FTextureData, 6> convert_equirectangular_to_cubemap( const FTextureData& equirect, int32 faceSize = 4096 );

		/* @brief Lookup table mapping ETexturePreset to texture binding slot indices. */
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

		/* @brief Lookup table mapping ETexturePreset to texture binding slot indices. */
		LUM_COMPILE_VARIABLE
		static uint16 skTextureBindingLookups[] = {
			LUM_TEX_ALBEDO,
			LUM_TEX_NORMAL,
			LUM_TEX_METALNESS,
			LUM_TEX_ROUGHNESS
		};

	};

}