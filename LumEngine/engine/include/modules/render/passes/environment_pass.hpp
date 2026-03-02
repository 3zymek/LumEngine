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
	class EnvironmentPass {
	public:

		EnvironmentPass() = default;

		/* @brief Initializes the pass, allocates GPU buffers and compiles shaders.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const FRendererContext& ctx );

		void SetCubemapTexture(rhi::RTextureHandle tex) { mCubemap.mTexture = tex; }

		/* @brief Begins the geometry pass — binds pipeline and prepares state. */
		void BeginPass();

		/* @brief Ends the geometry pass. */
		void EndPass();

	private:

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		detail::FCubemapData mCubemap;

		/* @brief Pipeline state for geometry rendering. */
		rhi::RPipelineHandle mPipeline;

		/* @brief Shader program used for geometry rendering. */
		rhi::RShaderHandle mShader;

		/* @brief Allocates GPU buffers and initializes pipeline and shader. */
		void init();

	};

} // namespace lum::render