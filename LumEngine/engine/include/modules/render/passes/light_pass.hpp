//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//
#pragma once

#include "render/common.hpp"

namespace lum::render {

	namespace detail { class GBuffer; }

	struct FLightPassDescriptor {

		rhi::RTextureHandle mIrradianceMap;
		rhi::RTextureHandle mPrefilteredEnvMap;
		rhi::RTextureHandle mShadowMap;

	};

	/* @brief Manages light data submission and GPU uploads for the deferred lighting pass.
	*  Collects point lights and directional light each frame, uploads them to GPU
	*  buffers and binds the light pass shader for the fullscreen quad draw call.
	*/
	class LightPass {
	public:

		LightPass( ) = default;

		/* @brief Initializes the pass, allocates GPU buffers and compiles shaders.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const FRendererContext& ctx );

		/* @brief Submits a point light to be included in the current frame's lighting.
		*  @param light Point light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddPointLight( const FPointLight& light );

		/* @brief Submits a spot light to be included in the current frame's lighting.
		* @param light Spot light to add.Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddSpotLight( const FSpotLight& light );

		/* @brief Sets the active directional light for the current frame.
		*  @param light Directional light to set.
		*/
		void SetDirectionalLight( const FDirectionalLight& light );

		/* @brief Returns the currently active directional light. */
		FDirectionalLight GetDirectionalLight( );
		FDirectionalLight GetDirectionalLight( ) const;

		/* @brief Clears all point lights from the previous frame. */
		LUM_FORCEINLINE
		void ClearLights( ) { mActivePointLights = 0; mActiveSpotLights = 0; }

		/* @brief Binds GBuffer textures, shadow map and light uniforms, then issues the fullscreen quad draw call.
		*  @param shadowPass Shadow pass to retrieve the shadow map from.
		*  @param gbuffer GBuffer containing geometry data from the geometry pass.
		*  @param quad Fullscreen quad VAO to draw the lighting onto.
		*/
		void Execute( const detail::GBuffer& gbuffer, const detail::FScreenQuad& quad, const FLightPassDescriptor& desc );

	private:

		static constexpr usize skOffsetPointLights = 0;
		static constexpr usize skOffsetSpotLights = sizeof( FPointLight ) * LUM_MAX_LIGHTS;
		static constexpr usize skOffsetActivePoint = skOffsetSpotLights + sizeof( FSpotLight ) * LUM_MAX_LIGHTS;
		static constexpr usize skOffsetActiveSpot = skOffsetActivePoint + sizeof( int32 );

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief Array of active point lights for this frame. */
		std::array<FPointLight, LUM_MAX_LIGHTS> mPointLights{};

		/* @brief Array of active spot lights for this frame. */
		std::array<FSpotLight, LUM_MAX_LIGHTS> mSpotLights{};

		/* @brief Number of currently active point lights. */
		uint32 mActivePointLights = 0;

		/* @brief Number of currently active spot lights. */
		uint32 mActiveSpotLights = 0;

		/* @brief GPU-ready uniform buffer representation of the active directional light. */
		detail::FDirectionalLightUniformBuffer mDirectionalLightData{};

		/* @brief Shader storage buffer holding all active point lights. */
		rhi::RBufferHandle mLightsUBO;

		/* @brief Uniform buffer holding the active directional light data. */
		rhi::RBufferHandle mDirectionalLightUBO;

		/* @brief Pipeline state for the light pass. */
		rhi::RPipelineHandle mPipeline;

		/* @brief Shader program used for the light pass. */
		rhi::RShaderHandle mShader;

		/* @brief Allocates GPU buffers and initializes pipeline and shader. */
		void init( );

		/* @brief Uploads all active point lights to the GPU shader storage buffer. */
		void upload_point_lights( );

		/* @brief Uploads all active spot lights to the GPU shader storage buffer. */
		void upload_spot_lights( );

		/* @brief Uploads the active directional light to its GPU uniform buffer. */
		void upload_directional_light( );

	};

} // namespace lum::render