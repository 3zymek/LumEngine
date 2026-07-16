//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: GPU texture loading, caching and lifecycle management.
//
//=============================================================================//

#include "Rhi/Core/RhiDevice.hpp"
#include "Render/TextureManager.hpp"
#include "Core/Utils/ResourceLoader.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MTextureManager::Initialize( rhi::RenderDevice* device ) {

		mRenderDevice = device;

		init( );

	}

	rhi::TextureHandle MTextureManager::Get( StringView path, ResourceRoot id ) {

		uint64 hash = HashString( path );

		if (mTextures.contains( hash )) {
			return mTextures[ hash ];
		}
		else
			return mMissingTexture;

	}

	rhi::TextureHandle MTextureManager::Load( StringView path, TexturePreset preset, ResourceRoot id ) {

		uint64 hash = HashString( path );

		if (mTextures.contains( hash ))
			return mTextures[ hash ];

		std::optional<ImageData> data = ResourceLoader::LoadImageFromFile( id, path );

		if (!data) {
			LUM_LOG_ERROR( "Failed to load texture %s: %s", path.data( ), ResourceLoader::GetErrorMessage( ) );
			return mMissingTexture;
		}

		rhi::TextureCreateInfo desc = sTexturePresetsLookup[ ToUnderlyingEnum( preset ) ];

		desc.mData = data.value( );
		desc.mTextureType = rhi::TextureType::Texture2D;
		desc.mPixelFormat = ChannelsToFormat( data.value( ).mChannels );
		rhi::TextureHandle handle = mRenderDevice->CreateTexture( desc );

		mTextures[ hash ] = handle;

		return handle;
	}

	rhi::TexturePixelFormat MTextureManager::ChannelsToFormat( uint32 channels ) {
		switch (channels) {
		case 1: { return rhi::TexturePixelFormat::R; }; break;
		case 2: { return rhi::TexturePixelFormat::RG; }; break;
		case 3: { return rhi::TexturePixelFormat::RGB; }; break;
		case 4: { return rhi::TexturePixelFormat::RGBA; }; break;
		default: { return rhi::TexturePixelFormat::RGBA; }; break;
		}
		return rhi::TexturePixelFormat::RGBA;
	}

	rhi::TextureHandle MTextureManager::LoadEquirectangularCubemap( StringView path, ResourceRoot root ) {

		uint64 hash = HashString( path );

		if (mTextures.contains( hash ))
			return mTextures[ hash ];

		std::optional<ImageData> data = ResourceLoader::LoadImageFromFile( root, path );
		if (!data) {
			LUM_LOG_ERROR( "Failed to load texture %s", path.data( ) );
			return mMissingTexture;
		}

		uint32 faceSize = std::min(data.value().mWidth / 4, data.value().mHeight / 2);

		std::array<ImageData, 6> convertedData = convert_equirectangular_to_cubemap( data.value( ), faceSize );

		rhi::TextureCreateInfo desc;
		for (int32 i = 0; i < 6; i++) {
			desc.mCubemap.mFaces[ i ] = convertedData[ i ];
		}

		desc.mInternalFormat = rhi::TextureInternalFormat::RGB16F;
		desc.mPixelFormat = rhi::TexturePixelFormat::RGB;
		desc.mTextureType = rhi::TextureType::Cubemap;

		rhi::TextureHandle handle = mRenderDevice->CreateTexture( desc );
		mTextures[ hash ] = handle;
		return handle;
	}

	rhi::TextureHandle MTextureManager::GetFallbackTexture( FallbackTexture fallback ) {
		switch (fallback) {
		case FallbackTexture::Missing: return mMissingTexture;
		case FallbackTexture::DefaultNormal: return mDefaultNormalTexture;
		case FallbackTexture::DefaultRoughness: return mDefaultRoughnessTexture;
		case FallbackTexture::DefaultMetallic: return mDefaultMetallicTexture;
		default: return mDefaultAlbedoTexture;
		}
	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void MTextureManager::init( ) {

		create_defaults( );

	}

	void MTextureManager::create_defaults( ) {
		{ // Default albedo texture
			ImageData data;
			data.mPixels = { 255, 255, 255, 255 };
			data.mWidth = 1;
			data.mHeight = 1;
			data.mChannels = 4;
			rhi::TextureCreateInfo desc;
			desc.mData = data;
			desc.mInternalFormat = rhi::TextureInternalFormat::SRGB8_Alpha8;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDefaultAlbedoTexture = mRenderDevice->CreateTexture( desc );
		}
		{ // Default normal texture
			ImageData data;
			data.mPixels = { 128, 128, 255 };
			data.mWidth = 1;
			data.mHeight = 1;
			data.mChannels = 3;
			rhi::TextureCreateInfo desc;
			desc.mData = data;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDefaultNormalTexture = mRenderDevice->CreateTexture( desc );
		}
		{ // Default roughness texture
			ImageData data;
			data.mPixels = { 128 };
			data.mWidth = 1;
			data.mHeight = 1;
			data.mChannels = 1;
			rhi::TextureCreateInfo desc;
			desc.mData = data;
			desc.mInternalFormat = rhi::TextureInternalFormat::R8;
			desc.mPixelFormat = rhi::TexturePixelFormat::R;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDefaultRoughnessTexture = mRenderDevice->CreateTexture( desc );
		}
		{ // Default metallic texture
			ImageData data;
			data.mPixels = { 255 };
			data.mWidth = 1;
			data.mHeight = 1;
			data.mChannels = 1;
			rhi::TextureCreateInfo desc;
			desc.mData = data;
			desc.mInternalFormat = rhi::TextureInternalFormat::R8;
			desc.mPixelFormat = rhi::TexturePixelFormat::R;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDefaultMetallicTexture = mRenderDevice->CreateTexture( desc );
		}
		{ // Missing texture
			std::optional<ImageData> data = ResourceLoader::LoadImageFromFile( ResourceRoot::Internal, "textures/missingTex.png" );
			if (!data) {
				LUM_LOG_ERROR( "Failed to load missing texture fallback" );
				mMissingTexture = mDefaultAlbedoTexture;
				return;
			}
			rhi::TextureCreateInfo desc;
			desc.mData = data.value( );
			desc.mInternalFormat = rhi::TextureInternalFormat::RGB8;
			desc.mPixelFormat = rhi::TexturePixelFormat::RGB;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mMissingTexture = mRenderDevice->CreateTexture( desc );
		}
	}

	std::array<ImageData, 6>
	MTextureManager::convert_equirectangular_to_cubemap( const ImageData& equirect, int32 faceSize ) {

		std::array<ImageData, 6> faces;
		for (int32 i = 0; i < 6; i++) {
			faces[ i ].mWidth = faceSize;
			faces[ i ].mHeight = faceSize;
			faces[ i ].mChannels = 3;
			faces[ i ].mIsHdr = equirect.mIsHdr;
			if (equirect.mIsHdr)
				faces[ i ].mFloatPixels.resize( faceSize * faceSize * 3 );
			else
				faces[ i ].mPixels.resize( faceSize * faceSize * 3 );
		}

		for (int32 face = 0; face < 6; face++) {
			for (int32 y = 0; y < faceSize; y++) {
				for (int32 x = 0; x < faceSize; x++) {

					float32 u = (x + 0.5f) / faceSize * 2.0f - 1.0f;
					float32 v = (y + 0.5f) / faceSize * 2.0f - 1.0f;

					Vector3 dir;
					switch (face) {
					case LUM_CUBEMAP_POSITIVE_X: dir = { 1, -v, -u }; break;
					case LUM_CUBEMAP_NEGATIVE_X: dir = { -1, -v, u }; break;
					case LUM_CUBEMAP_POSITIVE_Y: dir = { u, 1, v }; break;
					case LUM_CUBEMAP_NEGATIVE_Y: dir = { u, -1, -v }; break;
					case LUM_CUBEMAP_POSITIVE_Z: dir = { u, -v, 1 }; break;
					case LUM_CUBEMAP_NEGATIVE_Z: dir = { -u, -v, -1 }; break;
					}
					dir = Normalize( dir );

					float32 eu = atan2( dir.mZ, dir.mX ) / (2.0f * Pi()) + 0.5f;
					float32 ev = 0.5f - asin( std::clamp( dir.mY, -1.0f, 1.0f ) ) / Pi();

					int32 px = ( int32 ) (eu * equirect.mWidth) % equirect.mWidth;
					int32 py = ( int32 ) (ev * equirect.mHeight) % equirect.mHeight;

					int32 srcIdx = (py * equirect.mWidth + px) * equirect.mChannels;
					int32 dstIdx = (y * faceSize + x) * 3;

					if (equirect.mIsHdr) {
						faces[ face ].mFloatPixels[ dstIdx + 0 ] = equirect.mFloatPixels[ srcIdx + 0 ];
						faces[ face ].mFloatPixels[ dstIdx + 1 ] = equirect.mFloatPixels[ srcIdx + 1 ];
						faces[ face ].mFloatPixels[ dstIdx + 2 ] = equirect.mFloatPixels[ srcIdx + 2 ];
					}
					else {
						faces[ face ].mPixels[ dstIdx + 0 ] = equirect.mPixels[ srcIdx + 0 ];
						faces[ face ].mPixels[ dstIdx + 1 ] = equirect.mPixels[ srcIdx + 1 ];
						faces[ face ].mPixels[ dstIdx + 2 ] = equirect.mPixels[ srcIdx + 2 ];
					}
				}
			}
		}

		return faces;
	}

}