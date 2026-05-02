
#include "render/shadow_system.hpp"
#include "render/passes/geometry_pass.hpp"
#include "render/passes/light_pass.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void ShadowSystem::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;
		mDirectionalLight.Initialize( mContext );

	}

	void ShadowSystem::Execute( GeometryPass& geoPass, const LightPass& lightPass ) {

		mDirectionalLight.Execute( geoPass, lightPass, mContext );

	}

	void ShadowSystem::DirectionalLight::Initialize( FRendererContext& ctx ) {

		{ // Shadow map texture
			rhi::FTextureDescriptor desc;
			desc.mWidth = mShadowMapTexSize.x;
			desc.mHeight = mShadowMapTexSize.y;
			desc.mImageFormat = rhi::ImageFormat::DepthComponent;
			desc.mImageLayout = rhi::ImageLayout::Depth32F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mShadowMap = ctx.mRenderDev->CreateTexture( desc );
		}
		{ // Shadow FBO
			rhi::FFramebufferDescriptor desc;
			desc.mDepthTex = mShadowMap;
			mShadowFramebuffer = ctx.mRenderDev->CreateFramebuffer( desc );
		}
		{ // Light space matrices UBO
			rhi::FBufferDescriptor desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( glm::mat4 );
			mLightSpaceUBO = ctx.mRenderDev->CreateBuffer( desc );
			ctx.mRenderDev->SetUniformBufferBinding( mLightSpaceUBO, LUM_UBO_LIGHTSPACE_MATRIX );
		}
		{ // Shadow pipeline

			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mCull.bEnabled = true;
			desc.mCull.mFace = rhi::Face::Back;
			mShadowPipeline = ctx.mRenderDev->CreatePipeline( desc );

		}
		{ // Shaders
			mShader = ctx.mShaderMgr->LoadShader( "shaders/shadow_pass.vert", "shaders/shadow_pass.frag", RootID::Internal );
		}

	}

	void ShadowSystem::DirectionalLight::Execute( GeometryPass& geoPass, const LightPass& lightPass, FRendererContext& ctx ) {

		rhi::FViewportState viewport = ctx.mRenderDev->GetViewport( );

		ctx.mRenderDev->BindPipeline( mShadowPipeline );

		calculate_lightspace_matrix( lightPass.GetDirectionalLight( ).mDirection, ctx );
		ctx.mRenderDev->BindFramebuffer( mShadowFramebuffer );
		ctx.mRenderDev->SetViewport( 0, 0, mShadowMapTexSize.x, mShadowMapTexSize.y );
		ctx.mRenderDev->ClearDepth( );

		ctx.mRenderDev->BindShader( mShader );

		geoPass.DrawScene( );

		ctx.mRenderDev->BindFramebuffer( rhi::gDefaultFramebuffer );
		ctx.mRenderDev->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void ShadowSystem::DirectionalLight::calculate_lightspace_matrix( const glm::vec3& dir, FRendererContext& ctx ) {

		glm::vec3 lightDir = glm::normalize( dir );
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

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix, ctx );

	}

	void ShadowSystem::DirectionalLight::upload_lightspace_matrix( const glm::mat4& mat, FRendererContext& ctx ) {

		ctx.mRenderDev->UpdateBuffer(
			mLightSpaceUBO,
			glm::value_ptr( mat )
		);

	}

}