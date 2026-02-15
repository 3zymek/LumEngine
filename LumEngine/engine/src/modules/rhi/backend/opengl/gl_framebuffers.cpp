#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
///////////////////////////////////////////////////
/// Framebuffers
///////////////////////////////////////////////////

	FramebufferHandle GLDevice::CreateFramebuffer() {
		LUM_HOTCHK_RETURN_CUSTOM(
			mFramebuffers.dense_size() >= skMaxFramebuffers,
			LUM_SEV_ERROR,
			FramebufferHandle{},
			"Max framebuffers reached"
		);

		Framebuffer fbo;

		glCreateFramebuffers(1, &fbo.mHandle);

		return mFramebuffers.create_handle(std::move(fbo));

	}

	TextureHandle GLDevice::CreateFramebufferTexture(const FramebufferTextureDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.dense_size() >= skMaxTextures || desc.mHeight <= 0 || desc.mWidth <= 0,
			LUM_SEV_ERROR,
			TextureHandle{},
			"Max textures reached"
		);

		Texture tex;

		glCreateTextures(GL_TEXTURE_2D, 1, &tex.mHandle.mGlHandle);

		GLenum format = (desc.mAttachment == FramebufferAttachment::DepthAttach) ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
		glTextureStorage2D(tex.mHandle.mGlHandle, 1, format, desc.mWidth, desc.mHeight);

		return mTextures.create_handle(std::move(tex));
	}

	void GLDevice::SetFramebufferColorTexture(const FramebufferHandle& fbo, const TextureHandle& tex, uint8 index) {

		LUM_HOTCHK_RETURN_VOID(mFramebuffers.exist(fbo), LUM_SEV_DEBUG, "Framebuffer doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_DEBUG, "Texture doesn't exist");

		if(index > GL_COLOR_ATTACHMENT0)

		glNamedFramebufferTexture(mFramebuffers[fbo].mHandle, GL_COLOR_ATTACHMENT0, mTextures[tex].mHandle.mGlHandle, 0);

	}

	void GLDevice::SetFramebufferDepthTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {

		LUM_HOTCHK_RETURN_VOID(mFramebuffers.exist(fbo), LUM_SEV_DEBUG, "Framebuffer doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_DEBUG, "Texture doesn't exist");

		glNamedFramebufferTexture(mFramebuffers[fbo].mHandle, GL_DEPTH_ATTACHMENT, mTextures[tex].mHandle.mGlHandle, 0);

	}

	void GLDevice::SetFramebufferStencilTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {

		LUM_HOTCHK_RETURN_VOID(mFramebuffers.exist(fbo), LUM_SEV_DEBUG, "Framebuffer doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mTextures.exist(tex), LUM_SEV_DEBUG, "Texture doesn't exist");

		glNamedFramebufferTexture(mFramebuffers[fbo].mHandle, GL_STENCIL_ATTACHMENT, mTextures[tex].mHandle.mGlHandle, 0);

	}

	void GLDevice::ClearFramebuffer(FramebufferHandle fbo, ChannelRGBA color, float32 depth) {
		BindFramebuffer(fbo);
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(std::clamp(depth, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLDevice::DeleteFramebuffer(FramebufferHandle& buff) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.exist(buff), LUM_SEV_DEBUG, "Framebuffer doesn't exists");

		Framebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.mHandle);

		mFramebuffers.delete_handle(buff);
	}

	void GLDevice::BindFramebuffer(const FramebufferHandle& buff) {
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

		if (mCurrentFramebuffer == FramebufferHandle{}) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentFramebuffer = FramebufferHandle{};

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		LUM_PROFILER_CACHE_MISS();

	}
}