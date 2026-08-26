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

		std::vector<RenderInstance> m_Instances;

		inline constexpr static uint32 m_TempSize = 64;

		std::vector<RenderInstance> m_TempInstances;

		/* @brief Cached context holding all subsystem manager references. */
		SafePtr<RendererContext> m_Ctx = nullptr;

		/* @brief CPU-side material uniform buffer, uploaded per draw call. */
		detail::MaterialGPU m_MaterialUbo{};

		/* @brief GPU buffer holding the per-draw model matrix uniform. */
		rhi::BufferHandle m_ModelUniform;

		/* @brief GPU buffer holding the per-draw material uniform. */
		rhi::BufferHandle m_MaterialUniform;

		rhi::PipelineHandle m_Pipeline;
		rhi::ShaderHandle m_Shader;

		/* @brief Allocates GPU buffers and initializes pipeline and shader. */
		void init( );
		void draw_instance( const RenderInstance& instance );
		void draw_mesh( const RenderInstance& instance );
		void upload_model_matrix( const RenderInstance& instance );
		void upload_material( const CMaterialInstance& mat );

	};
} // namespace lum::render