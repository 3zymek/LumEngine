//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL texture operations (2D, 3D, Cubemap)
//          creation, update, delete, bind
//
//=============================================================================//

#include "modules/rhi/backend/gl_device.hpp"

namespace lum::rhi::gl {

	// -------------------------------------------------------------------------
	// Internal helpers
	// -------------------------------------------------------------------------

	/* @brief Resolves a pointer to raw pixel data from a TextureData descriptor.
	*  Returns nullptr if the appropriate pixel buffer is empty.
	*/
	LUM_FORCEINLINE static cvptr resolve_pixel_data( const ImageData& data ) noexcept {
		if (data.mIsHdr)
			return data.mFloatPixels.empty( ) ? nullptr : data.mFloatPixels.data( );
		return data.mPixels.empty( ) ? nullptr : data.mPixels.data( );
	}

	/* @brief Fills common metadata fields on a Texture from a TextureCreateInfo. */
	LUM_FORCEINLINE static void fill_texture_metadata(
		Texture& tex,
		const TextureCreateInfo& desc,
		uint32 width,
		uint32 height,
		uint32 mipmapLevels
	) noexcept {
		tex.mDataFormat = desc.mImageFormat;
		tex.mDataType = desc.mDataType;
		tex.mInternalFormat = desc.mImageLayout;
		tex.mSamples = desc.mSamples;
		tex.mRect.mWidth = width;
		tex.mRect.mHeight = height;
		tex.mRect.mDepth = desc.mDepth;
		tex.mType = desc.mTextureType;
		tex.mMipmapLevels = mipmapLevels;
	}

	// -------------------------------------------------------------------------
	// Public interface
	// -------------------------------------------------------------------------

	TextureHandle GLDevice::CreateTexture( const TextureCreateInfo& desc ) {

		LUM_ASSERT( desc.mTextureType != TextureType::None, "No texture type given" );

		switch (desc.mTextureType) {
		case TextureType::Texture2D:
		case TextureType::Texture2DSampled:  return create_texture_2d( desc );
		case TextureType::Texture2DArray:    return create_texture_2d_array( desc );
		case TextureType::Texture3D:         return create_texture_3d( desc );
		case TextureType::Cubemap:           return create_texture_cubemap( desc );
		default:                             return {};
		}

	}

	void GLDevice::Copy( TextureHandle src, TextureHandle dst ) {

		LUM_ASSERT( IsValid( src ), "Invalid source texture" );
		LUM_ASSERT( IsValid( dst ), "Invalid destination texture" );

		const Texture& srcTex = mTextures[ src ];
		const Texture& dstTex = mTextures[ dst ];

		glCopyImageSubData(
			srcTex.mHandle, skTextureTypeLookup[ LookupCast( srcTex.mType ) ], 0, 0, 0, 0,
			dstTex.mHandle, skTextureTypeLookup[ LookupCast( dstTex.mType ) ], 0, 0, 0, 0,
			srcTex.mRect.mWidth, srcTex.mRect.mHeight, 1
		);

	}

	void GLDevice::UnbindTexture( uint16 unit ) {

		glBindTextureUnit( unit, 0 );

	}

	void GLDevice::UpdateTexture( TextureHandle tex, const TextureUpdateDescription& desc ) {

		if (!IsValid( tex )) return;

		LUM_ASSERT( resolve_pixel_data( desc.mData ) != nullptr, "Texture pixel data is null" );
		LUM_ASSERT( desc.mData.mWidth > 0 && desc.mData.mHeight > 0, "Invalid texture dimensions" );

		const Texture& texture = mTextures[ tex ];

		uint32 width = (desc.mRect.mWidth == 0) ? desc.mData.mWidth : desc.mRect.mWidth;
		uint32 height = (desc.mRect.mHeight == 0) ? desc.mData.mHeight : desc.mRect.mHeight;

		width = std::clamp( width, 0u, texture.mRect.mWidth - desc.mRect.mX );
		height = std::clamp( height, 0u, texture.mRect.mHeight - desc.mRect.mY );

		glTextureSubImage2D(
			texture.mHandle,
			0,
			desc.mRect.mX, desc.mRect.mY,
			width, height,
			skImageFormatLookup[ LookupCast( texture.mDataFormat ) ],
			skTextureDataTypeLookup[ LookupCast( texture.mDataType ) ],
			resolve_pixel_data( desc.mData )
		);

		if (desc.bGenerateMipmaps && MipmapLvls( width, height ) > 1)
			glGenerateTextureMipmap( texture.mHandle );

	}

	void GLDevice::Delete( TextureHandle& texture ) {

		if (!IsValid( texture )) return;

		glDeleteTextures( 1, &mTextures[ texture ].mHandle );
		mTextures.Remove( texture );

	}

