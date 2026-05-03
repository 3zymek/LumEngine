
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

		{ // Shadow mapping pipeline

			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mCull.bEnabled = true;
			desc.mCull.mFace = rhi::Face::Back;
			mShadowMappingPipeline = mContext.mRenderDev->CreatePipeline( desc );

		}

	}

	void ShadowSystem::Execute( GeometryPass& geoPass, const LightPass& lightPass ) {

		mContext.mRenderDev->BindPipeline( mShadowMappingPipeline );
		mDirectionalLight.Execute( geoPass, lightPass, mContext );
		//mSpotLight.Execute( geoPass, lightPass, mContext );

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
			mFramebuffer = ctx.mRenderDev->CreateFramebuffer( desc );
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
		{ // Shaders
			mShader = ctx.mShaderMgr->LoadShader( "shaders/shadow_mapping/directional.vert", "shaders/shadow_mapping/directional.frag", RootID::Internal );
		}

	}

	void ShadowSystem::SpotLight::Initialize( FRendererContext& ctx ) {

		{ // Shadow maps texture
			rhi::FTextureDescriptor desc;
			desc.mTextureType = rhi::TextureType::Texture2DArray;
			desc.mDepth = limits::gMaxShadowCastingSpotLights;
			desc.mWidth = mShadowMapTexSize.x;
			desc.mHeight = mShadowMapTexSize.y;
			mShadowMaps = ctx.mRenderDev->CreateTexture( desc );
		}
		{ // Shadow FBO
			rhi::FFramebufferDescriptor desc;
			desc.mDepthTex = mShadowMaps;
			mFramebuffer = ctx.mRenderDev->CreateFramebuffer( desc );
		}
		{ // Light space matrices UBO
			rhi::FBufferDescriptor desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( glm::mat4 ) * limits::gMaxShadowCastingSpotLights;
			mLightSpaceUBO = ctx.mRenderDev->CreateBuffer( desc );
			ctx.mRenderDev->SetUniformBufferBinding( mLightSpaceUBO, LUM_UBO_LIGHTSPACE_MATRIX );
		}
		{ // Shader
			mShader = ctx.mShaderMgr->LoadShader( "shaders/shadow_mapping/spot.vert", "shaders/shadow_mapping/spot.frag", RootID::Internal );
		}

	}


	void ShadowSystem::DirectionalLight::Execute( GeometryPass& geoPass, const LightPass& lightPass, FRendererContext& ctx ) {

		rhi::FViewportState viewport = ctx.mRenderDev->GetViewport( );

		calculate_lightspace_matrix( lightPass.GetDirectionalLight( ).mDirection, ctx );
		ctx.mRenderDev->BindFramebuffer( mFramebuffer );
		ctx.mRenderDev->SetViewport( 0, 0, mShadowMapTexSize.x, mShadowMapTexSize.y );
		ctx.mRenderDev->ClearDepth( );
		
		ctx.mRenderDev->BindShader( mShader );

		geoPass.DrawScene( );

		ctx.mRenderDev->BindFramebuffer( rhi::gDefaultFramebuffer );
		ctx.mRenderDev->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );

	}

	void ShadowSystem::SpotLight::Execute( GeometryPass& geoPass, const LightPass& lightPass, FRendererContext& ctx ) {

		/*
		const auto& spotLights = lightPass.GetSpotLights( );
		for (uint32 i = 0; i < spotLights.second; i++) {

			const auto& light = spotLights.first[ i ];
			

		}*/


	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void ShadowSystem::DirectionalLight::calculate_lightspace_matrix( const glm::vec3& dir, FRendererContext& ctx ) {

		glm::vec3 lightDir = glm::normalize( dir );
		glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f );

		if (glm::abs( glm::dot( lightDir, up ) ) > 0.99f)
			up = glm::vec3( 0.0f, 0.0f, 1.0f );

		glm::vec3 lightPos = -lightDir * mShadowMapDistance;

		glm::mat4 lightViewMatrix = glm::lookAt(
			lightPos,
			glm::vec3( 0.0f ),
			up
		);

		glm::mat4 lightProjectionMatrix = glm::ortho(
			-mShadowMapSize, mShadowMapSize,
			-mShadowMapSize, mShadowMapSize,
			mShadowMapNear, mShadowMapFar
		);

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix, ctx );

	}

	void ShadowSystem::SpotLight::calculate_lightspace_matrix( const glm::vec3& dir, const glm::vec3 pos, float32 fov, FRendererContext& ctx ) {

		glm::vec3 lightDir = glm::normalize( dir );
		glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f );

		if (glm::abs( glm::dot( lightDir, up ) ) > 0.99f)
			up = glm::vec3( 0.0f, 0.0f, 1.0f );

		glm::mat4 lightViewMatrix = glm::lookAt(
			pos,
			pos + lightDir,
			up
		);

		float32 aspectRatio = ToFloat32( mShadowMapTexSize.x ) / ToFloat32( mShadowMapTexSize.y );

		glm::mat4 lightProjectionMatrix = glm::perspective(
			fov,
			aspectRatio,
			mShadowMapNear,
			mShadowMapFar
		);

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix, ctx );

	}

	void ShadowSystem::DirectionalLight::upload_lightspace_matrix( const glm::mat4& mat, FRendererContext& ctx ) {

		ctx.mRenderDev->UpdateBuffer(
			mLightSpaceUBO,
			glm::value_ptr( mat )
		);

	}

	void ShadowSystem::SpotLight::upload_lightspace_matrix( const glm::mat4& mat, FRendererContext& ctx ) {

		

	}

}