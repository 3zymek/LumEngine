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

		ValidateRendererContext( ctx );

		mContext = ctx;

		init( );

	}

	void ShadowPass::Execute( GeometryPass& geometryPass, const LightPass& lightPass ) {

		rhi::FViewportState viewport = mContext.mRenderDev->GetViewport( );

		mContext.mRenderDev->BindPipeline( mShadowPipeline );

		calculate_lightspace_matrix( lightPass.GetDirectionalLight( ).mDirection );
		mContext.mRenderDev->BindFramebuffer( mShadowFramebuffer );
		mContext.mRenderDev->SetViewport( 0, 0, mShadowMapTexSize.x, mShadowMapTexSize.y );
		mContext.mRenderDev->ClearDepth( );

		mContext.mRenderDev->BindShader( mShader );

		geometryPass.DrawScene( );

		mContext.mRenderDev->BindFramebuffer( rhi::gDefaultFramebuffer );
		mContext.mRenderDev->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );

	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void ShadowPass::calculate_lightspace_matrix( const glm::vec3& direction ) {

		glm::vec3 lightDir = glm::normalize( direction );
		glm::vec3 up = glm::vec3( 0.f, 1.f, 0.f );

		if (glm::abs( glm::dot( lightDir, up ) ) > 0.99f)
			up = glm::vec3( 0.f, 0.f, 1.f );

		glm::vec3 lightPos = -lightDir * mShadowMapDistance;

		glm::mat4 lightViewMatrix = glm::lookAt(
			lightPos,
			glm::vec3( 0.f ),
			up
		);

		glm::mat4 lightProjectionMatrix = glm::ortho(
			-mShadowMapSize, mShadowMapSize,
			-mShadowMapSize, mShadowMapSize,
			mShadowMapNear, mShadowMapFar
		);

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix );

	}

	void ShadowPass::upload_lightspace_matrix( const glm::mat4& mat ) {

		mContext.mRenderDev->UpdateBuffer(
			mLightSpaceUBO,
			glm::value_ptr( mat )
		);

	}

	void ShadowPass::init( ) {

		{ // Shadow map texture
			rhi::FTextureDescriptor desc;
			desc.mWidth = mShadowMapTexSize.x;
			desc.mHeight = mShadowMapTexSize.y;
			desc.mImageFormat = rhi::ImageFormat::DepthComponent;
			desc.mImageLayout = rhi::ImageLayout::Depth32F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mShadowMap = mContext.mRenderDev->CreateTexture( desc );
		}
		{ // Shadow FBO
			rhi::FFramebufferDescriptor desc;
			desc.mDepthTex = mShadowMap;
			mShadowFramebuffer = mContext.mRenderDev->CreateFramebuffer( desc );
		}
		{ // Light space matrices UBO
			rhi::FBufferDescriptor desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( glm::mat4 );
			mLightSpaceUBO = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetUniformBufferBinding( mLightSpaceUBO, LUM_UBO_LIGHTSPACE_MATRIX );
		}
		{ // Shadow pipeline

			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mCull.bEnabled = true;
			desc.mCull.mFace = rhi::Face::Back;
			mShadowPipeline = mContext.mRenderDev->CreatePipeline( desc );

		}
		{ // Shaders
			mShader = mContext.mShaderMgr->LoadShader( "shaders/shadow_pass.vert", "shaders/shadow_pass.frag", RootID::Internal );
		}

	}

} // namespace lum::render