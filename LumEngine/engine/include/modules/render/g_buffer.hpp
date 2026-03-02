#pragma once

#include "render/common.hpp"

namespace lum::render::detail {

	class GBuffer {
	public:


		void Initialize(const FRendererContext& ctx, uint32 w, uint32 h) {

			mContext = ctx;

			Resize(w, h);
			create_textures();

			init();

		}

		void Resize(uint32 w, uint32 h) {

			mWidth = w;
			mHeight = h;

		}

		void BeginPass() {

			mContext.mRenderDevice->BindFramebuffer(mFramebuffer);
			mContext.mRenderDevice->Clear(rhi::EClearFlag::Color | rhi::EClearFlag::Depth);

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

		uint32 mWidth = 0;
		uint32 mHeight = 0;

		void create_textures() {

			mContext.mRenderDevice->DeleteTexture(mAlbedo);
			mContext.mRenderDevice->DeleteTexture(mNormal);
			mContext.mRenderDevice->DeleteTexture(mDepth);

			{
				rhi::RTextureDescriptor desc;
				desc.mImageFormat = rhi::RImageFormat::RGBA;
				desc.mImageLayout = rhi::RImageLayout::SRGB8_Alpha8;
				desc.mWidth = mWidth;
				desc.mHeight = mHeight;
				mAlbedo = mContext.mRenderDevice->CreateTexture(desc);
			}
			{
				rhi::RTextureDescriptor desc;
				desc.mImageFormat = rhi::RImageFormat::RGB;
				desc.mImageLayout = rhi::RImageLayout::RGB8;
				desc.mWidth = mWidth;
				desc.mHeight = mHeight;
				mNormal = mContext.mRenderDevice->CreateTexture(desc);
			}
			{
				rhi::RTextureDescriptor desc;
				desc.mImageFormat = rhi::RImageFormat::DepthComponent;
				desc.mImageLayout = rhi::RImageLayout::Depth32F;
				desc.mWidth = mWidth;
				desc.mHeight = mHeight;
				mDepth = mContext.mRenderDevice->CreateTexture(desc);
			}

		}

		void init() {
			
			{
				rhi::RFramebufferDescriptor desc;
				desc.mColorTex[0] = mAlbedo;
				desc.mColorTex[1] = mNormal;
				desc.mNumColorTex = 2;
				desc.mDepthTex = mDepth;
				mFramebuffer = mContext.mRenderDevice->CreateFramebuffer(desc);
			}

		}

	};

}