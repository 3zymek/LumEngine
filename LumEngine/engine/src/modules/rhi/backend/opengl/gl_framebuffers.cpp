//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL framebuffer operations (FBO)
//          Creation, texture attachment, binding, and clearing
//
//=============================================================================//

#include "modules/rhi/backend/gl_device.hpp"

namespace lum::rhi::gl {

	RFramebufferHandle GLDevice::CreateFramebuffer( const FFramebufferDescriptor& desc ) {

		LUM_ASSERT( mFramebuffers.DenseSize( ) <= skMaxFramebuffers, "Max framebuffers reached" );

		FFramebuffer fbo;

		glCreateFramebuffers( 1, &fbo.mHandle );

		std::vector<GLenum> drawBuffers;

		uint32 samples = 0;

		for (auto& [slot, texHandle] : desc.mColorTex) {

			const FTexture* tex = mTextures.Get( texHandle );

			if (samples == 0)
				samples = tex->mSamples;
			else
				LUM_ASSERT( tex->mSamples == samples, "Sample count mismatch" );

			glNamedFramebufferTexture(
				fbo.mHandle,
				GL_COLOR_ATTACHMENT0 + slot,
				tex->mHandle,
				0
			);
			drawBuffers.push_back( GL_COLOR_ATTACHMENT0 + slot );

		}


		if (drawBuffers.empty( )) {
			GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
			glNamedFramebufferDrawBuffers( fbo.mHandle, 1, &drawBuffer );
		}
		else {
			glNamedFramebufferDrawBuffers( fbo.mHandle, drawBuffers.size( ), drawBuffers.data( ) );
		}


		if (IsValid( desc.mDepthTex )) {

			const FTexture* tex = mTextures.Get( desc.mDepthTex );

			LUM_ASSERT( is_depth_format( tex->mInternalFormat ), "Invalid framebuffer depth texture format" );
			LUM_ASSERT( tex->mSamples == samples, "Depth sample mismatch" );
			glNamedFramebufferTexture( fbo.mHandle, GL_DEPTH_ATTACHMENT, mTextures.Get( desc.mDepthTex )->mHandle, 0 );
		}

		if (IsValid( desc.mStencilTex )) {

			const FTexture* tex = mTextures.Get( desc.mStencilTex );

			LUM_ASSERT( is_stencil_format( tex->mInternalFormat ), "Invalid framebuffer stencil texture format" );
			glNamedFramebufferTexture( fbo.mHandle, GL_STENCIL_ATTACHMENT, mTextures.Get( desc.mStencilTex )->mHandle, 0 );

		}

		if (!desc.mColorTex.empty( ) || IsValid( desc.mDepthTex ) || IsValid( desc.mStencilTex )) {
			LUM_ASSERT(
				glCheckNamedFramebufferStatus( fbo.mHandle, GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE,
				"Framebuffer is not complete"
			);
		}

		return mFramebuffers.Append( std::move( fbo ) );

	}

	void GLDevice::AttachCubemapFace( RFramebufferHandle fbo, RTextureHandle tex, int32 face, uint32 mip ) {

		LUM_ASSERT( IsValid( fbo ), "Invalid framebuffer" );
		LUM_ASSERT( IsValid( tex ), "Invalid texture" );


		glNamedFramebufferTextureLayer( 
			mFramebuffers[fbo ].mHandle, 
			GL_COLOR_ATTACHMENT0, 
			mTextures[tex ].mHandle, 
			mip, 
			face 
		);

	}

	void GLDevice::BlitFramebuffer( const FFramebufferBlitDescriptor& desc ) {

		LUM_ASSERT( validate_framebuffer_handle( desc.mDestination ), "Invalid destination framebuffer" );
		LUM_ASSERT( validate_framebuffer_handle( desc.mSource ), "Invalid source framebuffer handle" );
		LUM_ASSERT(
			!(desc.mCopyMask.Has( BufferBit::Depth ) || desc.mCopyMask.Has( BufferBit::Stencil ))
			|| desc.mFilter == SamplerMagFilter::Nearest,
			"Depth and stencil blit requires Nearest filter"
		);

		uint32 srcX1 = desc.mSrcX1 == 0 ? mViewportState.mWidth : desc.mSrcX1;
		uint32 srcY1 = desc.mSrcY1 == 0 ? mViewportState.mHeight : desc.mSrcY1;
		uint32 dstX1 = desc.mDstX1 == 0 ? mViewportState.mWidth : desc.mDstX1;
		uint32 dstY1 = desc.mDstY1 == 0 ? mViewportState.mHeight : desc.mDstY1;

		GLuint dstID = IsValid( desc.mDestination ) ? mFramebuffers[ desc.mDestination ].mHandle : 0;
		GLuint srcID = IsValid( desc.mSource ) ? mFramebuffers[ desc.mSource ].mHandle : 0;

		GLbitfield mask = 0;

		mask |= (desc.mCopyMask.Has( BufferBit::Color )) ? GL_COLOR_BUFFER_BIT : 0;
		mask |= (desc.mCopyMask.Has( BufferBit::Depth )) ? GL_DEPTH_BUFFER_BIT : 0;
		mask |= (desc.mCopyMask.Has( BufferBit::Stencil )) ? GL_STENCIL_BUFFER_BIT : 0;

		glBlitNamedFramebuffer(
			srcID, dstID,
			desc.mSrcX0, desc.mSrcY0, srcX1, srcY1,
			desc.mDstX0, desc.mDstY0, dstX1, dstY1,
			mask,
			desc.mFilter == SamplerMagFilter::Linear ? GL_LINEAR : GL_NEAREST
		);

	}

	void GLDevice::ClearFramebuffer( RFramebufferHandle fbo, ChannelRGBA color, float32 depth ) {

		BindFramebuffer( fbo );

		glClearColor( color.r, color.g, color.b, color.a );
		glClearDepth( std::clamp( depth, 0.0f, 1.0f ) );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	}

	void GLDevice::DeleteFramebuffer( RFramebufferHandle& buff ) {

		LUM_HOTCHK_RETURN_VOID( IsValid( buff ), LUM_SEV_WARN, "Invalid framebuffer" );

		FFramebuffer& fbo = mFramebuffers[ buff ];
		glDeleteFramebuffers( 1, &fbo.mHandle );

		mFramebuffers.Remove( buff );
	}

	void GLDevice::BindFramebuffer( RFramebufferHandle buff ) {

		LUM_ASSERT( validate_framebuffer_handle( buff ), "Invalid framebuffer" );

		if (mCurrentFramebuffer == buff) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		mCurrentFramebuffer = buff;

		GLuint handle = IsValid( buff ) ? mFramebuffers[ buff ].mHandle : 0;
		glBindFramebuffer( GL_FRAMEBUFFER, handle );

		LUM_PROFILER_CACHE_MISS( );

	}



	//==============================================
	// Framebuffer operations
	//==============================================

	void GLDevice::SetColorMask( bool r, bool g, bool b, bool a ) {

		if (r == mColorMask.r &&
			 g == mColorMask.g &&
			 b == mColorMask.b &&
			 a == mColorMask.a) {
			LUM_PROFILER_CACHE_HIT( );
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

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::SetColorMask( FColorMask rgba ) {

		if (rgba.r == mColorMask.r &&
			 rgba.g == mColorMask.g &&
			 rgba.b == mColorMask.b &&
			 rgba.a == mColorMask.a) {
			LUM_PROFILER_CACHE_HIT( );
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

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::SetClearColor( ChannelRGBA color ) {

		if (mClearColor == color) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glClearColor( color.r, color.g, color.b, color.a );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ClearColor( ) {

		glClear( GL_COLOR_BUFFER_BIT );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ClearColor( ChannelRGBA color ) {

		SetClearColor( color );
		glClear( GL_COLOR_BUFFER_BIT );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ClearDepth( ) {

		glClear( GL_DEPTH_BUFFER_BIT );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ClearStencil( ) {

		glClear( GL_STENCIL_BUFFER_BIT );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::Clear( Flags<BufferBit> flags ) {

		GLbitfield mask = 0;

		mask |= (flags.Has( BufferBit::Color )) ? GL_COLOR_BUFFER_BIT : 0;
		mask |= (flags.Has( BufferBit::Depth )) ? GL_DEPTH_BUFFER_BIT : 0;
		mask |= (flags.Has( BufferBit::Stencil )) ? GL_STENCIL_BUFFER_BIT : 0;

		glClear( mask );

		LUM_PROFILER_CACHE_MISS( );

	}

}