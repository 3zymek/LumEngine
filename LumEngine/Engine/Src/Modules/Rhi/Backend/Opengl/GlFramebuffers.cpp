//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL framebuffer operations (FBO)
//          Creation, texture attachment, binding, and clearing
//
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	FramebufferHandle GLDevice::CreateFramebuffer( const FramebufferCreateInfo& desc ) {

		LUM_ASSERT( m_Framebuffers.DenseSize( ) <= skMaxFramebuffers, "Max framebuffers reached" );

		Framebuffer fbo;
		glCreateFramebuffers( 1, &fbo.m_Handle );

		std::vector<GLenum> drawBuffers;
		uint32 samples = 0;

		for (auto& [slot, texHandle] : desc.m_ColorTex) {

			const Texture* tex = m_Textures.Get( texHandle );

			if (samples == 0)
				samples = tex->m_SampleCount;
			else
				LUM_ASSERT( tex->m_SampleCount == samples, "Sample count mismatch" );

			glNamedFramebufferTexture(
				fbo.m_Handle,
				GL_COLOR_ATTACHMENT0 + slot,
				tex->m_Handle,
				0
			);
			drawBuffers.push_back( GL_COLOR_ATTACHMENT0 + slot );

		}


		if (drawBuffers.empty( )) {
			GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
			glNamedFramebufferDrawBuffers( fbo.m_Handle, 1, &drawBuffer );
		}
		else {
			glNamedFramebufferDrawBuffers( fbo.m_Handle, drawBuffers.size( ), drawBuffers.data( ) );
		}


		if (IsValid( desc.m_DepthTex )) {

			const Texture* tex = m_Textures.Get( desc.m_DepthTex );

			LUM_ASSERT( is_depth_format( tex->m_Format ), "Invalid framebuffer depth texture format" );
			LUM_ASSERT( tex->m_SampleCount == samples, "Depth sample mismatch" );
			glNamedFramebufferTexture( fbo.m_Handle, GL_DEPTH_ATTACHMENT, m_Textures.Get( desc.m_DepthTex )->m_Handle, 0 );
		}

		if (IsValid( desc.m_StencilTex )) {

			const Texture* tex = m_Textures.Get( desc.m_StencilTex );

			LUM_ASSERT( is_stencil_format( tex->m_Format ), "Invalid framebuffer stencil texture format" );
			glNamedFramebufferTexture( fbo.m_Handle, GL_STENCIL_ATTACHMENT, m_Textures.Get( desc.m_StencilTex )->m_Handle, 0 );

		}

		if (!desc.m_ColorTex.empty( ) || IsValid( desc.m_DepthTex ) || IsValid( desc.m_StencilTex )) {
			LUM_ASSERT(
				glCheckNamedFramebufferStatus( fbo.m_Handle, GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE,
				"Framebuffer is not complete"
			);
		}

		return m_Framebuffers.Append( std::move( fbo ) );

	}

	void GLDevice::AttachTextureLayer( rhi::FramebufferHandle fbo, rhi::TextureHandle tex, const TextureLayerAttachment& desc ) {

		LUM_ASSERT( IsValid( fbo ), "Invalid framebuffer" );
		LUM_ASSERT( IsValid( tex ), "Invalid texture" );

		GLenum attachment;
		if (desc.m_Attachment.Has( BufferBit::Depth ) && desc.m_Attachment.Has( BufferBit::Stencil ))
			attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		else if (desc.m_Attachment.Has( BufferBit::Depth ))
			attachment = GL_DEPTH_ATTACHMENT;
		else if (desc.m_Attachment.Has( BufferBit::Stencil ))
			attachment = GL_STENCIL_ATTACHMENT;
		else
			attachment = GL_COLOR_ATTACHMENT0 + desc.m_Slot;

		glNamedFramebufferTextureLayer(
			m_Framebuffers[fbo ].m_Handle,
			attachment,
			m_Textures[tex ].m_Handle,
			desc.m_Mip,
			desc.m_Layer
		);

	}

	void GLDevice::BlitFramebuffer( const FramebufferBlitDescription& desc ) {

		LUM_ASSERT( validate_framebuffer_handle( desc.m_Destination ), "Invalid destination framebuffer" );
		LUM_ASSERT( validate_framebuffer_handle( desc.m_Source ), "Invalid source framebuffer handle" );
		LUM_ASSERT(
			!(desc.m_CopyMask.Has( BufferBit::Depth ) || desc.m_CopyMask.Has( BufferBit::Stencil ))
			|| desc.m_Filter == SamplerMagFilter::Nearest,
			"Depth and stencil blit requires Nearest filter"
		);

		uint32 srcX1 = desc.m_SrcX1 == 0 ? m_ViewportState.m_Width : desc.m_SrcX1;
		uint32 srcY1 = desc.m_SrcY1 == 0 ? m_ViewportState.m_Height : desc.m_SrcY1;
		uint32 dstX1 = desc.m_DstX1 == 0 ? m_ViewportState.m_Width : desc.m_DstX1;
		uint32 dstY1 = desc.m_DstY1 == 0 ? m_ViewportState.m_Height : desc.m_DstY1;

		GLuint dstID = IsValid( desc.m_Destination ) ? m_Framebuffers[ desc.m_Destination ].m_Handle : 0;
		GLuint srcID = IsValid( desc.m_Source ) ? m_Framebuffers[ desc.m_Source ].m_Handle : 0;

		GLbitfield mask = 0;

		mask |= (desc.m_CopyMask.Has( BufferBit::Color )) ? GL_COLOR_BUFFER_BIT : 0;
		mask |= (desc.m_CopyMask.Has( BufferBit::Depth )) ? GL_DEPTH_BUFFER_BIT : 0;
		mask |= (desc.m_CopyMask.Has( BufferBit::Stencil )) ? GL_STENCIL_BUFFER_BIT : 0;

		glBlitNamedFramebuffer(
			srcID, dstID,
			desc.m_SrcX0, desc.m_SrcY0, srcX1, srcY1,
			desc.m_DstX0, desc.m_DstY0, dstX1, dstY1,
			mask,
			desc.m_Filter == SamplerMagFilter::Linear ? GL_LINEAR : GL_NEAREST
		);

	}

	void GLDevice::ClearFramebuffer( FramebufferHandle fbo, Vector4 color, float32 depth ) {

		BindFramebuffer( fbo );

		glClearColor( color.mR, color.mG, color.mB, color.mA );
		glClearDepth( std::clamp( depth, 0.0f, 1.0f ) );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	}

	void GLDevice::Delete( FramebufferHandle& buff ) {

		if (!IsValid( buff )) return;

		Framebuffer& fbo = m_Framebuffers[ buff ];
		glDeleteFramebuffers( 1, &fbo.m_Handle );

		m_Framebuffers.Remove( buff );
	}

	void GLDevice::BindFramebuffer( FramebufferHandle buff ) {

		LUM_ASSERT( validate_framebuffer_handle( buff ), "Invalid framebuffer" );

		if (m_CurrentFramebuffer == buff) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		m_CurrentFramebuffer = buff;

		GLuint handle = IsValid( buff ) ? m_Framebuffers[ buff ].m_Handle : 0;
		glBindFramebuffer( GL_FRAMEBUFFER, handle );

		LUM_PROFILER_CACHE_MISS( );

	}



	//==============================================
	// Framebuffer operations
	//==============================================

	void GLDevice::SetColorMask( bool r, bool g, bool b, bool a ) {

		if (r == m_ColorMask.mR &&
			 g == m_ColorMask.mG &&
			 b == m_ColorMask.mB &&
			 a == m_ColorMask.mA) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glColorMask(
			r ? GL_TRUE : GL_FALSE,
			g ? GL_TRUE : GL_FALSE,
			b ? GL_TRUE : GL_FALSE,
			a ? GL_TRUE : GL_FALSE
		);

		m_ColorMask.mR = r;
		m_ColorMask.mG = g;
		m_ColorMask.mB = b;
		m_ColorMask.mA = a;

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::SetColorMask( ColorMask rgba ) {

		if (rgba.mR == m_ColorMask.mR &&
			 rgba.mG == m_ColorMask.mG &&
			 rgba.mB == m_ColorMask.mB &&
			 rgba.mA == m_ColorMask.mA) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glColorMask(
			rgba.mR ? GL_TRUE : GL_FALSE,
			rgba.mG ? GL_TRUE : GL_FALSE,
			rgba.mB ? GL_TRUE : GL_FALSE,
			rgba.mA ? GL_TRUE : GL_FALSE
		);

		m_ColorMask.mR = rgba.mR;
		m_ColorMask.mG = rgba.mG;
		m_ColorMask.mB = rgba.mB;
		m_ColorMask.mA = rgba.mA;

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::SetClearColor( Vector4 color ) {

		if (m_ClearColor == color) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glClearColor( color.mR, color.mG, color.mB, color.mA );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ClearColor( ) {

		glClear( GL_COLOR_BUFFER_BIT );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ClearColor( Vector4 color ) {

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