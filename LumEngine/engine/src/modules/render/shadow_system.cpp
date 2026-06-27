
#include "render/shadow_system.hpp"
#include "render/passes/geometry_pass.hpp"
#include "render/passes/light_pass.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void ShadowSystem::Initialize( const RendererContext& ctx ) {

		mContext = ctx;
		mDirectionalLight.Initialize( mContext );

		{ // Shadow mapping pipeline

			rhi::PipelineCreateInfo desc;
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

	void ShadowSystem::DirectionalLight::Initialize( RendererContext& ctx ) {

		{ // Shadow map texture
			rhi::TextureCreateInfo desc;
			desc.mWidth = mShadowMapTexSize.mX;
			desc.mHeight = mShadowMapTexSize.mY;
			desc.mPixelFormat = rhi::TexturePixelFormat::DepthComponent;
			desc.mInternalFormat = rhi::TextureInternalFormat::Depth32F;
			desc.mTextureType = rhi::TextureType::Texture2D;
			mShadowMap = ctx.mRenderDev->CreateTexture( desc );
		}
		{ // Shadow FBO
			rhi::FramebufferCreateInfo desc;
			desc.mDepthTex = mShadowMap;
			mFramebuffer = ctx.mRenderDev->CreateFramebuffer( desc );
		}
		{ // Light space matrices UBO
			rhi::BufferCreateInfo desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( Matrix4 );
			mLightSpaceUBO = ctx.mRenderDev->CreateBuffer( desc );
			ctx.mRenderDev->SetUniformBufferBinding( mLightSpaceUBO, LUM_UBO_LIGHTSPACE_MATRIX );
		}
		{ // Shaders
			mShader = ctx.mShaderMgr->LoadShader( "shaders/shadow_mapping/directional.vert", "shaders/shadow_mapping/directional.frag", ResourceRoot::Internal );
		}

	}

	void ShadowSystem::SpotLight::Initialize( RendererContext& ctx ) {

		{ // Shadow maps texture
			rhi::TextureCreateInfo desc;
			desc.mTextureType = rhi::TextureType::Texture2DArray;
			desc.mDepth = limits::kMaxShadowCastingSpotLights;
			desc.mWidth = mShadowMapTexSize.mX;
			desc.mHeight = mShadowMapTexSize.mY;
			mShadowMaps = ctx.mRenderDev->CreateTexture( desc );
		}
		{ // Shadow FBO
			rhi::FramebufferCreateInfo desc;
			desc.mDepthTex = mShadowMaps;
			mFramebuffer = ctx.mRenderDev->CreateFramebuffer( desc );
		}
		{ // Light space matrices UBO
			rhi::BufferCreateInfo desc;
			desc.mBufferType = rhi::BufferType::Uniform;
			desc.mBufferUsage = rhi::BufferUsage::Dynamic;
			desc.mMapFlags = rhi::MapFlag::Write;
			desc.mSize = sizeof( Matrix4 ) * limits::kMaxShadowCastingSpotLights;
			mLightSpaceUBO = ctx.mRenderDev->CreateBuffer( desc );
			ctx.mRenderDev->SetUniformBufferBinding( mLightSpaceUBO, LUM_UBO_LIGHTSPACE_MATRIX );
		}
		{ // Shader
			mShader = ctx.mShaderMgr->LoadShader( "shaders/shadow_mapping/spot.vert", "shaders/shadow_mapping/spot.frag", ResourceRoot::Internal );
		}

	}


	void ShadowSystem::DirectionalLight::Execute( GeometryPass& geoPass, const LightPass& lightPass, RendererContext& ctx ) {

		rhi::ViewportState viewport = ctx.mRenderDev->GetViewport( );

		calculate_lightspace_matrix( lightPass.GetDirectionalLight( ).mDirection, ctx );
		ctx.mRenderDev->BindFramebuffer( mFramebuffer );
		ctx.mRenderDev->SetViewport( 0, 0, mShadowMapTexSize.mX, mShadowMapTexSize.mY );
		ctx.mRenderDev->ClearDepth( );
		
		ctx.mRenderDev->BindShader( mShader );

		geoPass.DrawScene( );

		ctx.mRenderDev->BindFramebuffer( rhi::kDefaultFramebuffer );
		ctx.mRenderDev->SetViewport( 0, 0, viewport.mWidth, viewport.mHeight );

	}

	void ShadowSystem::SpotLight::Execute( GeometryPass& geoPass, const LightPass& lightPass, RendererContext& ctx ) {

		/*
		const auto& spotLights = lightPass.GetSpotLights( );
		for (uint32 i = 0; i < spotLights.second; i++) {

			const auto& light = spotLights.first[ i ];
			

		}*/


	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void ShadowSystem::DirectionalLight::calculate_lightspace_matrix( const Vector3& dir, RendererContext& ctx ) {

		Vector3 lightDir = Normalize( dir );
		Vector3 up = Vector3( 0.0f, 1.0f, 0.0f );

		if (Abs( Dot( lightDir, up ) ) > 0.99f)
			up = Vector3( 0.0f, 0.0f, 1.0f );

		Vector3 lightPos = -lightDir * mShadowMapDistance;

		Matrix4 lightViewMatrix = LookAt(
			lightPos,
			Vector3( 0.0f ),
			up
		);

		Matrix4 lightProjectionMatrix = Orthographic(
			-mShadowMapSize, mShadowMapSize,
			-mShadowMapSize, mShadowMapSize,
			mShadowMapNear, mShadowMapFar
		);

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix, ctx );

	}

	void ShadowSystem::SpotLight::calculate_lightspace_matrix( const Vector3& dir, const Vector3& pos, float32 fov, RendererContext& ctx ) {

		Vector3 lightDir = Normalize( dir );
		Vector3 up = Vector3( 0.0f, 1.0f, 0.0f );

		if (Abs( Dot( lightDir, up ) ) > 0.99f)
			up = Vector3( 0.0f, 0.0f, 1.0f );

		Matrix4 lightViewMatrix = LookAt(
			pos,
			pos + lightDir,
			up
		);

		float32 aspectRatio = ToFloat32( mShadowMapTexSize.mX ) / ToFloat32( mShadowMapTexSize.mY );

		Matrix4 lightProjectionMatrix = Perspective(
			fov,
			aspectRatio,
			mShadowMapNear,
			mShadowMapFar
		);

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix, ctx );

	}

	void ShadowSystem::DirectionalLight::upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx ) {

		ctx.mRenderDev->UpdateBuffer(
			mLightSpaceUBO,
			mat.Data()
		);

	}

	void ShadowSystem::SpotLight::upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx ) {

		

	}

}