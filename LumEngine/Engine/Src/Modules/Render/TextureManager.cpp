//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: GPU texture loading, caching and lifecycle management.
//
//=============================================================================//

#include "Rhi/Core/RhiDevice.hpp"
#include "Render/TextureManager.hpp"
#include "Core/Utils/ResourceLoader.hpp"

namespace lum {

	//=======================================================//
	// Public
	//=======================================================//

	void TextureManager::Initialize( rhi::IRenderDevice& device ) {

		m_RenderDevice = &device;
		init( );

	}

	rhi::TextureHandle TextureManager::Get( StringView path, ResourceRoot id ) {

		uint64 hash = HashString( path );

		if (m_Textures.contains( hash )) {
			return m_Textures[ hash ];
		}
		else
			return m_MissingTexture;

	}

	rhi::TextureHandle TextureManager::Load( StringView path, TexturePreset preset, ResourceRoot id ) {

		uint64 hash = HashString( path );

		if (m_Textures.contains( hash ))
			return m_Textures[ hash ];

		auto data = ResourceLoader::LoadImageFromFile( id, path );

		if (!data) {
			LUM_LOG_ERROR( "Failed to load texture from file {}: {}", path.data( ), data.GetError() );
			return m_MissingTexture;
		}

		rhi::TextureCreateInfo desc = s_TexturePresetsLookup[ ToUnderlyingEnum( preset ) ];

		desc.m_Data = data.ValueRef( );
		desc.m_TextureType = rhi::TextureKind::Texture2D;
		desc.m_PixelFormat = ChannelsToFormat( data->m_Channels );
		rhi::TextureHandle handle = m_RenderDevice().CreateTexture( desc );

		m_Textures[ hash ] = handle;

		return handle;
	}

	rhi::PixelLayout TextureManager::ChannelsToFormat( uint32 channels ) {
		switch (channels) {
		case 1: { return rhi::PixelLayout::R; }; break;
		case 2: { return rhi::PixelLayout::RG; }; break;
		case 3: { return rhi::PixelLayout::RGB; }; break;
		case 4: { return rhi::PixelLayout::RGBA; }; break;
		default: { return rhi::PixelLayout::RGBA; }; break;
		}
		return rhi::PixelLayout::RGBA;
	}

	rhi::TextureHandle TextureManager::LoadEquirectangularCubemap( StringView path, ResourceRoot root ) {

		uint64 hash = HashString( path );

		if (m_Textures.contains( hash ))
			return m_Textures[ hash ];

		auto data = ResourceLoader::LoadImageFromFile( root, path );
		if (!data) {
			LUM_LOG_ERROR( "Failed to load texture %s: %s", path.data( ), data.GetError( ) );
			return m_MissingTexture;
		}

		uint32 faceSize = std::min(data->m_Width / 4, data->m_Height / 2);

		std::array<ImageData, 6> convertedData = convert_equirectangular_to_cubemap( data.ValueRef( ), faceSize );

		rhi::TextureCreateInfo desc;
		for (int32 i = 0; i < 6; i++) {
			desc.m_Cubemap.m_Faces[ i ] = convertedData[ i ];
		}

		desc.m_InternalFormat = rhi::TextureFormat::RGB16F;
		desc.m_PixelFormat = rhi::PixelLayout::RGB;
		desc.m_TextureType = rhi::TextureKind::Cubemap;

		rhi::TextureHandle handle = m_RenderDevice( ).CreateTexture( desc );
		m_Textures[ hash ] = handle;
		return handle;
	}

	rhi::TextureHandle TextureManager::GetFallbackTexture( FallbackTexture fallback ) {
		switch (fallback) {
		case FallbackTexture::Missing: return m_MissingTexture;
		case FallbackTexture::DefaultNormal: return m_DefaultNormalTexture;
		case FallbackTexture::DefaultRoughness: return m_DefaultRoughnessTexture;
		case FallbackTexture::DefaultMetallic: return m_DefaultMetallicTexture;
		default: return m_DefaultAlbedoTexture;
		}
	}





	//=======================================================//
	// Private
	//=======================================================//

	void TextureManager::init( ) {

		create_defaults( );

	}

