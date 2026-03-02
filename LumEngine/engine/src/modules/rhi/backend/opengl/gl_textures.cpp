//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL texture operations (2D, 3D, Cubemap)
//			creation, update, delete, bind
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RTextureHandle GLDevice::CreateTexture(const RTextureDescriptor& desc) {

		if (desc.mTextureType == RTextureType::None) {
			LUM_LOG_WARN("No texture type given");
			return {};
		}

		if (desc.mTextureType == RTextureType::Texture2D)
			return create_texture_2d(desc);

		else if (desc.mTextureType == RTextureType::Texture3D)
			return create_texture_3d(desc);

		else if (desc.mTextureType == RTextureType::Cubemap)
			return create_texture_cubemap(desc);

		return {};
	}
	void GLDevice::UnbindTexture(RTextureType type) {

		glBindTextureUnit(skTextureTypeLookup[LookupCast(type)], 0);

	}
	void GLDevice::UpdateTexture(const RTextureHandle& tex, const RTextureUpdateDescriptor& desc) {

		LUM_HOTCHK_RETURN_VOID(mTextures.Contains(tex), LUM_SEV_WARN, "Texture doesn't exist");

		LUM_HOTCHK_RETURN_VOID(
			desc.mData.mPixels.data() != nullptr,
			LUM_SEV_WARN,
			"Texture pixel data is null"
		);

		LUM_HOTCHK_RETURN_VOID(
			desc.mData.mWidth > 0 && desc.mData.mHeight > 0,
			LUM_SEV_WARN,
			"Invalid texture dimensions"
		);
		
		const auto& texture = mTextures[tex];

		uint32 width = (desc.mRect.mWidth == 0) ? desc.mData.mWidth : desc.mRect.mWidth;
		uint32 height = (desc.mRect.mHeight == 0) ? desc.mData.mHeight : desc.mRect.mHeight;

		width = std::clamp(width, 0u, texture.mRect.mWidth - desc.mRect.x);
		height = std::clamp(height, 0u, texture.mRect.mHeight - desc.mRect.y);


		glTextureSubImage2D(
			texture.mHandle,
			0,
			desc.mRect.x,
			desc.mRect.y,
			width,
			height,
			skImageFormatLookup[LookupCast(mTextures[tex].mDataFormat)],
			skTextureDataTypeLookup[LookupCast(mTextures[tex].mDataType)],
			desc.mData.mPixels.data()
		);

		uint32 mipmapLevels = desc.bGenerateMipmaps ? MipmapLvls(width, height) : 1;

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap(texture.mHandle);
		}

	}
	void GLDevice::DeleteTexture(RTextureHandle& texture) {

		LUM_HOTCHK_RETURN_VOID(mTextures.Contains(texture), LUM_SEV_WARN, "Texture doesn't exist");

		glDeleteTextures(1, &mTextures[texture].mHandle);

		mTextures.Remove(texture);

	}
	void GLDevice::BindTexture(const RTextureHandle& texture, uint16 binding) {

		LUM_HOTCHK_RETURN_VOID(mTextures.Contains(texture) && binding < skMaxTextureUnits, LUM_SEV_WARN, "Texture doesn't exist");

		if (mCurrentTextures[binding] == texture) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentTextures[binding] = texture;
		glBindTextureUnit(binding, mTextures[texture].mHandle);

		LUM_LOG_DEBUG("Binded texture %d to binding %d", texture.mID, binding);

		LUM_PROFILER_CACHE_MISS();

	}
	
	RTextureHandle GLDevice::create_texture_2d(const RTextureDescriptor& desc) {

		FTexture texture;

		uint32 width = (desc.mWidth == 0) ? desc.mData.mWidth : desc.mWidth;
		uint32 height = (desc.mHeight == 0) ? desc.mData.mHeight : desc.mHeight;
		uint32 mipmapLevels = desc.bGenerateMipmaps ?
			(desc.mMipmapLevels == 0 ? MipmapLvls(width, height) : desc.mMipmapLevels) : 1;

		cvptr data = (desc.mData.mPixels.empty()) ? nullptr : desc.mData.mPixels.data();

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.mHandle);

		glTextureStorage2D(
			texture.mHandle,
			mipmapLevels,
			skImageLayoutLookup[LookupCast(desc.mImageLayout)],
			width,
			height
		);

		glTextureSubImage2D(
			texture.mHandle,
			0,
			0, 0,
			width,
			height,
			skImageFormatLookup[LookupCast(desc.mImageFormat)],
			skTextureDataTypeLookup[LookupCast(desc.mDataType)],
			data
		);

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap(texture.mHandle);
		}

		texture.mDataFormat = desc.mImageFormat;
		texture.mDataType = desc.mDataType;
		texture.mInternalFormat = desc.mImageLayout;
		texture.mRect.mWidth = width;
		texture.mRect.mHeight = height;
		texture.mType = RTextureType::Texture2D;
		texture.mMipmapLevels = mipmapLevels;

		auto textureHandle = mTextures.Append(std::move(texture));

		LUM_LOG_INFO("Created texture 2D %d", textureHandle.mID);

		return textureHandle;

	}
	RTextureHandle GLDevice::create_texture_3d(const RTextureDescriptor&) {

		// TODO IMPLEMENT
		return {};

	}

	RTextureHandle GLDevice::create_texture_cubemap(const RTextureDescriptor& desc) {
	
		FTexture tex;

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &tex.mHandle);

		int32 width = desc.mCubemap.mFaces[0].mWidth;
		int32 height = desc.mCubemap.mFaces[0].mHeight;

		glTextureStorage2D(tex.mHandle, 1, skImageLayoutLookup[LookupCast(desc.mImageLayout)], width, height);

		for (usize i = 0; i < 6; i++) {

			auto texture = desc.mCubemap.mFaces[i];

			LUM_HOTCHK_RETURN_CUSTOM(
				!texture.mPixels.empty(),
				LUM_SEV_WARN,
				{},
				"Texture pixel data is null"
			);

			LUM_HOTCHK_RETURN_CUSTOM(
				texture.mWidth > 0 && texture.mHeight > 0,
				LUM_SEV_WARN,
				{},
				"Invalid texture dimensions"
			);

			if (texture.mWidth != width || texture.mHeight != height) {
				LUM_LOG_ERROR("Invalid cubemap height or width");
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
				skImageFormatLookup[LookupCast(desc.mImageFormat)],
				GL_UNSIGNED_BYTE,
				texture.mPixels.data()
			);

		}

		return mTextures.Append(std::move(tex));

	}

}