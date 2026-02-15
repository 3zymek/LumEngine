#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	TextureHandle GLDevice::CreateTexture2D ( const TextureDescriptor& desc ) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.dense_size() <= skMaxTextures,
			LUM_SEV_WARN,
			TextureHandle{},
			"Max textures reached"
		);

		Texture texture;

		uint32 mipmapLevels = 0;
		uint32 width = 0;
		uint32 height = 0;

		if (desc.mData == nullptr) {

			if (!desc.mFilename) {
				LUM_LOG_WARN("No info gained about texture data or filename");
				return {};
			}

			bool success;
			TextureData data = AssetService::LoadTexture(desc.mFilename, success);
			if (!success) {
				LUM_LOG_ERROR("Something went wrong during loading texture: %s", desc.mFilename);
				return {};
			}

			width = (desc.mRect.mWidth == 0) ? data.mWidth : desc.mRect.mWidth;
			height = (desc.mRect.mHeight == 0) ? data.mHeight : desc.mRect.mHeight;
			if (desc.bGenerateMipmaps) {
				if (desc.mMipmapLevels == 0) {
					mipmapLevels = mipmap_lvls(width, height);
				}
			}
			else {
				if (desc.mMipmapLevels == 0) {
					mipmapLevels = 1;
				}
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &texture.mHandle.mGlHandle);

			glTextureStorage2D(
				texture.mHandle.mGlHandle,
				mipmapLevels,
				skInternalImageFormatLookup[lookup_cast(desc.mInternalFormat)],
				width,
				height
			);

			glTextureSubImage2D(
				texture.mHandle.mGlHandle,
				0,
				0, 0,
				width,
				height,
				skLoadedImageFormatLookup[lookup_cast(desc.mDataFormat)],
				skTextureDataTypeLookup[lookup_cast(desc.mDataType)],
				data.mPixels.data()
			);

		}
		else {

			width = desc.mRect.mWidth;
			height = desc.mRect.mHeight;
			if (desc.bGenerateMipmaps) {
				if (desc.mMipmapLevels == 0) {
					mipmapLevels = mipmap_lvls(width, height);
				}
			}
			else {
				if (desc.mMipmapLevels == 0) {
					mipmapLevels = 1;
				}
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &texture.mHandle.mGlHandle);

			glTextureStorage2D(
				texture.mHandle.mGlHandle,
				mipmapLevels,
				skInternalImageFormatLookup[lookup_cast(desc.mInternalFormat)],
				width,
				height
			);

			glTextureSubImage2D(
				texture.mHandle.mGlHandle,
				0,
				0, 0,
				width,
				height,
				skLoadedImageFormatLookup[lookup_cast(desc.mDataFormat)],
				skTextureDataTypeLookup[lookup_cast(desc.mDataType)],
				desc.mData
			);

		}

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap(texture.mHandle.mGlHandle);
		}

		texture.mDataFormat = desc.mDataFormat;
		texture.mDataType = desc.mDataType;
		texture.mInternalFormat = desc.mInternalFormat;
		texture.mRect.mWidth = width;
		texture.mRect.mHeight = height;
		texture.mType = TextureType::Texture2D;
		texture.mMipmapLevels = mipmapLevels;

		auto textureHandle = mTextures.create_handle(std::move(texture));

		LUM_LOG_INFO("Created texture %d", textureHandle.id);

		return textureHandle;
	}
	// TO IMPLEMENT:
	TextureHandle GLDevice::CreateTexture3D ( const TextureDescriptor& desc ) {

		Texture texture;
		return mTextures.create_handle(std::move(texture));
	}
	TextureHandle GLDevice::CreateCubemapTexture ( const TextureCubemapDescriptor& desc ) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.dense_size() <= skMaxTextures,
			LUM_SEV_WARN,
			TextureHandle{},
			"Max textures reached"
		);

		Texture tex;

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &tex.mHandle.mGlHandle);

		TextureData texData;
		bool success;
		texData = AssetService::LoadTexture(desc.mFaces[0], success);
		if (!success) {
			LUM_LOG_ERROR("Failed to load texture %s", desc.mFaces[0]);
		}

		int32 width = texData.mWidth;
		int32 height = texData.mHeight;

		glTextureStorage2D(tex.mHandle.mGlHandle, 1, GL_RGBA8, width, height);

		for (usize i = 0; i < 6; i++) {

			TextureData texture = AssetService::LoadTexture(desc.mFaces[i], success);
			
			if (!success) {
				LUM_LOG_ERROR("Failed to load texture %s", desc.mFaces[i]);
				continue;
			}
			
			if (texture.mWidth != width || texture.mHeight != height) {
				LUM_LOG_ERROR("Invalid cubemap height or width");
				continue;
			}

			glTextureSubImage3D(
				tex.mHandle.mGlHandle, 
				0, 
				0, 
				0, 
				i, 
				texture.mWidth, 
				texture.mHeight, 
				1, 
				GL_RGBA, 
				GL_UNSIGNED_BYTE, 
				texture.mPixels.data()
			);
		
		}

		return mTextures.create_handle(std::move(tex));

	}
	void GLDevice::UpdateTexture(const TextureHandle& tex, const TextureDescriptor& desc) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_WARN, "Texture doesn't exist");

		bool success;
		auto data = AssetService::LoadTexture(desc.mFilename, success);

		if (!success) {
			LUM_LOG_ERROR("Failed to load image %s", desc.mFilename);
			return;
		}
		
		const auto& texture = mTextures[tex];

		uint32 width = (desc.mRect.mWidth == 0) ? data.mWidth : desc.mRect.mWidth;
		uint32 height = (desc.mRect.mHeight == 0) ? data.mHeight : desc.mRect.mHeight;

		width = std::clamp(width, 0u, texture.mRect.mWidth - desc.mRect.x);
		height = std::clamp(height, 0u, texture.mRect.mHeight - desc.mRect.y);


		glTextureSubImage2D(
			texture.mHandle.mGlHandle,
			0,
			desc.mRect.x,
			desc.mRect.y,
			width,
			height,
			skLoadedImageFormatLookup[lookup_cast(mTextures[tex].mDataFormat)],
			skTextureDataTypeLookup[lookup_cast(mTextures[tex].mDataType)],
			data.mPixels.data()
		);

		uint32 mipmapLevels = 0;

		if (desc.bGenerateMipmaps) {
			if (desc.mMipmapLevels == 0) {
				mipmapLevels = mipmap_lvls(texture.mRect.mWidth, texture.mRect.mHeight);
			}
		}
		else {
			if (desc.mMipmapLevels == 0) {
				mipmapLevels = 1;
			}
		}

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap(texture.mHandle.mGlHandle);
		}

	}
	void GLDevice::UpdateTexture(const TextureHandle& tex, const TextureRect& rect, const void* data) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_WARN, "Texture doesn't exist");

		if (!data) {
			LUM_LOG_ERROR("Given texture data pointer is nullptr");
			return;
		}

		glTextureSubImage2D(
			mTextures[tex].mHandle.mGlHandle,
			rect.mMipLevel,
			rect.x,
			rect.y,
			rect.mWidth,
			rect.mHeight,
			skLoadedImageFormatLookup[lookup_cast(mTextures[tex].mDataFormat)],
			skTextureDataTypeLookup[lookup_cast(mTextures[tex].mDataType)],
			data
		);

	}
	void GLDevice::DeleteTexture(TextureHandle& texture) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(texture), LUM_SEV_WARN, "Texture doesn't exist");

		glDeleteTextures(1, &mTextures[texture].mHandle.mGlHandle);

		mTextures.delete_handle(texture);

	}
	void GLDevice::BindTexture(const TextureHandle& texture, uint16 binding) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(texture) && binding < MAX_TEXTURE_UNITS, LUM_SEV_WARN, "Texture doesn't exist");

		if (mCurrentTextures[binding] == texture) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentTextures[binding] = texture;
		glBindTextureUnit(binding, mTextures[texture].mHandle.mGlHandle);

		LUM_LOG_DEBUG("Binded texture %d to binding %d", texture.id, binding);

		LUM_PROFILER_CACHE_MISS();

	}
}