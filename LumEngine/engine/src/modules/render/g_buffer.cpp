#include "render/g_buffer.hpp"
#include "event/event_bus.hpp"
#include "event/events/window_events.hpp"

namespace lum::render::detail {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void GBuffer::Initialize( const FRendererContext& ctx, uint32 w, uint32 h ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		create_textures( w, h );
		create_framebuffer( );

		init( );

	}

	void GBuffer::BindTextures( ) const {

		mContext.mRenderDevice->BindTexture( mAlbedo, LUM_GBUFFER_ALBEDO );
		mContext.mRenderDevice->BindTexture( mNormal, LUM_GBUFFER_NORMAL );
		mContext.mRenderDevice->BindTexture( mDepth, LUM_GBUFFER_DEPTH );

	}

	rhi::RTextureHandle GBuffer::GetTexture( GBufferTexture tex ) {

		switch (tex) {
		case GBufferTexture::Albedo: return mAlbedo;
		case GBufferTexture::Normal: return mNormal;
		case GBufferTexture::Depth: return mDepth;
		default: return mAlbedo;
		}

		return mAlbedo;

	}


	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void GBuffer::create_textures( uint32 width, uint32 height ) {

		mContext.mRenderDevice->DeleteTexture( mAlbedo );
		mContext.mRenderDevice->DeleteTexture( mNormal );
		mContext.mRenderDevice->DeleteTexture( mDepth );

		rhi::FTextureDescriptor desc;
		{ // Albedo
			desc.mImageFormat = rhi::ImageFormat::RGBA;
			desc.mImageLayout = rhi::ImageLayout::RGBA16F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mAlbedo = mContext.mRenderDevice->CreateTexture( desc );
		}
		{ // Normal
			desc.mImageFormat = rhi::ImageFormat::RGBA;
			desc.mImageLayout = rhi::ImageLayout::RGBA16F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mNormal = mContext.mRenderDevice->CreateTexture( desc );
		}
		{ // Depth
			desc.mImageFormat = rhi::ImageFormat::DepthComponent;
			desc.mImageLayout = rhi::ImageLayout::Depth32F;
			desc.mWidth = width;
			desc.mHeight = height;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDepth = mContext.mRenderDevice->CreateTexture( desc );
		}

	}

	void GBuffer::create_framebuffer( ) {

		if (mContext.mRenderDevice->IsValid( mFramebuffer )) mContext.mRenderDevice->DeleteFramebuffer( mFramebuffer );

		{
			rhi::FFramebufferDescriptor desc;
			desc.mColorTex.push_back( { LUM_GBUFFER_ALBEDO, mAlbedo } );
			desc.mColorTex.push_back( { LUM_GBUFFER_NORMAL, mNormal } );
			desc.mDepthTex = mDepth;
			mFramebuffer = mContext.mRenderDevice->CreateFramebuffer( desc );
		}

	}

	void GBuffer::init( ) {

		mContext.mEventBus->SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& ev ) {

				if (ev.mWidth == 0 || ev.mHeight == 0) return;

				create_textures( ev.mWidth, ev.mHeight );
				create_framebuffer( );

			} );

	}


} // namespace lum::render