	void GLDevice::BindTexture( TextureHandle texture, uint16 binding ) {

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

	// -------------------------------------------------------------------------
	// Private creation helpers
	// -------------------------------------------------------------------------

	TextureHandle GLDevice::create_texture_2d( const TextureCreateInfo& desc ) {

		Texture texture;

		uint32 width = (desc.mWidth == 0) ? desc.mData.mWidth : desc.mWidth;
		uint32 height = (desc.mHeight == 0) ? desc.mData.mHeight : desc.mHeight;
		uint32 mipmapLevels = desc.bGenerateMipmaps
			? (desc.mMipmapLevels == 0 ? MipmapLvls( width, height ) : desc.mMipmapLevels)
			: 1;

		if (desc.mTextureType == TextureType::Texture2D) {

			cvptr data = resolve_pixel_data( desc.mData );

			glCreateTextures( GL_TEXTURE_2D, 1, &texture.mHandle );
			glTextureStorage2D(
				texture.mHandle,
				mipmapLevels,
				skImageLayoutLookup[ LookupCast( desc.mImageLayout ) ],
				width, height
			);

			if (data != nullptr)
				glTextureSubImage2D(
					texture.mHandle,
					0,
					0, 0,
					width, height,
					skImageFormatLookup[ LookupCast( desc.mImageFormat ) ],
					skTextureDataTypeLookup[ LookupCast( desc.mDataType ) ],
					data
				);

			if (mipmapLevels > 1)
				glGenerateTextureMipmap( texture.mHandle );

		}
		else if (desc.mTextureType == TextureType::Texture2DSampled) {

			glCreateTextures( GL_TEXTURE_2D_MULTISAMPLE, 1, &texture.mHandle );
			glTextureStorage2DMultisample(
				texture.mHandle,
				desc.mSamples,
				skImageLayoutLookup[ LookupCast( desc.mImageLayout ) ],
				width, height,
				GL_TRUE
			);

		}

		fill_texture_metadata( texture, desc, width, height, mipmapLevels );
		return mTextures.Append( std::move( texture ) );

	}

	TextureHandle GLDevice::create_texture_2d_array( const TextureCreateInfo& desc ) {

		LUM_ASSERT( desc.mDepth > 0, "Texture2DArray requires at least 1 layer (mDepth)" );

		Texture texture;

		uint32 width = (desc.mWidth == 0) ? desc.mData.mWidth : desc.mWidth;
		uint32 height = (desc.mHeight == 0) ? desc.mData.mHeight : desc.mHeight;
		uint32 mipmapLevels = desc.bGenerateMipmaps
			? (desc.mMipmapLevels == 0 ? MipmapLvls( width, height ) : desc.mMipmapLevels)
			: 1;

		glCreateTextures( GL_TEXTURE_2D_ARRAY, 1, &texture.mHandle );
		glTextureStorage3D(
			texture.mHandle,
			mipmapLevels,
			skImageLayoutLookup[ LookupCast( desc.mImageLayout ) ],
			width, height,
			desc.mDepth
		);

		if (mipmapLevels > 1)
			glGenerateTextureMipmap( texture.mHandle );

		fill_texture_metadata( texture, desc, width, height, mipmapLevels );
		return mTextures.Append( std::move( texture ) );

	}

	TextureHandle GLDevice::create_texture_3d( const TextureCreateInfo& ) {

		// TODO: implement
		return {};

	}

	TextureHandle GLDevice::create_texture_cubemap( const TextureCreateInfo& desc ) {

		Texture tex;

		glCreateTextures( GL_TEXTURE_CUBE_MAP, 1, &tex.mHandle );

		bool bEmpty = desc.mCubemap.mFaces[ 0 ].mWidth == 0;

		uint32 width = bEmpty ? desc.mWidth : static_cast< uint32 >(desc.mCubemap.mFaces[ 0 ].mWidth);
		uint32 height = bEmpty ? desc.mHeight : static_cast< uint32 >(desc.mCubemap.mFaces[ 0 ].mHeight);
		uint32 mipmapLevels = (desc.mMipmapLevels > 0) ? desc.mMipmapLevels : 1;

		glTextureStorage2D(
			tex.mHandle,
			mipmapLevels,
			skImageLayoutLookup[ LookupCast( desc.mImageLayout ) ],
			width, height
		);

		if (!bEmpty) {

			for (usize i = 0; i < 6; i++) {

				const ImageData& face = desc.mCubemap.mFaces[ i ];
				cvptr data = resolve_pixel_data( face );

				LUM_ASSERT(
					static_cast< uint32 >( face.mWidth ) == width &&
					static_cast< uint32 >( face.mHeight ) == height,
					"Cubemap face %zu has mismatched dimensions", i
				);

				glTextureSubImage3D(
					tex.mHandle,
					0, 0, 0,
					static_cast< GLint >(i),
					face.mWidth, face.mHeight, 1,
					skImageFormatLookup[ LookupCast( desc.mImageFormat ) ],
					skTextureDataTypeLookup[ LookupCast( desc.mDataType ) ],
					data
				);

			}

		}

		fill_texture_metadata( tex, desc, width, height, mipmapLevels );
		return mTextures.Append( std::move( tex ) );

	}

} // namespace lum::rhi::gl
