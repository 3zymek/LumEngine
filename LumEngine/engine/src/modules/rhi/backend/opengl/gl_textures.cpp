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
		texture.type = TextureType::Texture2D;

		uint32 mipmapLevels = 0;

		if (desc.data == nullptr) {

			if (!desc.filename) {
				LUM_LOG_WARN("No info gained about texture data or filename");
				return {};
			}

			bool success;
			TextureData data = AssetService::load_texture(desc.filename, success);
			if (!success) {
				LUM_LOG_ERROR("Something went wrong during loading texture: %s", desc.filename);
				return {};
			}

			uint32 width = (desc.width == 0) ? data.width : desc.width;
			uint32 height = (desc.height == 0) ? data.height : desc.height;
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

			uint32 width = desc.width;
			uint32 height = desc.height;
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
		texData = AssetService::load_texture(desc.faces[0], success);
		if (!success) {
			LUM_LOG_ERROR("Failed to load texture %s", desc.faces[0]);
		}

		int32 width = texData.width;
		int32 height = texData.height;

		glTextureStorage2D(tex.handle.glHandle, 1, GL_RGBA8, width, height);

		for (usize i = 0; i < 6; i++) {

			TextureData texture = AssetService::load_texture(desc.faces[i], success);
			
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
		auto data = AssetService::load_texture(desc.filename, success);

		if (!success) {
			LUM_LOG_ERROR("Failed to load image %s", desc.filename);
			return;
		}
		
		glTextureSubImage2D(
			mTextures[tex].handle.glHandle,
			0,
			0,
			0,
			data.width,
			data.height,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data.pixels.data()
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