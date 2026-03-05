//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Geometry render pass — handles per-object draw calls,
//          model matrix and material uniform uploads.
//
//=============================================================================//
#pragma once

#include "render/common.hpp"

namespace lum::render {

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

		/* @brief Uploads uniforms and issues a draw call for the given render instance.
		*  @param instance Render instance containing transform, mesh and material.
		*/
		void Submit( const FRenderInstance& instance );

		/* @brief Begins the geometry pass — binds pipeline and prepares state. */
		void Execute( );

	private:

		std::vector<FRenderInstance> mInstances;

		inline constexpr static uint32 mTempSize = 64;
		std::vector<FRenderInstance> mTempInstances;

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief CPU-side material uniform buffer, uploaded per draw call. */
		detail::FMaterialUniformBuffer mMaterialUBO{};

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

		void draw_instance(const FRenderInstance& instance);

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