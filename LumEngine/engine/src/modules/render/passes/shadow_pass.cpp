//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Shadow map generation pass for the directional light.
//
//=============================================================================//

#include "render/passes/shadow_pass.hpp"
#include "render/passes/geometry_pass.hpp"
#include "render/passes/light_pass.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void ShadowPass::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;

		init();

	}

	void ShadowPass::Execute( GeometryPass& geometryPass, const LightPass& lightPass ) {

		rhi::FViewportState viewport = mContext.mRenderDevice->GetViewport();

		rhi::FCullState cull = mContext.mRenderDevice->GetCullState();
		mContext.mRenderDevice->ToggleCull(true);
		mContext.mRenderDevice->SetCullFace(rhi::Face::Front);

		calculate_lightspace_matrix(lightPass.GetDirectionalLight().mDirection);
		mContext.mRenderDevice->BindFramebuffer(mFramebuffer);
		mContext.mRenderDevice->SetViewport(0, 0, mShadowMapTexSize.x, mShadowMapTexSize.y);
		mContext.mRenderDevice->ClearDepth();

		mContext.mRenderDevice->BindShader(mShader);

		geometryPass.DrawScene();

		mContext.mRenderDevice->BindFramebuffer(rhi::gDefaultFramebuffer);
		mContext.mRenderDevice->SetViewport(0, 0, viewport.mWidth, viewport.mHeight);

		mContext.mRenderDevice->ToggleCull(cull.bEnabled);
		mContext.mRenderDevice->SetCullFace(cull.mFace);

	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

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
			mLightSpaceUBO,
			glm::value_ptr(mat)
		);

	}

	void ShadowPass::init( ) {

		{
			rhi::FTextureDescriptor desc;
			desc.mWidth = mShadowMapTexSize.x;
			desc.mHeight = mShadowMapTexSize.y;
			desc.mImageFormat = rhi::ImageFormat::DepthComponent;
			desc.mImageLayout = rhi::ImageLayout::Depth32F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mShadowMap = mContext.mRenderDevice->CreateTexture(desc);
		}
		{
			rhi::FFramebufferDescriptor desc;
			desc.mDepthTex = mShadowMap;
			mFramebuffer = mContext.mRenderDevice->CreateFramebuffer(desc);
		}
		{
			rhi::FBufferDescriptor desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof(glm::mat4);
			mLightSpaceUBO = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mLightSpaceUBO, LUM_UBO_LIGHTSPACE_MATRIX);
		}
		{
			mShader = mContext.mShaderMgr->LoadShader("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", RootID::Internal);
		}

	}

} // namespace lum::render