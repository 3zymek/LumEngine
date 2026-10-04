//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL texture operations (2D, 3D, Cubemap)
//          creation, update, delete, bind
//
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	// -------------------------------------------------------------------------
	// Internal helpers
	// -------------------------------------------------------------------------

	/* @brief Resolves a pointer to raw pixel data from a TextureData descriptor.
	*  Returns nullptr if the appropriate pixel buffer is empty.
	*/
	LUM_FORCEINLINE static const void* resolve_pixel_data( const ImageData& data ) noexcept {
		if (data.m_IsHdr)
			return data.m_FloatPixels.empty( ) ? nullptr : data.m_FloatPixels.data( );
		return data.m_Pixels.empty( ) ? nullptr : data.m_Pixels.data( );
	}

	/* @brief Fills common metadata fields on a Texture from a TextureCreateInfo. */
	LUM_FORCEINLINE static void fill_texture_metadata(
		Texture& tex,
		const TextureCreateInfo& desc,
		uint32 width,
		uint32 height,
		uint32 mipmapLevels
	) noexcept {
		tex.m_PixelLayout = desc.m_PixelFormat;
		tex.m_PixelDataType = desc.m_DataType;
		tex.m_Format = desc.m_InternalFormat;
		tex.m_SampleCount = desc.m_Samples;
		tex.m_Rect.m_Width = width;
		tex.m_Rect.m_Height = height;
		tex.m_Rect.m_Depth = desc.m_Depth;
		tex.m_Kind = desc.m_TextureType;
		tex.m_MipmapLevels = mipmapLevels;
	}

	// -------------------------------------------------------------------------
	// Public interface
	// -------------------------------------------------------------------------

	TextureHandle GLDevice::CreateTexture( const TextureCreateInfo& desc ) {

		LUM_ASSERT( desc.m_TextureType != TextureKind::None, "No texture type given" );

		switch (desc.m_TextureType) {
			case TextureKind::Texture2D:
			case TextureKind::Texture2DMultiSampled:	return create_texture_2d( desc );
			case TextureKind::Texture2DArray:			return create_texture_2d_array( desc );
			case TextureKind::Image3D:					return create_texture_3d( desc );
			case TextureKind::Cubemap:					return create_texture_cubemap( desc );
			default:									return {};
		}

	}

	void GLDevice::Copy( TextureHandle src, TextureHandle dst ) {

		LUM_ASSERT( IsValid( src ), "Invalid source texture" );
		LUM_ASSERT( IsValid( dst ), "Invalid destination texture" );

		const Texture& srcTex = m_Textures[ src ];
		const Texture& dstTex = m_Textures[ dst ];

		glCopyImageSubData(
			srcTex.m_Handle, sk_TextureTypeLookup[ LookupCast( srcTex.m_Kind ) ], 0, 0, 0, 0,
			dstTex.m_Handle, sk_TextureTypeLookup[ LookupCast( dstTex.m_Kind ) ], 0, 0, 0, 0,
			srcTex.m_Rect.m_Width, srcTex.m_Rect.m_Height, 1
		);

	}

	void GLDevice::UnbindTexture( uint16 unit ) {

		glBindTextureUnit( unit, 0 );

	}

	void GLDevice::UpdateTexture( TextureHandle tex, const TextureUpdateDescription& desc ) {

		if (!IsValid( tex )) return;

		LUM_ASSERT( resolve_pixel_data( desc.m_Data ) != nullptr, "Texture pixel data is null" );
		LUM_ASSERT( desc.m_Data.m_Width > 0 && desc.m_Data.m_Height > 0, "Invalid texture dimensions" );

		const Texture& texture = m_Textures[ tex ];

		uint32 width = (desc.m_Rect.m_Width == 0) ? desc.m_Data.m_Width : desc.m_Rect.m_Width;
		uint32 height = (desc.m_Rect.m_Height == 0) ? desc.m_Data.m_Height : desc.m_Rect.m_Height;

		width = std::clamp( width, 0u, texture.m_Rect.m_Width - desc.m_Rect.m_X );
		height = std::clamp( height, 0u, texture.m_Rect.m_Height - desc.m_Rect.m_Y );

		glTextureSubImage2D(
			texture.m_Handle,
			0,
			desc.m_Rect.m_X, desc.m_Rect.m_Y,
			width, height,
			sk_ImageFormatLookup[ LookupCast( texture.m_PixelLayout ) ],
			sk_TextureDataTypeLookup[ LookupCast( texture.m_PixelDataType ) ],
			resolve_pixel_data( desc.m_Data )
		);

		if (desc.m_GenerateMipmaps && MipmapLvls( width, height ) > 1)
			glGenerateTextureMipmap( texture.m_Handle );

	}

	void GLDevice::Delete( TextureHandle& texture ) {

		if (!IsValid( texture )) return;

		glDeleteTextures( 1, &m_Textures[ texture ].m_Handle );
		m_Textures.Remove( texture );

	}

	void GLDevice::BindTexture( TextureHandle texture, uint16 binding ) {

		LUM_ASSERT( binding < sk_MaxTextureUnits, "Invalid texture binding" );
		if (!IsValid( texture )) return;

		if (m_CurrentTextures[ binding ] == texture) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		m_CurrentTextures[ binding ] = texture;
		glBindTextureUnit( binding, m_Textures[ texture ].m_Handle );

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::GetTextureImage( TextureHandle texture, void* pixels ) {

		if (!IsValid( texture )) return;

		Texture& tex = m_Textures[ texture ];

		usize bufSize = tex.m_Rect.m_Width * tex.m_Rect.m_Height * 4;

		glGetTextureImage(
			static_cast<GLuint>(tex.m_Handle),
			0,
			sk_ImageFormatLookup[ ToUnderlyingEnum( tex.m_PixelLayout ) ], // <--- m_DataFormat (lub m_PixelLayout) zamias m_Format!
			sk_TextureDataTypeLookup[ ToUnderlyingEnum( tex.m_PixelDataType ) ],
			static_cast<GLsizei>(bufSize),
			pixels
		);

	}

	// -------------------------------------------------------------------------
	// Private creation helpers
	// -------------------------------------------------------------------------

	TextureHandle GLDevice::create_texture_2d( const TextureCreateInfo& desc ) {

		Texture texture;

		uint32 width = (desc.m_Width == 0) ? desc.m_Data.m_Width : desc.m_Width;
		uint32 height = (desc.m_Height == 0) ? desc.m_Data.m_Height : desc.m_Height;
		uint32 mipmapLevels = desc.m_GenerateMipmaps
			? (desc.m_MipmapLevels == 0 ? MipmapLvls( width, height ) : desc.m_MipmapLevels)
			: 1;

		if (desc.m_TextureType == TextureKind::Texture2D) {

			const void* data = resolve_pixel_data( desc.m_Data );

			glCreateTextures( GL_TEXTURE_2D, 1, &texture.m_Handle );
			glTextureStorage2D(
				texture.m_Handle,
				mipmapLevels,
				sk_ImageLayoutLookup[ LookupCast( desc.m_InternalFormat ) ],
				width, height
			);

			if (data != nullptr)
				glTextureSubImage2D(
					texture.m_Handle,
					0,
					0, 0,
					width, height,
					sk_ImageFormatLookup[ LookupCast( desc.m_PixelFormat ) ],
					sk_TextureDataTypeLookup[ LookupCast( desc.m_DataType ) ],
					data
				);

			if (mipmapLevels > 1)
				glGenerateTextureMipmap( texture.m_Handle );

		}
		else if (desc.m_TextureType == TextureKind::Texture2DMultiSampled) {

			glCreateTextures( GL_TEXTURE_2D_MULTISAMPLE, 1, &texture.m_Handle );
			glTextureStorage2DMultisample(
				texture.m_Handle,
				desc.m_Samples,
				sk_ImageLayoutLookup[ LookupCast( desc.m_InternalFormat ) ],
				width, height,
				GL_TRUE
			);

		}

		fill_texture_metadata( texture, desc, width, height, mipmapLevels );
		return m_Textures.Append( std::move( texture ) );

	}

	TextureHandle GLDevice::create_texture_2d_array( const TextureCreateInfo& desc ) {

		LUM_ASSERT( desc.m_Depth > 0, "Texture2DArray requires at least 1 layer (m_Depth)" );

		Texture texture;

		uint32 width = (desc.m_Width == 0) ? desc.m_Data.m_Width : desc.m_Width;
		uint32 height = (desc.m_Height == 0) ? desc.m_Data.m_Height : desc.m_Height;
		uint32 mipmapLevels = desc.m_GenerateMipmaps
			? (desc.m_MipmapLevels == 0 ? MipmapLvls( width, height ) : desc.m_MipmapLevels)
			: 1;

		glCreateTextures( GL_TEXTURE_2D_ARRAY, 1, &texture.m_Handle );
		glTextureStorage3D(
			texture.m_Handle,
			mipmapLevels,
			sk_ImageLayoutLookup[ LookupCast( desc.m_InternalFormat ) ],
			width, height,
			desc.m_Depth
		);

		if (mipmapLevels > 1)
			glGenerateTextureMipmap( texture.m_Handle );

		fill_texture_metadata( texture, desc, width, height, mipmapLevels );
		return m_Textures.Append( std::move( texture ) );

	}

	TextureHandle GLDevice::create_texture_3d( const TextureCreateInfo& ) {

		// TODO: implement
		return {};

	}

	TextureHandle GLDevice::create_texture_cubemap( const TextureCreateInfo& desc ) {

		Texture tex;

		glCreateTextures( GL_TEXTURE_CUBE_MAP, 1, &tex.m_Handle );

		bool bEmpty = desc.m_Cubemap.m_Faces[ 0 ].m_Width == 0;

		uint32 width = bEmpty ? desc.m_Width : SafeCast<uint32>(desc.m_Cubemap.m_Faces[ 0 ].m_Width);
		uint32 height = bEmpty ? desc.m_Height : SafeCast<uint32>(desc.m_Cubemap.m_Faces[ 0 ].m_Height);
		uint32 mipmapLevels = (desc.m_MipmapLevels > 0) ? desc.m_MipmapLevels : 1;

		glTextureStorage2D(
			tex.m_Handle,
			mipmapLevels,
			sk_ImageLayoutLookup[ LookupCast( desc.m_InternalFormat ) ],
			width, height
		);

		if (!bEmpty) {

			for (usize i = 0; i < 6; i++) {

				const ImageData& face = desc.m_Cubemap.m_Faces[ i ];
				const void* data = resolve_pixel_data( face );

				LUM_ASSERT(
					SafeCast<uint32>( face.m_Width ) == width &&
					SafeCast<uint32>( face.m_Height ) == height,
					"Cubemap face %zu has mismatched dimensions", i
				);

				glTextureSubImage3D(
					tex.m_Handle,
					0, 0, 0,
					static_cast<GLint>(i),
					face.m_Width, face.m_Height, 1,
					sk_ImageFormatLookup[ LookupCast( desc.m_PixelFormat ) ],
					sk_TextureDataTypeLookup[ LookupCast( desc.m_DataType ) ],
					data
				);

			}

		}

		fill_texture_metadata( tex, desc, width, height, mipmapLevels );
		return m_Textures.Append( std::move( tex ) );

	}

} // namespace lum::rhi::gl
