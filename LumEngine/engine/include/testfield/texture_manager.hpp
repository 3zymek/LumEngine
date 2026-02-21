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
		
		MTextureManager( rhi::RDevice* device ) { init(device); }

		/* @brief Retrieves a cached texture by path.
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

		/* @brief Loads a texture from disk or returns cached version.
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
			desc.mTextureType = rhi::RTextureType::Texture2D;
			rhi::RTextureHandle handle = mRenderDevice->CreateTexture(desc);

			mTextures[hash] = handle;
			
			return handle;
		}

		rhi::RTextureHandle LoadEquirectangularCubemap ( ccharptr path, ERootID root = ERootID::External ) {
			
			uint32 hash = HashStr(path);

			if (mTextures.contains(hash))
				return mTextures[hash];

			std::optional<FTextureData> data = AssetLoader::LoadTexture(root, path);
			if (!data) {
				LUM_LOG_ERROR("Failed to load texture %s", path);
				return mMissingTexture;
			}

			std::array<FTextureData, 6> convertedData = convert_equirectangular_to_cubemap(data.value(), data.value().mWidth);

			rhi::RTextureDescriptor desc;
			for (int32 i = 0; i < 6; i++)
				desc.mCubemap.mFaces[i] = convertedData[i];

			desc.mInternalFormat = rhi::RInternalImageFormat::RGB8;
			desc.mLoadedFormat = rhi::RLoadedImageFormat::RGB;
			desc.mTextureType = rhi::RTextureType::Cubemap;

			rhi::RTextureHandle handle = mRenderDevice->CreateTexture(desc);
			mTextures[hash] = handle;
			return handle;
		}

		/* @brief Returns a built-in fallback texture.
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

		/* @brief Initializes the texture manager with a render device.
		* Automatically creates default and missing fallback textures.
		* @param device Valid pointer to an RDevice instance.
		*/
		void init ( rhi::RDevice* device ) {

			mRenderDevice = device;

			create_default_textures();

		}

		/* @brief Creates built-in fallback textures on initialization.
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
				desc.mTextureType = rhi::RTextureType::Texture2D;
				mDefaultTexture = mRenderDevice->CreateTexture(desc);
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
				desc.mTextureType = rhi::RTextureType::Texture2D;
				mMissingTexture = mRenderDevice->CreateTexture(desc);
			}
		}

		std::array<FTextureData, 6> convert_equirectangular_to_cubemap(const FTextureData& equirect, int32 faceSize = 4096) {
		
			std::array<FTextureData, 6> faces;
			for (int32 i = 0; i < 6; i++) {
				faces[i].mWidth = faceSize;
				faces[i].mHeight = faceSize;
				faces[i].mPixels.resize(faceSize * faceSize * 3);
			}

			for (int32 face = 0; face < 6; face++) {

				for (int32 y = 0; y < faceSize; y++) {

					for (int32 x = 0; x < faceSize; x++) {

						float32 u = (x + 0.5f) / faceSize * 2.0f - 1.0f;
						float32 v = (y + 0.5f) / faceSize * 2.0f - 1.0f;

						glm::vec3 dir;

						switch (face) {
						case LUM_CUBEMAP_POSITIVE_X: dir = { 1, -v, -u }; break;
						case LUM_CUBEMAP_NEGATIVE_X: dir = { -1, -v, u }; break;
						case LUM_CUBEMAP_POSITIVE_Y: dir = { u, 1, v }; break;
						case LUM_CUBEMAP_NEGATIVE_Y: dir = { u, -1, -v }; break;
						case LUM_CUBEMAP_POSITIVE_Z: dir = { u, -v, 1 }; break;
						case LUM_CUBEMAP_NEGATIVE_Z: dir = { -u, -v, -1 }; break;
						}

						dir = glm::normalize(dir);

						float32 eu = atan2(dir.z, dir.x) / (2.0f * glm::pi<float32>()) + 0.5f;
						float32 ev = 1.0f - asin(dir.y) / glm::pi<float32>() + 0.5f;

						int32 px = (int32)(eu * equirect.mWidth) % equirect.mWidth;
						int32 py = (int32)(ev * equirect.mHeight) % equirect.mHeight;

						int32 srcIdx = (py * equirect.mWidth + px) * equirect.mColorChannels;
						uint8 r = equirect.mPixels[srcIdx + 0];
						uint8 g = equirect.mPixels[srcIdx + 1];
						uint8 b = equirect.mPixels[srcIdx + 2];

						int32 dstIdx = (y * faceSize + x) * 3;
						faces[face].mPixels[dstIdx + 0] = r;
						faces[face].mPixels[dstIdx + 1] = g;
						faces[face].mPixels[dstIdx + 2] = b;


					}

				}

			}

			return faces;

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