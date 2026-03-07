//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: GPU texture loading, caching and lifecycle management.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"

namespace lum {

	///Forward Declare///
	struct FTextureData;
	namespace rhi {
		class RDevice;
		struct RTextureHandle;
	} // lum::rhi
	/////////////////////


	/* @brief Defines texture type presets used to configure format and sampling. */
	enum class TexturePreset : uint8 {
		Albedo,
		Normal,
		Metallic,
		Roughness,
	};

	/* @brief Built-in fallback texture types for missing or unset assets. */
	enum class FallbackTexture : uint8 {
		Missing,       // Checkered texture indicating missing asset
		DefaultAlbedo, // 1x1 white (255, 255, 255) neutral fallback for albedo and non-color maps
		DefaultNormal, // 1x1 flat normal (128, 128, 255) fallback when no normal map is provided
		DefaultRoughness,
		DefaultMetallic
	};

	/* @brief Descriptor for loading a cubemap from 6 individual face textures. */
	struct FCubemapDescriptor {
		/* @brief Paths to the 6 cubemap faces in order: +X, -X, +Y, -Y, +Z, -Z */
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
		
		MTextureManager( ) = default;

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
		rhi::RTextureHandle Get( StringView path, RootID id = RootID::External );

		/* @brief Loads a texture from disk or returns cached version.
		* Applies preset configuration (format, mipmaps) based on texture type.
		* Returns missing texture fallback on load failure.
		* @param path Path to the texture asset.
		* @param preset Texture type preset (Albedo, Normal, Metalness, Roughness).
		* @param id Root directory identifier.
		*/
		rhi::RTextureHandle Load( StringView path, TexturePreset preset, RootID id = RootID::External );

		/* @brief Maps a channel count to the corresponding RImageFormat.
		* @param channels Number of color channels (1 = R, 2 = RG, 3 = RGB, 4 = RGBA).
		* @return Matching RImageFormat. Defaults to RGBA for unknown values.
		*/
		rhi::ImageFormat ChannelsToFormat( uint32 channels );

		/* @brief Loads an equirectangular HDR image and converts it to a cubemap.
		* @param path Path to the equirectangular texture asset.
		* @param faceSize Resolution of each cubemap face in pixels.
		* @param root Root directory identifier.
		*/
		rhi::RTextureHandle LoadEquirectangularCubemap( StringView path, int32 faceSize, RootID root = RootID::External );

		/* @brief Returns a built-in fallback texture.
		* @param fallback Fallback type (Missing = checkered, Default = white 1x1).
		*/
		rhi::RTextureHandle GetFallbackTexture( FallbackTexture fallback );


	private:

		/* @brief Rendering backend */
		rhi::RDevice*		mRenderDevice = nullptr;

		/* @brief Fallback texture displayed when a requested asset cannot be found. */
		rhi::RTextureHandle mMissingTexture;

		/* @brief Default 1x1 white texture used as a neutral fallback for albedo and non-color maps. */
		rhi::RTextureHandle mDefaultAlbedoTexture;

		/* @brief Default 1x1 (128, 128, 255) texture representing a flat normal, used when no normal map is provided. */
		rhi::RTextureHandle mDefaultNormalTexture;

		rhi::RTextureHandle mDefaultRoughnessTexture;
		rhi::RTextureHandle mDefaultMetallicTexture;

		/* @brief Cache mapping texture path hashes to their GPU handles. */
		std::unordered_map<uint64, rhi::RTextureHandle> mTextures;


		/* @brief Internal initialization — sets up device reference and creates fallback textures. */
		void init( );

		/* @brief Creates built-in fallback textures on initialization.
		*
		* Creates two fallback textures:
		* - Default: 1x1 white texture used as safe fallback.
		* - Missing: loaded from internal assets, used when requested
		*   text	ure cannot be found. Falls back to default if file is missing.
		*/
		void create_defaults( );

		/* @brief Converts an equirectangular texture into six cubemap faces.
		* @param equirect Source equirectangular texture data.
		* @param faceSize Resolution of each output face in pixels.
		* @return Array of six FTextureData, one per cubemap face.
		*/
		std::array<FTextureData, 6> convert_equirectangular_to_cubemap( const FTextureData& equirect, int32 faceSize = 4096 );

		/* @brief Lookup table mapping ETexturePreset to texture binding slot indices. */
		static inline rhi::FTextureDescriptor sTexturePresetsLookup[] = {
			{ // ALBEDO
				.mImageLayout = rhi::ImageLayout::SRGB8_Alpha8,
				.mImageFormat = rhi::ImageFormat::RGBA,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true,
			},
			{ // NORMAL
				.mImageLayout = rhi::ImageLayout::RGB8,
				.mImageFormat = rhi::ImageFormat::RGB,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
			{ // METALNESS
				.mImageLayout = rhi::ImageLayout::R8,
				.mImageFormat = rhi::ImageFormat::R,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
			{ // ROUGHNESS
				.mImageLayout = rhi::ImageLayout::R8,
				.mImageFormat = rhi::ImageFormat::R,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
		};

		/* @brief Lookup table mapping ETexturePreset to texture binding slot indices. */
		inline constexpr
		static uint16 skTextureBindingLookups[] = {
			LUM_TEX_ALBEDO,
			LUM_TEX_NORMAL,
			LUM_TEX_METALNESS,
			LUM_TEX_ROUGHNESS
		};

	};

}