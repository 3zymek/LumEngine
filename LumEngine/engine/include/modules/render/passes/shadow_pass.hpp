#pragma once

#include "render/common.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	class ShadowPass {
	public:
		
		ShadowPass( ) = default;

		void Initialize(const FRendererContext& ctx) {

			mContext = ctx;

			init();

		}

		void BindShader() {

			mContext.mRenderDevice->BindShader(mShader);
		
		}

		void BindFramebuffer() {

			mContext.mRenderDevice->BindFramebuffer(mFramebuffer);

		}

		void UnbindFramebuffer() {

			mContext.mRenderDevice->UnbindFramebuffer();
		
		}

		void BindShadowMap() {

			mContext.mRenderDevice->BindTexture(mShadowMap, LUM_SHADOW_MAP);

		}

		void CalculateLightSpaceMatrix(const glm::vec3& direction) {
		
			glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 lightDir = glm::normalize(direction);

			glm::vec3 lightPos = -lightDir * mShadowMapDistance;
			
			glm::mat4 lightViewMatrix = glm::lookAt(
				lightPos,
				glm::vec3(0.f),
				up
			);

			glm::mat4 lightProjectionMatrix = glm::ortho(
				-mShadowMapSize, mShadowMapSize,
				-mShadowMapSize, mShadowMapSize,
				mShadowMapNear, mShadowMapFar
			);

			upload_lightspace_matrix(lightProjectionMatrix * lightViewMatrix);

		}

	private:

		FRendererContext mContext;

		float32 mShadowMapDistance = 20.f;
		float32 mShadowMapSize = 50.f;
		float32 mShadowMapNear = 0.1f;
		float32 mShadowMapFar = mShadowMapDistance * 2.0f;


		rhi::RTextureHandle mShadowMap;
		rhi::RFramebufferHandle mFramebuffer;
		rhi::RShaderHandle mShader;

		rhi::RBufferHandle mLightSpaceUniform;
		
		void upload_lightspace_matrix(const glm::mat4& mat) {

			mContext.mRenderDevice->UpdateBuffer(
				mLightSpaceUniform,
				glm::value_ptr(mat)
			);

		}

		void init( ) {
			
			{
				rhi::FTextureDescriptor desc;
				desc.mWidth = 2024;
				desc.mHeight = 2024;
				desc.mImageFormat = rhi::ImageFormat::DepthComponent;
				desc.mImageLayout = rhi::ImageLayout::Depth32F;
				desc.mTextureType = rhi::TextureType::Texture2D;
				mShadowMap = mContext.mRenderDevice->CreateTexture(desc);
			}
			{
				rhi::FFramebufferDescriptor desc;
				desc.mDepthTex = mShadowMap;
				desc.mWidth = 2024;
				desc.mHeight = 2024;
				mFramebuffer = mContext.mRenderDevice->CreateFramebuffer(desc);
			}
			{
				rhi::FBufferDescriptor desc;
				desc.mBufferType = rhi::BufferType::Uniform;
				desc.mBufferUsage = rhi::BufferUsage::Dynamic;
				desc.mMapFlags = rhi::MapFlag::Write;
				desc.mSize = sizeof(glm::mat4);
				mLightSpaceUniform = mContext.mRenderDevice->CreateBuffer(desc);
				mContext.mRenderDevice->SetUniformBufferBinding(mLightSpaceUniform, LUM_UBO_LIGHTSPACE_MATRIX);
			}
			{
				mShader = mContext.mShaderMgr->LoadShader("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", RootID::Internal);	
			}

		}

	};

} // namespace lum::render