	void TextureManager::create_defaults( ) {
		{ // Default albedo texture
			ImageData data;
			data.m_Pixels = { 255, 255, 255, 255 };
			data.m_Width = 1;
			data.m_Height = 1;
			data.m_Channels = 4;
			rhi::TextureCreateInfo desc;
			desc.m_Data = data;
			desc.m_InternalFormat = rhi::TextureFormat::SRGB8_Alpha8;
			desc.m_PixelFormat = rhi::PixelLayout::RGBA;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_DefaultAlbedoTexture = m_RenderDevice( ).CreateTexture( desc );
		}
		{ // Default normal texture
			ImageData data;
			data.m_Pixels = { 128, 128, 255 };
			data.m_Width = 1;
			data.m_Height = 1;
			data.m_Channels = 3;
			rhi::TextureCreateInfo desc;
			desc.m_Data = data;
			desc.m_InternalFormat = rhi::TextureFormat::RGBA16F;
			desc.m_PixelFormat = rhi::PixelLayout::RGBA;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_DefaultNormalTexture = m_RenderDevice( ).CreateTexture( desc );
		}
		{ // Default roughness texture
			ImageData data;
			data.m_Pixels = { 128 };
			data.m_Width = 1;
			data.m_Height = 1;
			data.m_Channels = 1;
			rhi::TextureCreateInfo desc;
			desc.m_Data = data;
			desc.m_InternalFormat = rhi::TextureFormat::R8;
			desc.m_PixelFormat = rhi::PixelLayout::R;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_DefaultRoughnessTexture = m_RenderDevice( ).CreateTexture( desc );
		}
		{ // Default metallic texture
			ImageData data;
			data.m_Pixels = { 255 };
			data.m_Width = 1;
			data.m_Height = 1;
			data.m_Channels = 1;
			rhi::TextureCreateInfo desc;
			desc.m_Data = data;
			desc.m_InternalFormat = rhi::TextureFormat::R8;
			desc.m_PixelFormat = rhi::PixelLayout::R;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_DefaultMetallicTexture = m_RenderDevice( ).CreateTexture( desc );
		}
		{ // Missing texture
			auto data = ResourceLoader::LoadImageFromFile( ResourceRoot::Internal, "textures/missingTex.png" );
			if (!data) {
				LUM_LOG_ERROR( "Failed to load missing texture fallback: %s", data.GetError() );
				m_MissingTexture = m_DefaultAlbedoTexture;
				return;
			}
			rhi::TextureCreateInfo desc;
			desc.m_Data = data.ValueRef( );
			desc.m_InternalFormat = rhi::TextureFormat::RGB8;
			desc.m_PixelFormat = rhi::PixelLayout::RGB;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_MissingTexture = m_RenderDevice( ).CreateTexture( desc );
		}
	}

	std::array<ImageData, 6> TextureManager::convert_equirectangular_to_cubemap( const ImageData& equirect, int32 faceSize ) {

		std::array<ImageData, 6> faces;
		for (int32 i = 0; i < 6; i++) {
			faces[ i ].m_Width = faceSize;
			faces[ i ].m_Height = faceSize;
			faces[ i ].m_Channels = 3;
			faces[ i ].m_IsHdr = equirect.m_IsHdr;
			if (equirect.m_IsHdr)
				faces[ i ].m_FloatPixels.resize( faceSize * faceSize * 3 );
			else
				faces[ i ].m_Pixels.resize( faceSize * faceSize * 3 );
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

					float32 eu = atan2( dir.m_Z, dir.m_X ) / (2.0f * Pi()) + 0.5f;
					float32 ev = 0.5f - asin( std::clamp( dir.m_Y, -1.0f, 1.0f ) ) / Pi();

					int32 px = ( int32 ) (eu * equirect.m_Width) % equirect.m_Width;
					int32 py = ( int32 ) (ev * equirect.m_Height) % equirect.m_Height;

					int32 srcIdx = (py * equirect.m_Width + px) * equirect.m_Channels;
					int32 dstIdx = (y * faceSize + x) * 3;

					if (equirect.m_IsHdr) {
						faces[ face ].m_FloatPixels[ dstIdx + 0 ] = equirect.m_FloatPixels[ srcIdx + 0 ];
						faces[ face ].m_FloatPixels[ dstIdx + 1 ] = equirect.m_FloatPixels[ srcIdx + 1 ];
						faces[ face ].m_FloatPixels[ dstIdx + 2 ] = equirect.m_FloatPixels[ srcIdx + 2 ];
					}
					else {
						faces[ face ].m_Pixels[ dstIdx + 0 ] = equirect.m_Pixels[ srcIdx + 0 ];
						faces[ face ].m_Pixels[ dstIdx + 1 ] = equirect.m_Pixels[ srcIdx + 1 ];
						faces[ face ].m_Pixels[ dstIdx + 2 ] = equirect.m_Pixels[ srcIdx + 2 ];
					}
				}
			}
		}

		return faces;
	}

}