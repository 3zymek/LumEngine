//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//
#pragma once

#include "render/common.hpp"

namespace lum::render {

	/* @brief Handles the main geometry render pass — uploads per-draw uniforms
	*  and issues draw calls for all submitted render instances.
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

		LUM_FORCEINLINE void SetDirectionalLight( const FDirectionalLight& light ) {
			mDirectionalLightStruct.mColor = glm::vec4(light.mColor, 0.0);
			mDirectionalLightStruct.mDirection = glm::vec4(light.mDirection, 0.0);
			mDirectionalLightStruct.mIntensity = light.mIntensity;

		}

		/* @brief Returns the currently active directional light. */
		LUM_FORCEINLINE FDirectionalLight GetDirectionalLight() {
			return {
				glm::vec3(mDirectionalLightStruct.mDirection),
				mDirectionalLightStruct.mIntensity,
				glm::vec3(mDirectionalLightStruct.mColor)
			};
		}

		void BindShader( ) { mContext.mRenderDevice->BindShader(mShader); }

		/* @brief Begins the geometry pass — binds pipeline and prepares state. */
		void ClearLights( );

		/* @brief Ends the geometry pass. */
		void UploadLights( );

	private:

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief Active directional light for this frame. */
		FDirectionalLight mDirectionalLight;

		/* @brief Array of active point lights for this frame. */
		std::array<FPointLight, LUM_MAX_LIGHTS> mPointLights{};

		/* @brief Number of currently active point lights. */
		uint32 mActivePoints = 0;

		/* @brief GPU-ready uniform buffer representation of the active directional light. */
		detail::FDirectionalLightUniformBuffer mDirectionalLightStruct{};

		/* @brief Shader storage buffer holding all active point lights. */
		rhi::RBufferHandle mPointLightsBuffer;

		/* @brief Uniform buffer holding the active directional light data. */
		rhi::RBufferHandle mDirectionalLightBuffer;

		/* @brief Pipeline state for geometry rendering. */
		rhi::RPipelineHandle mPipeline;

		/* @brief Shader program used for geometry rendering. */
		rhi::RShaderHandle mShader;

		/* @brief Allocates GPU buffers and initializes pipeline and shader. */
		void init( );
		
		/* @brief Resets the point light counter, discarding all lights from the previous frame. */
		LUM_FORCEINLINE void clear_point_lights( ) { mActivePoints = 0; }
		
		/* @brief Uploads the light-space transformation matrix used for shadow mapping.
		*  @param mat Light-space matrix to upload.
		*/
		void upload_lightspace_matrix( const glm::mat4& mat );
		
		/* @brief Uploads all active point lights to the GPU shader storage buffer. */
		void upload_point_lights( );

		/* @brief Uploads the active directional light to its GPU uniform buffer. */
		void upload_directional_light( );
	};

} // namespace lum::render::detail