//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Creation, management and cache textures
//
//=============================================================================//
#pragma once

#include "rhi/core/rhi_device.hpp"
#include "core/utils/asset_loader.hpp"

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

	/*====================================================================
	* @brief Manages GPU texture resources and their lifecycle.
	*
	* Handles texture loading, caching, and creation through the RHI backend.
	* Textures are identified by path hash and accessed via RTextureHandle.
	* Provides preset configurations for common texture types (Albedo, Normal, etc.)
	* and fallback textures for missing or invalid assets.
	*
	* @note Must be initialized with a valid RDevice before use.
	*/
	class CTextureManager {
	public:
		
		CTextureManager( rhi::RDevice* device ) { init(device); }

		/*====================================================================
		* @brief Retrieves a cached texture by path.
		* Returns missing texture fallback if not found in cache.
		* @param path Path to the texture asset.
		* @param id Root directory identifier.
		*/
		rhi::RTextureHandle Get ( ccharptr path, ERootID id = ERootID::External ) {

			uint64 hash = HashStr(path);

			if (mTextures.contains(hash)) {
				return mTextures[hash];
			}
			else
				return mMissingTexture;

		}

		/*====================================================================
		* @brief Loads a texture from disk or returns cached version.
		* Applies preset configuration (format, mipmaps) based on texture type.
		* Returns missing texture fallback on load failure.
		* @param path Path to the texture asset.
		* @param preset Texture type preset (Albedo, Normal, Metalness, Roughness).
		* @param id Root directory identifier.
		*/
		rhi::RTextureHandle Load ( ccharptr path, ETexturePreset preset, ERootID id = ERootID::External ) {

			uint64 hash = HashStr(path);

			if (mTextures.contains(hash))
				return mTextures[hash];

			std::optional<FTextureData> data = AssetLoader::LoadTexture(id, path);

			if (!data) {
				LUM_LOG_ERROR("Failed to load texture %s: %s", path, AssetLoader::GetErrorMessage());
				return mMissingTexture;
			}

			rhi::RTextureDescriptor desc = sTexturePresetsLookup[ToUnderlyingEnum(preset)];

			desc.mData = data.value();
			rhi::RTextureHandle handle = mRenderDevice->CreateTexture2D(desc);

			mTextures[hash] = handle;
			
			return handle;
		}

		/*====================================================================
		* @brief Returns a built-in fallback texture.
		* @param fallback Fallback type (Missing = checkered, Default = white 1x1).
		*/
		rhi::RTextureHandle GetFallbackTexture ( EFallbackTexture fallback ) {
			switch (fallback) {
			case EFallbackTexture::Missing: return mMissingTexture;
			case EFallbackTexture::Default: return mDefaultTexture;
			default: return mDefaultTexture;
			}
		}


	private:

		rhi::RDevice*		mRenderDevice = nullptr;
		rhi::RTextureHandle mMissingTexture;
		rhi::RTextureHandle mDefaultTexture;
		
		std::unordered_map<uint64, rhi::RTextureHandle> mTextures;

		/*====================================================================
		* @brief Initializes the texture manager with a render device.
		* Automatically creates default and missing fallback textures.
		* @param device Valid pointer to an RDevice instance.
		*/
		void init ( rhi::RDevice* device ) {

			mRenderDevice = device;

			create_default_textures();

		}

		/*====================================================================
		* @brief Creates built-in fallback textures on initialization.
		*
		* Creates two fallback textures:
		* - Default: 1x1 white texture used as safe fallback.
		* - Missing: loaded from internal assets, used when requested
		*   text	ure cannot be found. Falls back to default if file is missing.
		*/
		void create_default_textures ( ) {
			{ // Default texture
				FTextureData data;
				data.mPixels = { 255, 255, 255, 255 };
				data.mWidth = 1;
				data.mHeight = 1;
				rhi::RTextureDescriptor desc;
				desc.mData = data;
				desc.mInternalFormat = rhi::RInternalImageFormat::SRGB8_Alpha8;
				desc.mLoadedFormat = rhi::RLoadedImageFormat::RGBA;
				mDefaultTexture = mRenderDevice->CreateTexture2D(desc);
			}
			{ // Missing texture
				std::optional<FTextureData> data = AssetLoader::LoadTexture(ERootID::Internal, "textures/missingTex.png");
				if (!data) {
					LUM_LOG_ERROR("Failed to load missing texture fallback");
					mMissingTexture = mDefaultTexture;
					return;
				}
				rhi::RTextureDescriptor desc;
				desc.mData = data.value();
				desc.mInternalFormat = rhi::RInternalImageFormat::SRGB8_Alpha8;
				desc.mLoadedFormat = rhi::RLoadedImageFormat::RGBA;
				mMissingTexture = mRenderDevice->CreateTexture2D(desc);
			}
		}

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