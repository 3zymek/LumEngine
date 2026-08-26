//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: G-Buffer management for deferred rendering.
//          Handles creation, resizing and binding of geometry buffers.
//
//=============================================================================//

#include "Render/DeferredBuffer.hpp"
#include "Event/EventBus.hpp"
#include "Event/Events/WindowEvents.hpp"

namespace lum::render::detail {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void DeferredBuffer::Initialize( RendererContext& ctx, uint32 w, uint32 h ) {

		ctx.Validate( );

		m_Ctx = ctx;

		create_textures( w, h );
		create_framebuffer( );

		subscribe_event( );

	}

	void DeferredBuffer::BindTextures( ) const {

		m_Ctx( ).m_RenderDev( ).BindTexture( m_AlbedoTex, LUM_GBUFFER_ALBEDO );
		m_Ctx( ).m_RenderDev( ).BindTexture( m_NormalTex, LUM_GBUFFER_NORMAL );
		m_Ctx( ).m_RenderDev( ).BindTexture( m_DepthTex, LUM_GBUFFER_DEPTH );

	}

	rhi::TextureHandle DeferredBuffer::GetAttachment( DeferredBufferAttachment tex ) {

		switch (tex) {
		case DeferredBufferAttachment::Albedo: return m_AlbedoTex;
		case DeferredBufferAttachment::Normal: return m_NormalTex;
		case DeferredBufferAttachment::Depth: return m_DepthTex;
		default: return m_AlbedoTex;
		}

		return m_AlbedoTex;

	}


	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void DeferredBuffer::create_textures( uint32 width, uint32 height ) {

		m_Ctx( ).m_RenderDev( ).Delete( m_AlbedoTex );
		m_Ctx( ).m_RenderDev( ).Delete( m_NormalTex );
		m_Ctx( ).m_RenderDev( ).Delete( m_DepthTex );

		rhi::TextureCreateInfo desc;
		{ // Albedo
			desc.m_PixelFormat = rhi::PixelLayout::RGBA;
			desc.m_InternalFormat = rhi::TextureFormat::RGBA16F;
			desc.m_Width = width;
			desc.m_Height = height;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_AlbedoTex = m_Ctx( ).m_RenderDev( ).CreateTexture( desc );
		}
		{ // Normal
			desc.m_PixelFormat = rhi::PixelLayout::RGBA;
			desc.m_InternalFormat = rhi::TextureFormat::RGBA16F;
			desc.m_Width = width;
			desc.m_Height = height;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_NormalTex = m_Ctx( ).m_RenderDev( ).CreateTexture( desc );
		}
		{ // Depth
			desc.m_PixelFormat = rhi::PixelLayout::DepthComponent;
			desc.m_InternalFormat = rhi::TextureFormat::Depth32F;
			desc.m_Width = width;
			desc.m_Height = height;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_DepthTex = m_Ctx( ).m_RenderDev( ).CreateTexture( desc );
		}

	}

	void DeferredBuffer::create_framebuffer( ) {

		if (m_Ctx( ).m_RenderDev( ).IsValid( m_Framebuffer ))
			m_Ctx( ).m_RenderDev( ).Delete( m_Framebuffer );

		{
			rhi::FramebufferCreateInfo desc;
			desc.m_ColorTex.push_back( { LUM_GBUFFER_ALBEDO, m_AlbedoTex } );
			desc.m_ColorTex.push_back( { LUM_GBUFFER_NORMAL, m_NormalTex } );
			desc.m_DepthTex = m_DepthTex;
			m_Framebuffer = m_Ctx( ).m_RenderDev( ).CreateFramebuffer( desc );
		}

	}

	void DeferredBuffer::subscribe_event( ) {

		m_Ctx( ).m_EventBus( ).SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& ev ) {

				if (ev.m_Width == 0 || ev.m_Height == 0) return;

				create_textures( ev.m_Width, ev.m_Height );
				create_framebuffer( );

			} );

	}


} // namespace lum::render