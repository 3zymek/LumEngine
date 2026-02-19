//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL framebuffer operations (FBO)
//          Creation, texture attachment, binding, and clearing
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RFramebufferHandle GLDevice::CreateFramebuffer() {
		LUM_HOTCHK_RETURN_CUSTOM(
			mFramebuffers.dense_size() <= skMaxFramebuffers,
			LUM_SEV_ERROR,
			RFramebufferHandle{},
			"Max framebuffers reached"
		);

		RFramebuffer fbo;

		glCreateFramebuffers(1, &fbo.mHandle);

		return mFramebuffers.create_handle(std::move(fbo));

	}

	RTextureHandle GLDevice::CreateFramebufferTexture(const RFramebufferTextureDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.dense_size() <= skMaxTextures || desc.mHeight <= 0 || desc.mWidth <= 0,
			LUM_SEV_ERROR,
			RTextureHandle{},
			"Max textures reached"
		);

		RTexture tex;

		glCreateTextures(GL_TEXTURE_2D, 1, &tex.mHandle.gl);

		GLenum format = skInternalImageFormatLookup[lookup_cast(desc.mFormat)];
		glTextureStorage2D(tex.mHandle.gl, 1, format, desc.mWidth, desc.mHeight);

		return mTextures.create_handle(std::move(tex));
	}

	void GLDevice::SetFramebufferColorTexture(const RFramebufferHandle& fbo, const RTextureHandle& tex, int8 index) {

		LUM_HOTCHK_RETURN_VOID(mFramebuffers.exist(fbo), LUM_SEV_DEBUG, "Framebuffer doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_DEBUG, "Texture doesn't exist");
	   
		if (index > 31) {
			LUM_LOG_WARN("Too big color index given");
			return;
		}

		glNamedFramebufferTexture(mFramebuffers[fbo].mHandle, GL_COLOR_ATTACHMENT0, mTextures[tex].mHandle.gl, 0);

	}

	void GLDevice::SetFramebufferDepthTexture(const RFramebufferHandle& fbo, const RTextureHandle& tex) {

		LUM_HOTCHK_RETURN_VOID(mFramebuffers.exist(fbo), LUM_SEV_DEBUG, "Framebuffer doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_DEBUG, "Texture doesn't exist");

		glNamedFramebufferTexture(mFramebuffers[fbo].mHandle, GL_DEPTH_ATTACHMENT, mTextures[tex].mHandle.gl, 0);

	}

	void GLDevice::SetFramebufferStencilTexture(const RFramebufferHandle& fbo, const RTextureHandle& tex) {

		LUM_HOTCHK_RETURN_VOID(mFramebuffers.exist(fbo), LUM_SEV_DEBUG, "Framebuffer doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_DEBUG, "Texture doesn't exist");

		glNamedFramebufferTexture(mFramebuffers[fbo].mHandle, GL_STENCIL_ATTACHMENT, mTextures[tex].mHandle.gl, 0);

	}

	void GLDevice::ClearFramebuffer(RFramebufferHandle fbo, ChannelRGBA color, float32 depth) {
		BindFramebuffer(fbo);
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(std::clamp(depth, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void GLDevice::DeleteFramebuffer(RFramebufferHandle& buff) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.exist(buff), LUM_SEV_DEBUG, "Framebuffer doesn't exists");

		RFramebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.mHandle);

		mFramebuffers.delete_handle(buff);
	}

	void GLDevice::BindFramebuffer(const RFramebufferHandle& buff) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.exist(buff), LUM_SEV_DEBUG, "Framebuffer doesn't exists");

		if (mCurrentFramebuffer == buff) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentFramebuffer = buff;

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffers[buff].mHandle);

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::UnbindFramebuffer() {

		if (mCurrentFramebuffer == RFramebufferHandle{}) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentFramebuffer = RFramebufferHandle{};

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		LUM_PROFILER_CACHE_MISS();

	}
}