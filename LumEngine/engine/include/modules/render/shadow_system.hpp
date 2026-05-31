#pragma once

#include "render/render_common.hpp"

namespace lum::render {

	class GeometryPass;
	class LightPass;

	class ShadowSystem {
	public:

		ShadowSystem( ) = default;

		void Initialize( const RendererContext& ctx );

		void Execute( GeometryPass& geoPass, const LightPass& lightPass );

		// Shadow map generation pass for the directional light.
		class DirectionalLight {
		public:

			/* @brief Initializes the pass, allocates GPU resources and compiles shaders.
			*  @param ctx Context struct containing valid pointers to all subsystem managers.
			*/
			void Initialize( RendererContext& ctx );
			
			/* @brief Renders the scene from the light's perspective and writes the depth to the shadow map.
			*  @param geoPass Geometry pass providing the scene render queue.
			*  @param lightPass Light pass providing the active directional light direction.
			*  @param ctx Renderer context.
			*/
			void Execute( GeometryPass& geoPass, const LightPass& lightPass, RendererContext& ctx );

			rhi::TextureHandle GetShadowMap( ) const noexcept { return mShadowMap; }

		private:

			/* @brief Shadow map texture dimensions in pixels. */
			Vector2u mShadowMapTexSize = { 4096, 4096 };

			/* @brief Distance from the scene origin used to position the light. */
			float32 mShadowMapDistance = 50.f;

			/* @brief Half-size of the orthographic projection frustum. */
			float32 mShadowMapSize = 100.f;

			/* @brief Near plane of the light space orthographic projection. */
			float32 mShadowMapNear = 0.1f;

			/* @brief Far plane of the light space orthographic projection. */
			float32 mShadowMapFar = mShadowMapDistance * 2.0f;

			/* @brief Depth texture used as the shadow map attachment. */
			rhi::TextureHandle mShadowMap;

			/* @brief Framebuffer with the shadow map as its depth attachment. */
			rhi::FramebufferHandle mFramebuffer;

			/* @brief Shader program used during the shadow pass. */
			rhi::ShaderHandle mShader;

			/* @brief Uniform buffer holding the light space transformation matrix. */
			rhi::BufferHandle mLightSpaceUBO;

			/* @brief Computes and uploads the light space matrix from the given light direction.
			*  @param direction Normalized direction vector of the directional light.
			*/
			void calculate_lightspace_matrix( const Vector3& dir, RendererContext& ctx );

			/* @brief Uploads the light space matrix to the GPU uniform buffer.
			*  @param mat Light space transformation matrix to upload.
			*/
			void upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx );

		} mDirectionalLight;

		class SpotLight {
		public:
			
			void Initialize( RendererContext& ctx );

			void Execute( GeometryPass& geoPass, const LightPass& lightPass, RendererContext& ctx );
			
			rhi::TextureHandle GetShadowMaps( ) const noexcept { return mShadowMaps; }
			
		private:
			
			/* @brief Shadow map texture dimensions in pixels. */
			Vector2u mShadowMapTexSize = { 1024, 1024 };

			/* @brief Near plane of the light space projection. */
			float32 mShadowMapNear = 0.1f;

			/* @brief Far plane of the light space projection. */
			float32 mShadowMapFar = 100.0f;

			/* @brief Depth texture used as the shadow maps attachments. */
			rhi::TextureHandle mShadowMaps;

			/* @brief Framebuffer with the shadow maps as its depth attachment. */
			rhi::FramebufferHandle mFramebuffer;

			/* @brief Shader program used during the shadow pass. */
			rhi::ShaderHandle mShader;

			/* @brief Uniform buffer holding the light space transformation matrix. */
			rhi::BufferHandle mLightSpaceUBO;
			
			void calculate_lightspace_matrix( const Vector3& dir, const Vector3& pos, float32 fov, RendererContext& ctx );

			void upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx );
			
		} mSpotLight;


	private:

		RendererContext mContext;

		rhi::PipelineHandle mShadowMappingPipeline;

	};

}