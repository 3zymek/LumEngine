//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines the shadow rendering system responsible for generating
//          shadow maps for supported light types.
//
//=============================================================================//
#pragma once

#include "Render/RenderCommon.hpp"

namespace lum::render {

	class GeometryPass;
	class LightPass;

	class LUM_API ShadowSystem {
	public:


		ShadowSystem( ) = default;

		/* @brief Initializes shadow rendering resources.
		* @param ctx Renderer context containing subsystem references.
		*/
		void Initialize( RendererContext& ctx );

		/* @brief Executes all shadow rendering passes.
		* @param geoPass Geometry pass providing visible geometry.
		* @param lightPass Light pass providing active light sources.
		*/
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

			/* @brief Returns the generated shadow map texture.
			* @return Shadow map texture handle.
			*/
			rhi::TextureHandle GetShadowMap( ) const noexcept { return m_ShadowMap; }

		private:

			/* @brief Shadow map texture dimensions in pixels. */
			Vector2u m_ShadowMapTexSize = { 4096, 4096 };

			/* @brief Distance from the scene origin used to position the light. */
			float32 m_ShadowMapDistance = 50.f;

			/* @brief Half-size of the orthographic projection frustum. */
			float32 m_ShadowMapSize = 100.f;

			/* @brief Near plane of the light space orthographic projection. */
			float32 m_ShadowMapNear = 0.1f;

			/* @brief Far plane of the light space orthographic projection. */
			float32 m_ShadowMapFar = m_ShadowMapDistance * 2.0f;

			/* @brief Depth texture used as the shadow map attachment. */
			rhi::TextureHandle m_ShadowMap{};

			/* @brief Framebuffer with the shadow map as its depth attachment. */
			rhi::FramebufferHandle m_Framebuffer{};

			/* @brief Shader program used during the shadow pass. */
			rhi::ShaderHandle m_Shader{};

			/* @brief Uniform buffer holding the light space transformation matrix. */
			rhi::BufferHandle m_LightSpaceUbo{};

			/* @brief Computes and uploads the light space matrix from the given light direction.
			*  @param direction Normalized direction vector of the directional light.
			*/
			void calculate_lightspace_matrix( const Vector3& dir, RendererContext& ctx );

			/* @brief Uploads the light space matrix to the GPU uniform buffer.
			*  @param mat Light space transformation matrix to upload.
			*/
			void upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx );

		} m_DirectionalLight{};




		class SpotLight {
		public:
			
			/* @brief Initializes GPU resources required for spot light shadow mapping.
			* @param ctx Renderer context.
			*/
			void Initialize( RendererContext& ctx );

			/* @brief Generates shadow maps for all visible spot lights.
			* @param geoPass Geometry pass providing scene geometry.
			* @param lightPass Light pass providing active spot lights.
			* @param ctx Renderer context.
			*/
			void Execute( GeometryPass& geoPass, const LightPass& lightPass, RendererContext& ctx );
			
			/* @brief Returns the shadow map texture array.
			* @return Shadow map texture handle.
			*/
			LUM_NODISCARD rhi::TextureHandle GetShadowMaps( ) const noexcept { return m_ShadowMaps; }
			
		private:
			
			/* @brief Shadow map texture dimensions in pixels. */
			Vector2u m_ShadowMapTexSize = { 1024, 1024 };

			/* @brief Near plane of the light space projection. */
			float32 m_ShadowMapNear = 0.1f;

			/* @brief Far plane of the light space projection. */
			float32 m_ShadowMapFar = 100.0f;

			/* @brief Depth texture used as the shadow maps attachments. */
			rhi::TextureHandle m_ShadowMaps{};

			/* @brief Framebuffer with the shadow maps as its depth attachment. */
			rhi::FramebufferHandle m_Framebuffer{};

			/* @brief Shader program used during the shadow pass. */
			rhi::ShaderHandle m_Shader{};

			/* @brief Uniform buffer holding the light space transformation matrix. */
			rhi::BufferHandle m_LightSpaceUbo{};
			
			/* @brief Computes the light space matrix for a spot light.
			* @param dir Light direction.
			* @param pos Light position.
			* @param fov Spot light field of view.
			*/
			void calculate_lightspace_matrix( const Vector3& dir, const Vector3& pos, float32 fov, RendererContext& ctx );

			/* @brief Uploads the light space matrix to the GPU.
			* @param mat Light space transformation matrix.
			*/
			void upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx );
			
		} m_SpotLight{};




	private:

		SafePtr<RendererContext> m_Ctx = nullptr;

		rhi::PipelineHandle m_ShadowMappingPipeline{};

	};

}