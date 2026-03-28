//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Shadow map generation pass for the directional light.
//
//=============================================================================//
#pragma once

#include "render/render_common.hpp"

namespace lum::render {

	class GeometryPass;
	class LightPass;

	/* @brief Manages shadow map generation for the directional light.
	*  Renders the scene from the light's perspective into a depth texture
	*  and uploads the light space matrix for use in the lighting pass.
	*/
	class ShadowPass {
	public:

		ShadowPass( ) = default;

		/* @brief Initializes the pass, allocates GPU resources and compiles shaders.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const FRendererContext& ctx );

		/* @brief Returns the shadow map depth texture produced by this pass. */
		rhi::RTextureHandle GetShadowMap( ) const noexcept { return mShadowMap; }

		/* @brief Renders the scene from the light's perspective and writes the depth to the shadow map.
		*  @param geometryPass Geometry pass providing the scene render queue.
		*  @param lightPass Light pass providing the active directional light direction.
		*/
		void Execute( GeometryPass& geometryPass, const LightPass& lightPass );

	private:

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief Shadow map texture dimensions in pixels. */
		glm::vec2 mShadowMapTexSize = { 4096, 4096 };

		/* @brief Distance from the scene origin used to position the light. */
		float32 mShadowMapDistance = 50.f;

		/* @brief Half-size of the orthographic projection frustum. */
		float32 mShadowMapSize = 100.f;

		/* @brief Near plane of the light space orthographic projection. */
		float32 mShadowMapNear = 0.1f;

		/* @brief Far plane of the light space orthographic projection. */
		float32 mShadowMapFar = mShadowMapDistance * 2.0f;

		/* @brief Depth texture used as the shadow map attachment. */
		rhi::RTextureHandle mShadowMap;

		/* @brief Framebuffer with the shadow map as its depth attachment. */
		rhi::RFramebufferHandle mShadowFramebuffer;

		/* @brief Shader program used during the shadow pass. */
		rhi::RShaderHandle mShader;

		/* @brief Uniform buffer holding the light space transformation matrix. */
		rhi::RBufferHandle mLightSpaceUBO;

		/* @brief Pipeline ensuring that everyting is prepared to generate shadow map. */
		rhi::RPipelineHandle mShadowPipeline;

		/* @brief Computes and uploads the light space matrix from the given light direction.
		*  @param direction Normalized direction vector of the directional light.
		*/
		void calculate_lightspace_matrix( const glm::vec3& direction );

		/* @brief Uploads the light space matrix to the GPU uniform buffer.
		*  @param mat Light space transformation matrix to upload.
		*/
		void upload_lightspace_matrix( const glm::mat4& mat );

		/* @brief Allocates GPU resources and initializes the shadow map framebuffer and shader. */
		void init( );

	};

} // namespace lum::render