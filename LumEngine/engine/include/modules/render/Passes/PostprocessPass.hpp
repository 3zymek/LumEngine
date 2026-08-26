//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: 
//
//=============================================================================//
#pragma once

#include "Render/RenderCommon.hpp"

namespace lum::render {

	struct PostprocessPassExecute {
		bool m_TAAEnabled = false;
		rhi::TextureHandle m_PreviousFrameTex;
		Vector2 m_JitterOffset{};
	};

	class PostprocessPass {
	public:

		PostprocessPass( ) = default;

		void Initialize( RendererContext& ctx );

		void Execute( const detail::ScreenQuad& quad, const PostprocessPassExecute& exe );


	private:

		SafePtr<RendererContext> m_Ctx = nullptr;

		/* @brief Shader used for the final post-process blit to the default framebuffer. */
		rhi::ShaderHandle m_Shader;

		

	};


} // namespace lum::render