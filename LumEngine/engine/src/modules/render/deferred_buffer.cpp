//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: G-Buffer management for deferred rendering.
//          Handles creation, resizing and binding of geometry buffers.
//
//=============================================================================//

#include "render/deferred_buffer.hpp"
#include "event/event_bus.hpp"
#include "event/events/window_events.hpp"

namespace lum::render::detail {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void DeferredBuffer::Initialize( const RendererContext& ctx, uint32 w, uint32 h ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		create_textures( w, h );
		create_framebuffer( );

		subscribe_event( );

	}

	void DeferredBuffer::BindTextures( ) const {

		mContext.mRenderDev->BindTexture( mAlbedo, LUM_GBUFFER_ALBEDO );
		mContext.mRenderDev->BindTexture( mNormal, LUM_GBUFFER_NORMAL );
		mContext.mRenderDev->BindTexture( mDepth, LUM_GBUFFER_DEPTH );

	}

	rhi::TextureHandle DeferredBuffer::GetAttachment( DeferredBufferAttachment tex ) {

		switch (tex) {
		case DeferredBufferAttachment::Albedo: return mAlbedo;
		case DeferredBufferAttachment::Normal: return mNormal;
		case DeferredBufferAttachment::Depth: return mDepth;
		default: return mAlbedo;
		}

		return mAlbedo;

	}


	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void DeferredBuffer::create_textures( uint32 width, uint32 height ) {

		mContext.mRenderDev->Delete( mAlbedo );
		mContext.mRenderDev->Delete( mNormal );
		mContext.mRenderDev->Delete( mDepth );

		rhi::TextureCreateInfo desc;
		{ // Albedo
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mAlbedo = mContext.mRenderDev->CreateTexture( desc );
		}
		{ // Normal
			desc.mPixelFormat = rhi::TexturePixelFormat::RGBA;
			desc.mInternalFormat = rhi::TextureInternalFormat::RGBA16F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mNormal = mContext.mRenderDev->CreateTexture( desc );
		}
		{ // Depth
			desc.mPixelFormat = rhi::TexturePixelFormat::DepthComponent;
			desc.mInternalFormat = rhi::TextureInternalFormat::Depth32F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDepth = mContext.mRenderDev->CreateTexture( desc );
		}

	}

	void DeferredBuffer::create_framebuffer( ) {

		if (mContext.mRenderDev->IsValid( mFramebuffer )) mContext.mRenderDev->Delete( mFramebuffer );

		{
			rhi::FramebufferCreateInfo desc;
			desc.mColorTex.push_back( { LUM_GBUFFER_ALBEDO, mAlbedo } );
			desc.mColorTex.push_back( { LUM_GBUFFER_NORMAL, mNormal } );
			desc.mDepthTex = mDepth;
			mFramebuffer = mContext.mRenderDev->CreateFramebuffer( desc );
		}

	}

	void DeferredBuffer::subscribe_event( ) {

		mContext.mEvBus->SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& ev ) {

				if (ev.mWidth == 0 || ev.mHeight == 0) return;

				create_textures( ev.mWidth, ev.mHeight );
				create_framebuffer( );

			} );

	}


} // namespace lum::render