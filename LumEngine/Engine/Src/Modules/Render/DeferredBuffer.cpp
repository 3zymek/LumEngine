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

	void DeferredBuffer::Initialize( const RendererContext& ctx, uint32 w, uint32 h ) {

		ctx.Validate( );

		mCtx = ctx;

		create_textures( w, h );
		create_framebuffer( );

		subscribe_event( );

	}

	void DeferredBuffer::BindTextures( ) const {

		mCtx.mRenderDev->BindTexture( mAlbedoTex, LUM_GBUFFER_ALBEDO );
		mCtx.mRenderDev->BindTexture( mNormalTex, LUM_GBUFFER_NORMAL );
		mCtx.mRenderDev->BindTexture( mDepthTex, LUM_GBUFFER_DEPTH );

	}

	rhi::TextureHandle DeferredBuffer::GetAttachment( DeferredBufferAttachment tex ) {

		switch (tex) {
		case DeferredBufferAttachment::Albedo: return mAlbedoTex;
		case DeferredBufferAttachment::Normal: return mNormalTex;
		case DeferredBufferAttachment::Depth: return mDepthTex;
		default: return mAlbedoTex;
		}

		return mAlbedoTex;

	}


	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void DeferredBuffer::create_textures( uint32 width, uint32 height ) {

		mCtx.mRenderDev->Delete( mAlbedoTex );
		mCtx.mRenderDev->Delete( mNormalTex );
		mCtx.mRenderDev->Delete( mDepthTex );

		rhi::TextureCreateInfo desc;
		{ // Albedo
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mAlbedoTex = mCtx.mRenderDev->CreateTexture( desc );
		}
		{ // Normal
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mNormalTex = mCtx.mRenderDev->CreateTexture( desc );
		}
		{ // Depth
			desc.mPixelFormat = rhi::TexturePixelFormat::DepthComponent;
			desc.mInternalFormat = rhi::TextureInternalFormat::Depth32F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDepthTex = mCtx.mRenderDev->CreateTexture( desc );
		}

	}

	void DeferredBuffer::create_framebuffer( ) {

		if (mCtx.mRenderDev->IsValid( mFramebuffer )) 
			mCtx.mRenderDev->Delete( mFramebuffer );

		{
			rhi::FramebufferCreateInfo desc;
			desc.mColorTex.push_back( { LUM_GBUFFER_ALBEDO, mAlbedoTex } );
			desc.mColorTex.push_back( { LUM_GBUFFER_NORMAL, mNormalTex } );
			desc.mDepthTex = mDepthTex;
			mFramebuffer = mCtx.mRenderDev->CreateFramebuffer( desc );
		}

	}

	void DeferredBuffer::subscribe_event( ) {

		mCtx.mEvBus->SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& ev ) {

				if (ev.mWidth == 0 || ev.mHeight == 0) return;

				create_textures( ev.mWidth, ev.mHeight );
				create_framebuffer( );

			} );

	}


} // namespace lum::render