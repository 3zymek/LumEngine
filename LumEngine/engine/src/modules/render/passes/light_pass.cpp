//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//

#include "rhi/core/rhi_device.hpp"
#include "render/renderer.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void LightPass::Initialize( const FRendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		init( );

	}

	void LightPass::AddPointLight( const FPointLight& light ) {

		LUM_ASSERT( mActivePointLights + 1 <= mPointLights.size( ), "Max point lights reached" );

		mPointLights[ mActivePointLights++ ] = light;

	}
	void LightPass::AddSpotLight( const FSpotLight& light ) {

		LUM_ASSERT( mActiveSpotLights + 1 <= mSpotLights.size( ), "Max spot lights reached" );

		mSpotLights[ mActiveSpotLights++ ] = light;

	}


	void LightPass::SetDirectionalLight( const FDirectionalLight& light ) {

		mDirectionalLightData.mColor = glm::vec4( light.mColor, 0.0 );
		mDirectionalLightData.mDirection = glm::vec4( light.mDirection, 0.0 );
		mDirectionalLightData.mIntensity = light.mIntensity;

	}

	FDirectionalLight LightPass::GetDirectionalLight( ) {
		return {
			glm::vec3( mDirectionalLightData.mDirection ),
			mDirectionalLightData.mIntensity,
			glm::vec3( mDirectionalLightData.mColor )
		};
	}
	FDirectionalLight LightPass::GetDirectionalLight( ) const {
		return {
			glm::vec3( mDirectionalLightData.mDirection ),
			mDirectionalLightData.mIntensity,
			glm::vec3( mDirectionalLightData.mColor )
		};
	}

	void LightPass::Execute( const detail::GBuffer& gbuffer, const detail::FScreenQuad& quad, const FLightPassExecute& desc ) {

		mContext.mRenderDev->BindFramebuffer( quad.mSceneFbo );
		mContext.mRenderDev->BindPipeline( mPipeline );

		upload_directional_light( );
		upload_point_lights( );
		upload_spot_lights( );

		mContext.mRenderDev->BindShader( mShader );
		mContext.mRenderDev->BindTexture( desc.mShadowMap, LUM_SHADOW_MAP );
		mContext.mRenderDev->BindTexture( desc.mIrradianceMap, LUM_TEX_IRRADIANCE );
		mContext.mRenderDev->BindTexture( desc.mPrefilteredEnvMap, LUM_TEX_PREFILTERED );
		gbuffer.BindTextures( );

		mContext.mRenderDev->DrawElements( quad.mVao, 6 );

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void LightPass::init( ) {

		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::BufferUsage::Dynamic;
		desc.mMapFlags = rhi::MapFlag::Write;

		// Point Lights SSBO
		if (!mContext.mRenderDev->IsValid( mLightsUBO )) {

			desc.mSize = (sizeof( FPointLight ) * LUM_MAX_LIGHTS + sizeof( int32 )) + (sizeof( FSpotLight ) * LUM_MAX_LIGHTS + sizeof( int32 ));
			desc.mBufferType = rhi::BufferType::ShaderStorage;
			mLightsUBO = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetShaderStorageBinding( mLightsUBO, LUM_SSBO_LIGHTS_BINDING );

		}
		// Directional Light UBO
		if (!mContext.mRenderDev->IsValid( mDirectionalLightUBO )) {

			desc.mSize = sizeof( mDirectionalLightData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mDirectionalLightUBO = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetUniformBufferBinding( mDirectionalLightUBO, LUM_UBO_DIRECTIONAL_LIGHT );

		}

		if (!mContext.mRenderDev->IsValid( mPipeline )) {
		
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = false;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			mPipeline = mContext.mRenderDev->CreatePipeline( desc );
		
		}

		{ // Shaders

			mShader = mContext.mShaderMgr->LoadShader( "shaders/light_pass.vert", "shaders/light_pass.frag", RootID::Internal );
		
		}


	}

	void LightPass::upload_point_lights( ) {

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, &mActivePointLights,
			skOffsetActivePoint, sizeof( int32 )
		);

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, mPointLights.data( ),
			skOffsetPointLights, sizeof( FPointLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_spot_lights( ) {

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, &mActiveSpotLights,
			skOffsetActiveSpot, sizeof( int32 )
		);

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, mSpotLights.data( ),
			skOffsetSpotLights, sizeof( FSpotLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_directional_light( ) {

		mContext.mRenderDev->UpdateBuffer(
			mDirectionalLightUBO,
			&mDirectionalLightData
		);

	}

} // namespace lum::render