//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//
#pragma once

#include "Render/RenderCommon.hpp"

namespace lum::render {

	namespace detail { class DeferredBuffer; }
	using PointLightsArr = std::array<PointLight, LUM_MAX_LIGHTS>;
	using SpotLightsArr = std::array<SpotLight, LUM_MAX_LIGHTS>;

	/* @brief Descriptor passed to Execute() containing IBL and shadow map handles
	*  required for the deferred lighting calculation.
	*/
	struct LightPassExectueContext {

		rhi::TextureHandle m_IrradianceMap{};     /* @brief Precomputed irradiance cubemap for diffuse IBL. */
		rhi::TextureHandle m_PrefilteredEnvMap{}; /* @brief Prefiltered environment cubemap for specular IBL. */
		rhi::TextureHandle m_ShadowMap{};         /* @brief Depth map from the shadow pass. */

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
		void Initialize( RendererContext& ctx );

		/* @brief Submits a point light to be included in the current frame's lighting.
		*  @param light Point light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddPointLight( const PointLight& light );

		/* @brief Submits a spot light to be included in the current frame's lighting.
		*  @param light Spot light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddSpotLight( const SpotLight& light );

		/* @brief Sets the active directional light for the current frame.
		*  @param light Directional light to set.
		*/
		void SetDirectionalLight( const DirectionalLight& light );

		DirectionalLight GetDirectionalLight( );
		DirectionalLight GetDirectionalLight( ) const;

		const std::pair<PointLightsArr, uint32>& GetPointLights( ) const { 
			return { m_PointLights, m_ActivePointLights }; 
		}
		const std::pair<SpotLightsArr, uint32>& GetSpotLights( ) const { 
			return { m_SpotLights, m_ActiveSpotLights }; 
		}

		/* @brief Should be called at the start of each frame before submitting new lights. */
		LUM_FORCEINLINE void ClearLights( ) { 
			m_ActivePointLights = 0; 
			m_ActiveSpotLights = 0; 
		}

		/* @brief Binds GBuffer textures, shadow map and light uniforms, then issues the fullscreen quad draw call.
		*  @param gbuffer GBuffer containing geometry data from the geometry pass.
		*  @param quad    Fullscreen quad VAO to draw the lighting onto.
		*  @param desc    IBL and shadow map handles required for lighting.
		*/
		void Execute( const detail::DeferredBuffer& gbuffer, const detail::ScreenQuad& quad, const LightPassExectueContext& desc );

	private:

		/* @brief Byte offsets into the light SSBO for each data section. */
		static constexpr usize sk_OffsetPointLights = 0;
		static constexpr usize sk_OffsetSpotLights = sizeof( PointLight ) * LUM_MAX_LIGHTS;
		static constexpr usize sk_OffsetActivePoint = sk_OffsetSpotLights + sizeof( SpotLight ) * LUM_MAX_LIGHTS;
		static constexpr usize sk_OffsetActiveSpot = sk_OffsetActivePoint + sizeof( int32 );

		/* @brief Cached context holding all subsystem manager references. */
		SafePtr<RendererContext> m_Ctx = nullptr;

		/* @brief Array of active point lights for this frame. */
		PointLightsArr m_PointLights{};

		/* @brief Array of active spot lights for this frame. */
		SpotLightsArr m_SpotLights{};

		/* @brief Number of currently active point lights. */
		uint32 m_ActivePointLights = 0;

		/* @brief Number of currently active spot lights. */
		uint32 m_ActiveSpotLights = 0;

		/* @brief GPU-ready uniform buffer representation of the active directional light. */
		detail::DirectionalLightGPU m_DirectionalLightData{};

		/* @brief Shader storage buffer holding all active point and spot lights. */
		rhi::BufferHandle m_LightsUbo{};

		/* @brief Uniform buffer holding the active directional light data. */
		rhi::BufferHandle m_DirectionalLightUbo{};

		rhi::PipelineHandle m_Pipeline{};
		rhi::ShaderHandle	m_Shader{};

		/* @brief Allocates GPU buffers and initializes pipeline and shader. */
		void init( );
		void upload_point_lights( );
		void upload_spot_lights( );
		void upload_directional_light( );

	};

} // namespace lum::render