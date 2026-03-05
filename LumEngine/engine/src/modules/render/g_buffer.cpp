#include "render/g_buffer.hpp"
#include "event/event_bus.hpp"

namespace lum::render::detail {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void GBuffer::Initialize( const FRendererContext& ctx, uint32 w, uint32 h ) {

		mContext = ctx;

		create_textures(w, h);
		create_framebuffer();

		init();

	}

	void GBuffer::BindTextures( ) {

		mContext.mRenderDevice->BindTexture(mAlbedo, LUM_GBUFFER_ALBEDO);
		mContext.mRenderDevice->BindTexture(mNormal, LUM_GBUFFER_NORMAL);
		mContext.mRenderDevice->BindTexture(mDepth, LUM_GBUFFER_DEPTH);

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void GBuffer::create_textures(uint32 mWidth, uint32 mHeight) {

		mContext.mRenderDevice->DeleteTexture(mAlbedo);
		mContext.mRenderDevice->DeleteTexture(mNormal);
		mContext.mRenderDevice->DeleteTexture(mDepth);

		rhi::FTextureDescriptor desc;
		{
			desc.mImageFormat = rhi::ImageFormat::RGBA;
			desc.mImageLayout = rhi::ImageLayout::RGBA8;
			desc.mWidth = mWidth;
			desc.mHeight = mHeight;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mAlbedo = mContext.mRenderDevice->CreateTexture(desc);
		}
		{
			desc.mImageFormat = rhi::ImageFormat::RGBA;
			desc.mImageLayout = rhi::ImageLayout::RGBA16F;
			desc.mWidth = mWidth;
			desc.mHeight = mHeight;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mNormal = mContext.mRenderDevice->CreateTexture(desc);
		}
		{
			desc.mImageFormat = rhi::ImageFormat::DepthComponent;
			desc.mImageLayout = rhi::ImageLayout::Depth32F;
			desc.mWidth = mWidth;
			desc.mHeight = mHeight;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mDepth = mContext.mRenderDevice->CreateTexture(desc);
		}

	}

	void GBuffer::create_framebuffer() {

		if (mContext.mRenderDevice->IsValid(mFramebuffer)) mContext.mRenderDevice->DeleteFramebuffer(mFramebuffer);

		{
			rhi::FFramebufferDescriptor desc;
			desc.mColorTex.push_back({ LUM_GBUFFER_ALBEDO, mAlbedo });
			desc.mColorTex.push_back({ LUM_GBUFFER_NORMAL, mNormal });
			desc.mDepthTex = mDepth;
			mFramebuffer = mContext.mRenderDevice->CreateFramebuffer(desc);
		}

	}

	void GBuffer::init() {

		mContext.mEventBus->SubscribePermanently<EWindowResized>([this](const EWindowResized& ev) {
			this->create_textures(ev.mWidth, ev.mHeight);
			this->create_framebuffer();
		});

	}


} // namespace lum::render