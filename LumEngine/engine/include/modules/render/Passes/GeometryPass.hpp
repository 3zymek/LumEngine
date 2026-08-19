//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Geometry render pass — handles per-object draw calls,
//          model matrix and material uniform uploads.
//
//=============================================================================//
#pragma once

#include "Render/RenderCommon.hpp"

namespace lum::render {
	
	namespace detail { class DeferredBuffer; }

	/* @brief Handles the main geometry render pass — uploads per-draw uniforms
	*  and issues draw calls for all submitted render instances.
	*/
	class GeometryPass {
	public:

		GeometryPass( ) = default;

		/* @brief Initializes the pass, allocates GPU buffers and compiles shaders.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( RendererContext& ctx );

		/* @brief Enqueues a render instance for drawing in the current frame.
		*  @param instance Render instance containing transform, mesh and material.
		*/
		void Submit( const RenderInstance& instance );

		/* @brief Binds the DefferedBuffer, pipeline and shader, then issues all queued draw calls.
		*  @param gbuffer DefferedBuffer to render geometry into.
		*/
		void Execute( const detail::DeferredBuffer& gbuffer );

		/* @brief Issues draw calls for all queued instances without binding any pipeline state.
		*  Used by other passes (e.g. ShadowPass) that manage their own shader and pipeline.
		*/
		void DrawScene( );

	private:

		std::vector<RenderInstance> mInstances;

		inline constexpr static uint32 mTempSize = 64;

		std::vector<RenderInstance> mTempInstances;

		/* @brief Cached context holding all subsystem manager references. */
		SafePtr<RendererContext> mCtx = nullptr;

		/* @brief CPU-side material uniform buffer, uploaded per draw call. */
		detail::MaterialGPU mMaterialUbo{};

		/* @brief GPU buffer holding the per-draw model matrix uniform. */
		rhi::BufferHandle mModelUniform;

		/* @brief GPU buffer holding the per-draw material uniform. */
		rhi::BufferHandle mMaterialUniform;

		rhi::PipelineHandle mPipeline;
		rhi::ShaderHandle mShader;

		/* @brief Allocates GPU buffers and initializes pipeline and shader. */
		void init( );
		void draw_instance( const RenderInstance& instance );
		void draw_mesh( const RenderInstance& instance );
		void upload_model_matrix( const RenderInstance& instance );
		void upload_material( const CMaterialInstance& mat );

	};
} // namespace lum::render