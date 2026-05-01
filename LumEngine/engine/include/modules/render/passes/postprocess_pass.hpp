//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: 
//
//=============================================================================//
#pragma once

#include "render/render_common.hpp"

namespace lum::render {

	struct PostprocessPassExecute {
		bool bTAAEnabled = false;
		rhi::RTextureHandle mPreviousFrameTex;
	};

	class PostprocessPass {
	public:

		PostprocessPass( ) = default;

		void Initialize( const FRendererContext& ctx );

		void Execute( const detail::FScreenQuad& quad, const PostprocessPassExecute& exe );


	private:

		FRendererContext mContext;

		/* @brief Shader used for the final post-process blit to the default framebuffer. */
		rhi::RShaderHandle mShader;

		

	};


} // namespace lum::render