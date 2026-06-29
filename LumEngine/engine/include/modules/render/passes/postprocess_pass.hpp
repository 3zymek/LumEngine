//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: 
//
//=============================================================================//
#pragma once

#include "render/render_common.hpp"

namespace lum::render {

	struct PostprocessPassExecute {
		bool mTAAEnabled = false;
		rhi::TextureHandle mPreviousFrameTex;
		Vector2 mJitterOffset{};
	};

	class PostprocessPass {
	public:

		PostprocessPass( ) = default;

		void Initialize( const RendererContext& ctx );

		void Execute( const detail::ScreenQuad& quad, const PostprocessPassExecute& exe );


	private:

		RendererContext mContext;

		/* @brief Shader used for the final post-process blit to the default framebuffer. */
		rhi::ShaderHandle mShader;

		

	};


} // namespace lum::render