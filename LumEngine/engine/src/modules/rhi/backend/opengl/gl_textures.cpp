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

		if (desc.data == nullptr) {

			if (!desc.filename) {
				LUM_LOG_WARN("No info gained about texture data or filename");
				return {};
			}

			bool success;
			TextureData data = AssetService::LoadTexture(desc.filename, success);
			if (!success) {
				LUM_LOG_ERROR("Something went wrong during loading texture: %s", desc.filename);
				return {};
			}

			width = (desc.rect.width == 0) ? data.width : desc.rect.width;
			height = (desc.rect.height == 0) ? data.height : desc.rect.height;
			if (desc.bGenerateMipmaps) {
				if (desc.mipmapLevels == 0) {
					mipmapLevels = mipmap_lvls(width, height);
				}
			}
			else {
				if (desc.mipmapLevels == 0) {
					mipmapLevels = 1;
				}
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.glHandle);

			glTextureStorage2D(
				texture.handle.glHandle,
				mipmapLevels,
				skInternalImageFormatLookup[lookup_cast(desc.internalFormat)],
				width,
				height
			);

			glTextureSubImage2D(
				texture.handle.glHandle,
				0,
				0, 0,
				width,
				height,
				skLoadedImageFormatLookup[lookup_cast(desc.dataFormat)],
				skTextureDataTypeLookup[lookup_cast(desc.dataType)],
				data.pixels.data()
			);

		}
		else {

			width = desc.rect.width;
			height = desc.rect.height;
			if (desc.bGenerateMipmaps) {
				if (desc.mipmapLevels == 0) {
					mipmapLevels = mipmap_lvls(width, height);
				}
			}
			else {
				if (desc.mipmapLevels == 0) {
					mipmapLevels = 1;
				}
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.glHandle);

			glTextureStorage2D(
				texture.handle.glHandle,
				mipmapLevels,
				skInternalImageFormatLookup[lookup_cast(desc.internalFormat)],
				width,
				height
			);

			glTextureSubImage2D(
				texture.handle.glHandle,
				0,
				0, 0,
				width,
				height,
				skLoadedImageFormatLookup[lookup_cast(desc.dataFormat)],
				skTextureDataTypeLookup[lookup_cast(desc.dataType)],
				desc.data
			);

		}

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap(texture.handle.glHandle);
		}

		texture.dataFormat = desc.dataFormat;
		texture.dataType = desc.dataType;
		texture.internalFormat = desc.internalFormat;
		texture.rect.width = width;
		texture.rect.height = height;
		texture.type = TextureType::Texture2D;
		texture.mipmapLevels = mipmapLevels;

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

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &tex.handle.glHandle);

		TextureData texData;
		bool success;
		texData = AssetService::LoadTexture(desc.faces[0], success);
		if (!success) {
			LUM_LOG_ERROR("Failed to load texture %s", desc.faces[0]);
		}

		int32 width = texData.width;
		int32 height = texData.height;

		glTextureStorage2D(tex.handle.glHandle, 1, GL_RGBA8, width, height);

		for (usize i = 0; i < 6; i++) {

			TextureData texture = AssetService::LoadTexture(desc.faces[i], success);
			
			if (!success) {
				LUM_LOG_ERROR("Failed to load texture %s", desc.faces[i]);
				continue;
			}
			
			if (texture.width != width || texture.height != height) {
				LUM_LOG_ERROR("Invalid cubemap height or width");
				continue;
			}

			glTextureSubImage3D(
				tex.handle.glHandle, 
				0, 
				0, 
				0, 
				i, 
				texture.width, 
				texture.height, 
				1, 
				GL_RGBA, 
				GL_UNSIGNED_BYTE, 
				texture.pixels.data()
			);
		
		}

		return mTextures.create_handle(std::move(tex));

	}
	void GLDevice::UpdateTexture(const TextureHandle& tex, const TextureDescriptor& desc) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_WARN, "Texture doesn't exist");

		bool success;
		auto data = AssetService::LoadTexture(desc.filename, success);

		if (!success) {
			LUM_LOG_ERROR("Failed to load image %s", desc.filename);
			return;
		}
		
		const auto& texture = mTextures[tex];

		uint32 width = (desc.rect.width == 0) ? data.width : desc.rect.width;
		uint32 height = (desc.rect.height == 0) ? data.height : desc.rect.height;

		width = std::clamp(width, 0u, texture.rect.width - desc.rect.x);
		height = std::clamp(height, 0u, texture.rect.height - desc.rect.y);


		glTextureSubImage2D(
			texture.handle.glHandle,
			0,
			desc.rect.x,
			desc.rect.y,
			width,
			height,
			skLoadedImageFormatLookup[lookup_cast(mTextures[tex].dataFormat)],
			skTextureDataTypeLookup[lookup_cast(mTextures[tex].dataType)],
			data.pixels.data()
		);

		uint32 mipmapLevels = 0;

		if (desc.bGenerateMipmaps) {
			if (desc.mipmapLevels == 0) {
				mipmapLevels = mipmap_lvls(texture.rect.width, texture.rect.height);
			}
		}
		else {
			if (desc.mipmapLevels == 0) {
				mipmapLevels = 1;
			}
		}

		if (mipmapLevels > 1) {
			glGenerateTextureMipmap(texture.handle.glHandle);
		}

	}
	void GLDevice::UpdateTexture(const TextureHandle& tex, const TextureRect& rect, const void* data) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_WARN, "Texture doesn't exist");

		if (!data) {
			LUM_LOG_ERROR("Given texture data pointer is nullptr");
			return;
		}

		glTextureSubImage2D(
			mTextures[tex].handle.glHandle,
			rect.mip,
			rect.x,
			rect.y,
			rect.width,
			rect.height,
			skLoadedImageFormatLookup[lookup_cast(mTextures[tex].dataFormat)],
			skTextureDataTypeLookup[lookup_cast(mTextures[tex].dataType)],
			data
		);

	}
	void GLDevice::DeleteTexture(TextureHandle& texture) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(texture), LUM_SEV_WARN, "Texture doesn't exist");

		glDeleteTextures(1, &mTextures[texture].handle.glHandle);

		mTextures.delete_handle(texture);

	}
	void GLDevice::BindTexture(const TextureHandle& texture, uint16 binding) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(texture) && binding < MAX_TEXTURE_UNITS, LUM_SEV_WARN, "Texture doesn't exist");

		if (mCurrentTextures[binding] == texture) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentTextures[binding] = texture;
		glBindTextureUnit(binding, mTextures[texture].handle.glHandle);

		LUM_LOG_DEBUG("Binded texture %d to binding %d", texture.id, binding);

		LUM_PROFILER_CACHE_MISS();

	}
}