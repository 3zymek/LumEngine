//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: GPU texture loading, caching and lifecycle management.
//
//=============================================================================//

#include "rhi/core/rhi_device.hpp"
#include "render/texture_manager.hpp"
#include "core/utils/asset_loader.hpp"

namespace lum {
	
	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MTextureManager::Initialize( rhi::RDevice* device ) {

		mRenderDevice = device;

		init();

	}

	rhi::RTextureHandle MTextureManager::Get( StringView path, RootID id ) {

		uint64 hash = HashStr(path);

		if (mTextures.contains(hash)) {
			return mTextures[hash];
		}
		else
			return mMissingTexture;

	}

	rhi::RTextureHandle MTextureManager::Load( StringView path, TexturePreset preset, RootID id ) {

		uint64 hash = HashStr(path);

		if (mTextures.contains(hash))
			return mTextures[hash];

		std::optional<FTextureData> data = AssetLoader::LoadTexture(id, path);

		if (!data) {
			LUM_LOG_ERROR("Failed to load texture %s: %s", path.data(), AssetLoader::GetErrorMessage());
			return mMissingTexture;
		}

		rhi::FTextureDescriptor desc = sTexturePresetsLookup[ToUnderlyingEnum(preset)];

		desc.mData = data.value();
		desc.mTextureType = rhi::TextureType::Texture2D;
		desc.mImageFormat = ChannelsToFormat(data.value().mChannels);
		rhi::RTextureHandle handle = mRenderDevice->CreateTexture(desc);

		mTextures[hash] = handle;

		return handle;
	}

	rhi::ImageFormat MTextureManager::ChannelsToFormat( uint32 channels ) {
		switch (channels) {
		case 1: { return rhi::ImageFormat::R; }; break;
		case 2: { return rhi::ImageFormat::RG; }; break;
		case 3: { return rhi::ImageFormat::RGB; }; break;
		case 4: { return rhi::ImageFormat::RGBA; }; break;
		default: { return rhi::ImageFormat::RGBA; }; break;
		}
		return rhi::ImageFormat::RGBA;
	}

	rhi::RTextureHandle MTextureManager::LoadEquirectangularCubemap( StringView path, int32 faceSize, RootID root ) {

		uint64 hash = HashStr(path);

		if (mTextures.contains(hash))
			return mTextures[hash];

		std::optional<FTextureData> data = AssetLoader::LoadTexture(root, path);
		if (!data) {
			LUM_LOG_ERROR("Failed to load texture %s", path.data());
			return mMissingTexture;
		}

		std::array<FTextureData, 6> convertedData = convert_equirectangular_to_cubemap(data.value(), faceSize);

		rhi::FTextureDescriptor desc;
		for (int32 i = 0; i < 6; i++)
			desc.mCubemap.mFaces[i] = convertedData[i];

		desc.mImageLayout = rhi::ImageLayout::RGB8;
		desc.mImageFormat = rhi::ImageFormat::RGB;
		desc.mTextureType = rhi::TextureType::Cubemap;

		rhi::RTextureHandle handle = mRenderDevice->CreateTexture(desc);
		mTextures[hash] = handle;
		return handle;
	}

	rhi::RTextureHandle MTextureManager::GetFallbackTexture( FallbackTexture fallback ) {
		switch (fallback) {
		case FallbackTexture::Missing: return mMissingTexture;
		case FallbackTexture::DefaultAlbedo: return mDefaultAlbedoTexture;
		case FallbackTexture::DefaultNormal: return mDefaultNormalTexture;
		default: return mDefaultAlbedoTexture;
		}
	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void MTextureManager::init( ) {

		create_defaults();

	}

	void MTextureManager::create_defaults( ) {
		{ // Default albedo texture
			FTextureData data;
			data.mPixels = { 255, 255, 255, 255 };
			data.mWidth = 1;
			data.mHeight = 1;
			data.mChannels = 4;
			rhi::FTextureDescriptor desc;
			desc.mData = data;
			desc.mImageLayout = rhi::ImageLayout::SRGB8_Alpha8;
			desc.mImageFormat = rhi::ImageFormat::RGBA;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDefaultAlbedoTexture = mRenderDevice->CreateTexture(desc);
		}
		{ // Default normal texture
			FTextureData data;
			data.mPixels = { 128, 128, 255 };
			data.mWidth = 1;
			data.mHeight = 1;
			data.mChannels = 3;
			rhi::FTextureDescriptor desc;
			desc.mData = data;
			desc.mImageLayout = rhi::ImageLayout::RGB8;
			desc.mImageFormat = rhi::ImageFormat::RGB;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDefaultNormalTexture = mRenderDevice->CreateTexture(desc);
		}
		{ // Missing texture
			std::optional<FTextureData> data = AssetLoader::LoadTexture(RootID::Internal, "textures/missingTex.png");
			if (!data) {
				LUM_LOG_ERROR("Failed to load missing texture fallback");
				mMissingTexture = mDefaultAlbedoTexture;
				return;
			}
			rhi::FTextureDescriptor desc;
			desc.mData = data.value();
			desc.mImageLayout = rhi::ImageLayout::RGB8;
			desc.mImageFormat = rhi::ImageFormat::RGB;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mMissingTexture = mRenderDevice->CreateTexture(desc);
		}
	}

	std::array<FTextureData, 6> 
	MTextureManager::convert_equirectangular_to_cubemap( const FTextureData& equirect, int32 faceSize ) {

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

					int32 srcIdx = (py * equirect.mWidth + px) * equirect.mChannels;
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

}