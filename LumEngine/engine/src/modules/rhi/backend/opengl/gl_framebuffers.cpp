//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL framebuffer operations (FBO)
//          Creation, texture attachment, binding, and clearing
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RFramebufferHandle GLDevice::CreateFramebuffer(const RFramebufferDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mFramebuffers.DenseSize() <= skMaxFramebuffers,
			LUM_SEV_ERROR,
			RFramebufferHandle{},
			"Max framebuffers reached"
		);

		FFramebuffer fbo;

		glCreateFramebuffers(1, &fbo.mHandle);

		for (uint8 i = 0; i < desc.mNumColorTex; i++) {

			if (mTextures.Contains(desc.mColorTex[i])) {
				
				const FTexture* tex = mTextures.Get(desc.mColorTex[i]);

				LUM_ASSERT(is_color_format(tex), "Invalid framebuffer color texture format");
				glNamedFramebufferTexture(fbo.mHandle, GL_COLOR_ATTACHMENT0 + i, tex->mHandle, 0);

			}
		}

		if (mTextures.Contains(desc.mDepthTex)) {
			
			const FTexture* tex = mTextures.Get(desc.mDepthTex);

			LUM_ASSERT(is_depth_format(tex), "Invalid framebuffer depth texture format");
			glNamedFramebufferTexture(fbo.mHandle, GL_DEPTH_ATTACHMENT, mTextures.Get(desc.mDepthTex)->mHandle, 0);
		}

		if (mTextures.Contains(desc.mStencilTex)) {

			const FTexture* tex = mTextures.Get(desc.mStencilTex);

			LUM_ASSERT(is_stencil_format(tex), "Invalid framebuffer stencil texture format");
			glNamedFramebufferTexture(fbo.mHandle, GL_STENCIL_ATTACHMENT, mTextures.Get(desc.mStencilTex)->mHandle, 0);

		}
	   
		LUM_ASSERT(
			glCheckNamedFramebufferStatus(fbo.mHandle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is not complete"
		);

		return mFramebuffers.Append(std::move(fbo));

	}

	void GLDevice::ClearFramebuffer(RFramebufferHandle fbo, ChannelRGBA color, float32 depth) {
		BindFramebuffer(fbo);
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(std::clamp(depth, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void GLDevice::DeleteFramebuffer(RFramebufferHandle& buff) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.Contains(buff), LUM_SEV_DEBUG, "Framebuffer doesn't exists");

		FFramebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.mHandle);

		mFramebuffers.Remove(buff);
	}

	void GLDevice::BindFramebuffer(const RFramebufferHandle& buff) {
		LUM_HOTCHK_RETURN_VOID(!mFramebuffers.Contains(buff), LUM_SEV_DEBUG, "Framebuffer doesn't exists");

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