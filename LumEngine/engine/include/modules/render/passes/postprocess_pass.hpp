//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: 
//
//=============================================================================//
#pragma once

#include "render/render_common.hpp"

namespace lum::render {

	class PostprocessPass {
	public:

		PostprocessPass( ) = default;

		void Initialize( const FRendererContext& ctx );

		void Execute( const detail::FScreenQuad& quad );


	private:

		FRendererContext mContext;

		/* @brief Shader used for the final post-process blit to the default framebuffer. */
		rhi::RShaderHandle mShader;

		

	};


} // namespace lum::render