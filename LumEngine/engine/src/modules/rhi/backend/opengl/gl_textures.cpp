//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL texture operations (2D, 3D, Cubemap)
//			creation, update, delete, bind
//
//=============================================================================//

#include "modules/rhi/backend/gl_device.hpp"

namespace lum::rhi::gl {

	RTextureHandle GLDevice::CreateTexture( const FTextureDescriptor& desc ) {

		LUM_HOTCHK_RETURN_CUSTOM(
			desc.mTextureType != TextureType::None,
			LUM_SEV_WARN,
			{},
			"No texture type given"
		);

		if (desc.mTextureType == TextureType::Texture2D)
			return create_texture_2d( desc );

		else if (desc.mTextureType == TextureType::Texture3D)
			return create_texture_3d( desc );

		else if (desc.mTextureType == TextureType::Cubemap)
			return create_texture_cubemap( desc );

		return {};

	}
	void GLDevice::UnbindTexture( TextureType type ) {

		glBindTextureUnit( skTextureTypeLookup[ LookupCast( type ) ], 0 );

	}
	void GLDevice::UpdateTexture( RTextureHandle tex, const FTextureUpdateDescriptor& desc ) {

		if (!IsValid( tex )) return;
		LUM_ASSERT( desc.mData.mPixels.data( ) != nullptr, "Texture pixel data is null" );
		LUM_ASSERT( desc.mData.mWidth > 0 && desc.mData.mHeight > 0, "Invalid texture dimensions" );

		const auto& texture = mTextures[ tex ];

		uint32 width = (desc.mRect.mWidth == 0) ? desc.mData.mWidth : desc.mRect.mWidth;
		uint32 height = (desc.mRect.mHeight == 0) ? desc.mData.mHeight : desc.mRect.mHeight;

		width = std::clamp( width, 0u, texture.mRect.mWidth - desc.mRect.x );
		height = std::clamp( height, 0u, texture.mRect.mHeight - desc.mRect.y );


		glTextureSubImage2D(
			texture.mHandle,
			0,
			desc.mRect.x,
			desc.mRect.y,
			width,
			height,
			skImageFormatLookup[ LookupCast( mTextures[ tex ].mDataFormat ) ],
			skTextureDataTypeLookup[ LookupCast( mTextures[ tex ].mDataType ) ],
			desc.mData.mPixels.data( )
		);

		uint32 mipmapLevels = desc.bGenerateMipmaps ? MipmapLvls( width, height ) : 1;

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap( texture.mHandle );
		}

	}
	void GLDevice::DeleteTexture( RTextureHandle& texture ) {

		if (!IsValid( texture )) return;

		glDeleteTextures( 1, &mTextures[ texture ].mHandle );

		mTextures.Remove( texture );

	}
	void GLDevice::BindTexture( RTextureHandle texture, uint16 binding ) {

		LUM_ASSERT( binding < skMaxTextureUnits, "Invalid texture binding" );
		if (!IsValid( texture )) return;

		if (mCurrentTextures[ binding ] == texture) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		mCurrentTextures[ binding ] = texture;
		glBindTextureUnit( binding, mTextures[ texture ].mHandle );

		LUM_PROFILER_CACHE_MISS( );

	}

	RTextureHandle GLDevice::create_texture_2d( const FTextureDescriptor& desc ) {

		FTexture texture;

		uint32 width = (desc.mWidth == 0) ? desc.mData.mWidth : desc.mWidth;
		uint32 height = (desc.mHeight == 0) ? desc.mData.mHeight : desc.mHeight;
		uint32 mipmapLevels = desc.bGenerateMipmaps ?
			(desc.mMipmapLevels == 0 ? MipmapLvls( width, height ) : desc.mMipmapLevels) : 1;

		cvptr data = (desc.mData.mPixels.empty( )) ? nullptr : desc.mData.mPixels.data( );

		glCreateTextures( GL_TEXTURE_2D, 1, &texture.mHandle );

		glTextureStorage2D(
			texture.mHandle,
			mipmapLevels,
			skImageLayoutLookup[ LookupCast( desc.mImageLayout ) ],
			width,
			height
		);

		if (data != nullptr)
			glTextureSubImage2D(
				texture.mHandle,
				0,
				0, 0,
				width,
				height,
				skImageFormatLookup[ LookupCast( desc.mImageFormat ) ],
				skTextureDataTypeLookup[ LookupCast( desc.mDataType ) ],
				data
			);

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap( texture.mHandle );
		}

		texture.mDataFormat = desc.mImageFormat;
		texture.mDataType = desc.mDataType;
		texture.mInternalFormat = desc.mImageLayout;
		texture.mRect.mWidth = width;
		texture.mRect.mHeight = height;
		texture.mType = TextureType::Texture2D;
		texture.mMipmapLevels = mipmapLevels;

		auto textureHandle = mTextures.Append( std::move( texture ) );

		LUM_LOG_INFO( "Created texture 2D %d", textureHandle.mID );

		return textureHandle;

	}
	RTextureHandle GLDevice::create_texture_3d( const FTextureDescriptor& ) {

		// TODO IMPLEMENT
		return {};

	}

	RTextureHandle GLDevice::create_texture_cubemap( const FTextureDescriptor& desc ) {

		FTexture tex;

		glCreateTextures( GL_TEXTURE_CUBE_MAP, 1, &tex.mHandle );

		int32 width = desc.mCubemap.mFaces[ 0 ].mWidth;
		int32 height = desc.mCubemap.mFaces[ 0 ].mHeight;

		glTextureStorage2D( tex.mHandle, 1, skImageLayoutLookup[ LookupCast( desc.mImageLayout ) ], width, height );

		for (usize i = 0; i < 6; i++) {

			auto texture = desc.mCubemap.mFaces[ i ];

			LUM_ASSERT( !texture.mPixels.empty( ), "Texture pixel data is null" );
			LUM_ASSERT( texture.mWidth > 0 && texture.mHeight > 0, "Invalid texture dimensions" );

			if (texture.mWidth != width || texture.mHeight != height) {
				LUM_LOG_ERROR( "Invalid cubemap height or width" );
				continue;
			}

			glTextureSubImage3D(
				tex.mHandle,
				0,
				0,
				0,
				i,
				texture.mWidth,
				texture.mHeight,
				1,
				skImageFormatLookup[ LookupCast( desc.mImageFormat ) ],
				GL_UNSIGNED_BYTE,
				texture.mPixels.data( )
			);

		}

		return mTextures.Append( std::move( tex ) );

	}

}