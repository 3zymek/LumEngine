#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
///////////////////////////////////////////////////
/// Framebuffers
///////////////////////////////////////////////////

	FramebufferHandle GLDevice::CreateFramebuffer() {
		LUM_HOTCHK_RETURN_CUSTOM(
			mFramebuffers.dense_size() >= skMaxFramebuffers,
			"Max framebuffers reached",
			FramebufferHandle{}
		);

		Framebuffer fbo;

		glCreateFramebuffers(1, &fbo.handle);

		return mFramebuffers.create_handle(std::move(fbo));

	}

	TextureHandle GLDevice::CreateFramebufferTexture(const FramebufferTextureDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mTextures.dense_size() >= skMaxTextures || desc.height <= 0 || desc.width <= 0,
			"Max textures reached",
			TextureHandle{}
		);

		Texture tex;

		glCreateTextures(GL_TEXTURE_2D, 1, &tex.handle.glHandle);

		GLenum format = (desc.attachment == FramebufferAttachment::DepthAttach) ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
		glTextureStorage2D(tex.handle.glHandle, 1, format, desc.width, desc.height);

		return mTextures.create_handle(std::move(tex));
	}

	void GLDevice::SetFramebufferColorTexture(const FramebufferHandle& fbo, const TextureHandle& tex, uint8 index) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.exist(fbo) || !mTextures.exist(tex), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextures[tex].handle.glHandle, 0);

	}

	void GLDevice::SetFramebufferDepthTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.exist(fbo), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextures[tex].handle.glHandle, 0);

	}

	void GLDevice::SetFramebufferStencilTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {

	}

	void GLDevice::ClearFramebuffer(FramebufferHandle fbo, glm::vec4 color, float32 depth) {
		BindFramebuffer(fbo);
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(std::clamp(depth, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLDevice::DeleteFramebuffer(FramebufferHandle& buff) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.exist(buff), "Framebuffer doesn't exists");

		Framebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.handle);

		mFramebuffers.delete_handle(buff);
	}

	void GLDevice::BindFramebuffer(const FramebufferHandle& buff) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.exist(buff), "Framebuffer doesn't exists");

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffers[buff].handle);

	}

	void GLDevice::UnbindFramebuffer() {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}