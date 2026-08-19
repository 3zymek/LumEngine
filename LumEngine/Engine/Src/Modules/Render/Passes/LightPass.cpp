//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//

#include "Rhi/Core/RhiDevice.hpp"
#include "Render/Renderer.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void LightPass::Initialize( RendererContext& ctx ) {

		ctx.Validate( );

		mCtx = ctx;

		init( );

	}

	void LightPass::AddPointLight( const PointLight& light ) {

		LUM_ASSERT( mActivePointLights + 1 <= mPointLights.size( ), "Max point lights reached" );

		mPointLights[ mActivePointLights++ ] = light;

	}
	void LightPass::AddSpotLight( const SpotLight& light ) {

		LUM_ASSERT( mActiveSpotLights + 1 <= mSpotLights.size( ), "Max spot lights reached" );

		mSpotLights[ mActiveSpotLights++ ] = light;

	}


	void LightPass::SetDirectionalLight( const DirectionalLight& light ) {

		mDirectionalLightData.mColor = Vector4( light.mColor, 0.0f );
		mDirectionalLightData.mDirection = Vector4( light.mDirection, 0.0f );
		mDirectionalLightData.mIntensity = light.mIntensity;

	}

	DirectionalLight LightPass::GetDirectionalLight( ) {
		return {
			Vector3( mDirectionalLightData.mDirection ),
			mDirectionalLightData.mIntensity,
			Vector3( mDirectionalLightData.mColor )
		};
	}
	DirectionalLight LightPass::GetDirectionalLight( ) const {
		return {
			Vector3( mDirectionalLightData.mDirection ),
			mDirectionalLightData.mIntensity,
			Vector3( mDirectionalLightData.mColor )
		};
	}

	void LightPass::Execute( const detail::DeferredBuffer& gbuffer, const detail::ScreenQuad& quad, const LightPassExectueContext& desc ) {

		auto& device = mCtx( ).mRenderDev( );

		device.BindPipeline( mPipeline );

		upload_directional_light( );
		upload_point_lights( );
		upload_spot_lights( );

		device.BindShader( mShader );
		device.BindTexture( desc.mShadowMap, LUM_SHADOW_MAP );
		device.BindTexture( desc.mIrradianceMap, LUM_TEX_IRRADIANCE );
		device.BindTexture( desc.mPrefilteredEnvMap, LUM_TEX_PREFILTERED );
		gbuffer.BindTextures( );

		device.DrawElements( quad.mVao, 6 );

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void LightPass::init( ) {

		rhi::BufferCreateInfo desc;
		desc.mBufferUsage = rhi::BufferUsage::Dynamic;
		desc.mMapFlags = rhi::MapFlag::Write;

		auto& device = mCtx( ).mRenderDev( );

		// Point Lights SSBO
		if (!device.IsValid( mLightsUbo )) {

			desc.mSize = (sizeof( PointLight ) * LUM_MAX_LIGHTS + sizeof( int32 )) + (sizeof( SpotLight ) * LUM_MAX_LIGHTS + sizeof( int32 ));
			desc.mBufferType = rhi::BufferType::ShaderStorage;
			mLightsUbo = device.CreateBuffer( desc );
			device.SetShaderStorageBinding( mLightsUbo, LUM_SSBO_LIGHTS_BINDING );

		}
		// Directional Light UBO
		if (!device.IsValid( mDirectionalLightUbo )) {

			desc.mSize = sizeof( mDirectionalLightData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mDirectionalLightUbo = device.CreateBuffer( desc );
			device.SetUniformBufferBinding( mDirectionalLightUbo, LUM_UBO_DIRECTIONAL_LIGHT );

		}

		if (!device.IsValid( mPipeline )) {
		
			rhi::PipelineCreateInfo desc;
			desc.mDepthStencil.mDepth.bEnabled = false;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			mPipeline = mCtx( ).mRenderDev( ).CreatePipeline( desc );
		
		}

		{ // Shaders

			mShader = mCtx( ).mShaderMgr( ).LoadShader( "shaders/light_pass.vert", "shaders/light_pass.frag", ResourceRoot::Internal );
		
		}


	}

	void LightPass::upload_point_lights( ) {

		mCtx( ).mRenderDev().UpdateBuffer(
			mLightsUbo, &mActivePointLights,
			skOffsetActivePoint, sizeof( int32 )
		);

		mCtx( ).mRenderDev( ).UpdateBuffer(
			mLightsUbo, mPointLights.data( ),
			skOffsetPointLights, sizeof( PointLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_spot_lights( ) {

		mCtx( ).mRenderDev( ).UpdateBuffer(
			mLightsUbo, &mActiveSpotLights,
			skOffsetActiveSpot, sizeof( int32 )
		);

		mCtx( ).mRenderDev( ).UpdateBuffer(
			mLightsUbo, mSpotLights.data( ),
			skOffsetSpotLights, sizeof( SpotLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_directional_light( ) {

		mCtx( ).mRenderDev( ).UpdateBuffer(
			mDirectionalLightUbo,
			&mDirectionalLightData
		);

	}

} // namespace lum::render