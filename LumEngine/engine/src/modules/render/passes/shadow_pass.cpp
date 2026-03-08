#pragma once

#include "render/passes/shadow_pass.hpp"
#include "render/passes/geometry_pass.hpp"
#include "render/passes/light_pass.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	void ShadowPass::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;

		init();

	}

	void ShadowPass::Execute( GeometryPass& geometryPass, const LightPass& lightPass ) {

		rhi::FViewportState viewport = mContext.mRenderDevice->GetViewport();

		rhi::FCullState cull = mContext.mRenderDevice->GetCullState();
		mContext.mRenderDevice->SetCullFace(rhi::Face::Front);

		calculate_lightspace_matrix(lightPass.GetDirectionalLight().mDirection);
		mContext.mRenderDevice->BindFramebuffer(mFramebuffer);
		mContext.mRenderDevice->SetViewport(0, 0, 2024, 2024);
		mContext.mRenderDevice->ClearDepth();

		mContext.mRenderDevice->BindShader(mShader);

		geometryPass.DrawScene();

		mContext.mRenderDevice->UnbindFramebuffer();
		mContext.mRenderDevice->SetViewport(0, 0, viewport.mWidth, viewport.mHeight);

		mContext.mRenderDevice->SetCullFace(cull.mFace);

	}

	void ShadowPass::calculate_lightspace_matrix( const glm::vec3& direction ) {

		glm::vec3 lightDir = glm::normalize(direction);
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);

		if (glm::abs(glm::dot(lightDir, up)) > 0.99f)
			up = glm::vec3(0.f, 0.f, 1.f);

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

	void ShadowPass::upload_lightspace_matrix( const glm::mat4& mat ) {

		mContext.mRenderDevice->UpdateBuffer(
			mLightSpaceUniform,
			glm::value_ptr(mat)
		);

	}

	void ShadowPass::init( ) {

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

} // namespace lum::render