#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
///////////////////////////////////////////////////
/// Textures
///////////////////////////////////////////////////

	TextureHandle GLDevice::CreateTexture2D(const TextureDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.dense_size() <= skMaxTextures,
			TextureHandle{},
			"Max textures reached"
		);

		Texture texture;
		
		bool success;
		TextureData data = AssetService::load_texture(desc.filename, success);
		if (!success) {
			LUM_LOG_ERROR("Failed to load texture %s", desc.filename);
			return TextureHandle{};
		}

		uint32 width = (desc.width == 0) ? data.width : desc.width;
		uint32 height = (desc.height == 0) ? data.height : desc.height;

		texture.height = height;
		texture.width = width;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.glHandle);

		int32 mipmaps = mipmap_lvls(width, height);

		glTextureStorage2D(
			texture.handle.glHandle,
			mipmaps,
			GL_RGBA8,
			width,
			height
		);

		glTextureSubImage2D(
			texture.handle.glHandle,
			0,
			0,
			0,
			width,
			height,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data.pixels.data()
		);

		if (mipmaps > 1)
			glGenerateTextureMipmap(texture.handle.glHandle);

		auto textureHandle = mTextures.create_handle(std::move(texture));

		LUM_LOG_INFO("Created texture %d", textureHandle.id);

		return textureHandle;
	}
	// TO IMPLEMENT:
	TextureHandle GLDevice::CreateTexture3D(const TextureDescriptor& desc) {

		Texture texture;
		return mTextures.create_handle(std::move(texture));
	}
	TextureHandle GLDevice::CreateCubemapTexture(const TextureCubemapDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.dense_size() <= skMaxTextures,
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

		glTextureStorage2D(tex.handle.glHandle, 1, GL_RGBA8, texData.width, texData.height);

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

			glTextureSubImage3D(tex.handle.glHandle, 0, 0, 0, i, texture.width, texture.height, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture.pixels.data());
		
		}

		return mTextures.create_handle(std::move(tex));

	}
	void GLDevice::DeleteTexture(TextureHandle& texture) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(texture), "Texture %d doesn't exist", texture.id);

		glDeleteTextures(1, &mTextures[texture].handle.glHandle);

		mTextures.delete_handle(texture);

	}
	void GLDevice::SetTextureBinding(const TextureHandle& texture, uint16 binding) {

		LUM_HOTCHK_RETURN_VOID(mTextures.exist(texture), "Texture %d doesn't exist", texture.id);

		mTextures[texture].binding = binding;

	}
	void GLDevice::BindTexture(const TextureHandle& texture, uint16 binding) {
		LUM_HOTCHK_RETURN_VOID(mTextures.exist(texture), "Texture %d doesn't exist", texture.id);
		LUM_HOTCHK_RETURN_VOID(
			mTextures[texture].binding != LUM_NULL_BINDING &&
			binding == LUM_NULL_BINDING,
			"Binding has not been given to texture %d",
			texture.id
		);

		uint16 bind = (binding == LUM_NULL_BINDING) ? mTextures[texture].binding : binding;

		glBindTextureUnit(bind, mTextures[texture].handle.glHandle);

	}
}