//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Geometry render pass — handles per-object draw calls,
//          model matrix and material uniform uploads.
//
//=============================================================================//
#pragma once

#include "render/render_common.hpp"

namespace lum::render {
	
	namespace detail { class GBuffer; }

	/* @brief Handles the main geometry render pass — uploads per-draw uniforms
	*  and issues draw calls for all submitted render instances.
	*/
	class GeometryPass {
	public:

		GeometryPass( ) = default;

		/* @brief Initializes the pass, allocates GPU buffers and compiles shaders.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const FRendererContext& ctx );

		/* @brief Enqueues a render instance for drawing in the current frame.
		*  @param instance Render instance containing transform, mesh and material.
		*/
		void Submit( const FRenderInstance& instance );

		/* @brief Binds the GBuffer, pipeline and shader, then issues all queued draw calls.
		*  @param gbuffer GBuffer to render geometry into.
		*/
		void Execute( const detail::GBuffer& gbuffer );

		/* @brief Issues draw calls for all queued instances without binding any pipeline state.
		*  Used by other passes (e.g. ShadowPass) that manage their own shader and pipeline.
		*/
		void DrawScene( );

	private:

		std::vector<FRenderInstance> mInstances;

		inline constexpr static uint32 mTempSize = 64;

		std::vector<FRenderInstance> mTempInstances;

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief CPU-side material uniform buffer, uploaded per draw call. */
		detail::FMaterialUBOData mMaterialUBO{};

		/* @brief GPU buffer holding the per-draw model matrix uniform. */
		rhi::RBufferHandle mModelUniform;

		/* @brief GPU buffer holding the per-draw material uniform. */
		rhi::RBufferHandle mMaterialUniform;

		/* @brief Pipeline state for geometry rendering. */
		rhi::RPipelineHandle mPipeline;

		/* @brief Shader program used for geometry rendering. */
		rhi::RShaderHandle mShader;

		/* @brief Allocates GPU buffers and initializes pipeline and shader. */
		void init( );

		/* @brief Uploads material uniforms, binds textures and issues a draw call for the given instance. */
		void draw_instance( const FRenderInstance& instance );

		/* @brief Uploads the model matrix and issues a draw call without binding any material state. */
		void draw_mesh( const FRenderInstance& instance );

		/* @brief Computes and uploads the model matrix for the given instance.
		*  @param instance Render instance to compute the model matrix from.
		*/
		void upload_model_matrix( const FRenderInstance& instance );

		/* @brief Uploads material parameters for the given material instance.
		*  @param mat Material instance to upload.
		*/
		void upload_material( const FMaterialInstance& mat );

	};
} // namespace lum::render