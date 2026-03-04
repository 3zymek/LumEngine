#pragma once

#include "render/common.hpp"
#include "event/events/window_events.hpp"

namespace lum::render::detail {

	class GBuffer {
	public:


		void Initialize(const FRendererContext& ctx, uint32 w, uint32 h) {

			mContext = ctx;

			create_textures(w, h);

			init();

		}

		void BeginPass() {

			mContext.mRenderDevice->BindFramebuffer(mFramebuffer);
			mContext.mRenderDevice->Clear(rhi::ClearFlag::Color | rhi::ClearFlag::Depth);

		}

		void EndPass() {

			mContext.mRenderDevice->UnbindFramebuffer();

		}

	private:

		FRendererContext mContext;

		rhi::RFramebufferHandle mFramebuffer;

		rhi::RTextureHandle mAlbedo;
		rhi::RTextureHandle mNormal;
		rhi::RTextureHandle mDepth;

		void create_textures(uint32 mWidth, uint32 mHeight) {

			mContext.mRenderDevice->DeleteTexture(mAlbedo);
			mContext.mRenderDevice->DeleteTexture(mNormal);
			mContext.mRenderDevice->DeleteTexture(mDepth);

			rhi::FTextureDescriptor desc;
			{
				desc.mImageFormat = rhi::ImageFormat::RGBA;
				desc.mImageLayout = rhi::ImageLayout::SRGB8_Alpha8;
				desc.mWidth = mWidth;
				desc.mHeight = mHeight;
				desc.mTextureType = rhi::TextureType::Texture2D;
				mAlbedo = mContext.mRenderDevice->CreateTexture(desc);
			}
			{
				desc.mImageFormat = rhi::ImageFormat::RGB;
				desc.mImageLayout = rhi::ImageLayout::RGB8;
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

		void init() {

			mContext.mEventBus->SubscribePermanently<EWindowResized>([this](const EWindowResized& ev) {
				this->create_textures(ev.mWidth, ev.mHeight);
			});

			{
				rhi::RFramebufferDescriptor desc;
				desc.mColorTex[LUM_GBUFFER_ALBEDO] = mAlbedo;
				desc.mColorTex[LUM_GBUFFER_NORMAL] = mNormal;
				desc.mNumColorTex = 2;
				desc.mDepthTex = mDepth;
				mFramebuffer = mContext.mRenderDevice->CreateFramebuffer(desc);
			}

		}

	};

}