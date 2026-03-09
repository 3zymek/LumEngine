//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL framebuffer operations (FBO)
//          Creation, texture attachment, binding, and clearing
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RFramebufferHandle GLDevice::CreateFramebuffer( const FFramebufferDescriptor& desc ) {

		LUM_ASSERT(mFramebuffers.DenseSize() <= skMaxFramebuffers, "Max framebuffers reached");

		FFramebuffer fbo;

		glCreateFramebuffers(1, &fbo.mHandle);

		std::vector<GLenum> drawBuffers;
		
		for (auto& [slot, texHandle] : desc.mColorTex) {
			
			const FTexture* tex = mTextures.Get(texHandle);
			
			glNamedFramebufferTexture(fbo.mHandle, GL_COLOR_ATTACHMENT0 + slot, tex->mHandle, 0);
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + slot);

		}

		glNamedFramebufferDrawBuffers(fbo.mHandle, drawBuffers.size(), drawBuffers.data());

		if (IsValid(desc.mDepthTex)) {
			
			const FTexture* tex = mTextures.Get(desc.mDepthTex);

			LUM_ASSERT(is_depth_format(tex->mInternalFormat), "Invalid framebuffer depth texture format");
			glNamedFramebufferTexture(fbo.mHandle, GL_DEPTH_ATTACHMENT, mTextures.Get(desc.mDepthTex)->mHandle, 0);
		}

		if (IsValid(desc.mStencilTex)) {

			const FTexture* tex = mTextures.Get(desc.mStencilTex);

			LUM_ASSERT(is_stencil_format(tex->mInternalFormat), "Invalid framebuffer stencil texture format");
			glNamedFramebufferTexture(fbo.mHandle, GL_STENCIL_ATTACHMENT, mTextures.Get(desc.mStencilTex)->mHandle, 0);

		}
	   
		LUM_ASSERT(
			glCheckNamedFramebufferStatus(fbo.mHandle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is not complete"
		);

		return mFramebuffers.Append(std::move(fbo));

	}

	void GLDevice::BlitFramebuffer( const FFramebufferBlitDescriptor& desc ) {

		FFramebuffer srcBuffer = mFramebuffers[desc.mSource];
		FFramebuffer dstBuffer = mFramebuffers[desc.mDestination];
		/*
		uint32 finalSrc = src.mID == NullID<RFramebufferID>() ? 0 : srcBuffer.mHandle;
		uint32 finalDst = dst.mID == NullID<RFramebufferID>() ? 0 : dstBuffer.mHandle;

		glBlitNamedFramebuffer(
			(GLuint)finalSrc, (GLuint)finalDst,

		);

	   */

	}

	void GLDevice::ClearFramebuffer( RFramebufferHandle fbo, ChannelRGBA color, float32 depth ) {
		BindFramebuffer(fbo);
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(std::clamp(depth, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void GLDevice::DeleteFramebuffer( RFramebufferHandle& buff ) {
		LUM_HOTCHK_RETURN_VOID(IsValid(buff), LUM_SEV_WARN, "Framebuffer doesn't exists");

		FFramebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.mHandle);

		mFramebuffers.Remove(buff);
	}

	void GLDevice::BindFramebuffer( RFramebufferHandle buff ) {

		LUM_HOTCHK_RETURN_VOID(IsValid(buff), LUM_SEV_WARN, "Invalid framebuffer");

		if (mCurrentFramebuffer == buff) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentFramebuffer = buff;

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffers[buff].mHandle);

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::UnbindFramebuffer( ) {

		if (mCurrentFramebuffer == RFramebufferHandle{}) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mCurrentFramebuffer = RFramebufferHandle{};

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		LUM_PROFILER_CACHE_MISS();

	}



	//==============================================
	// Framebuffer operations
	//==============================================

	void GLDevice::SetColorMask( bool r, bool g, bool b, bool a) {

		if (r == mColorMask.r &&
			g == mColorMask.g &&
			b == mColorMask.b &&
			a == mColorMask.a)
		{
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glColorMask(
			r ? GL_TRUE : GL_FALSE,
			g ? GL_TRUE : GL_FALSE,
			b ? GL_TRUE : GL_FALSE,
			a ? GL_TRUE : GL_FALSE
		);

		mColorMask.r = r;
		mColorMask.g = g;
		mColorMask.b = b;
		mColorMask.a = a;

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::SetColorMask(FColorMask rgba) {

		if (rgba.r == mColorMask.r &&
			rgba.g == mColorMask.g &&
			rgba.b == mColorMask.b &&
			rgba.a == mColorMask.a)
		{
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glColorMask(
			rgba.r ? GL_TRUE : GL_FALSE,
			rgba.g ? GL_TRUE : GL_FALSE,
			rgba.b ? GL_TRUE : GL_FALSE,
			rgba.a ? GL_TRUE : GL_FALSE
		);

		mColorMask.r = rgba.r;
		mColorMask.g = rgba.g;
		mColorMask.b = rgba.b;
		mColorMask.a = rgba.a;

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::SetClearColor(ChannelRGBA color) {

		if (mClearColor == color) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glClearColor(color.r, color.g, color.b, color.a);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearColor() {

		glClear(GL_COLOR_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearColor(ChannelRGBA color) {

		SetClearColor(color);
		glClear(GL_COLOR_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearDepth() {

		glClear(GL_DEPTH_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ClearStencil() {

		glClear(GL_STENCIL_BUFFER_BIT);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::Clear(Flags<BufferBit> flags) {

		GLbitfield mask = 0;

		mask |= (flags.Has(BufferBit::Color)) ? GL_COLOR_BUFFER_BIT : 0;
		mask |= (flags.Has(BufferBit::Depth)) ? GL_DEPTH_BUFFER_BIT : 0;
		mask |= (flags.Has(BufferBit::Stencil)) ? GL_STENCIL_BUFFER_BIT : 0;

		glClear(mask);

		LUM_PROFILER_CACHE_MISS();

	}

}