//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Pipeline configuration for the Rendering Hardware Interface.
//
//=============================================================================//
#pragma once

#include "Rhi/RhiCommon.hpp"
#include "Rhi/Core/RhiStates.hpp"

namespace lum::rhi {

	/* @brief Descriptor used to configure and create a graphics pipeline state object (PSO). */
	struct PipelineCreateInfo {

		RasterizerState		m_Rasterizer{};   /* @brief Polygon rasterization mode (fill, line, point, depth bias). */
		DepthStencilState	m_DepthStencil{}; /* @brief Depth testing, depth writing, and stencil operations state. */
		ScissorState		m_Scissor{};      /* @brief Scissor test bounds and enable state. */
		CullState			m_Cull{};         /* @brief Backface/frontface culling mode and winding order. */
		BlendState			m_Blend{};        /* @brief Color blending factors and equations. */
		ColorMask			m_ColorMask{};    /* @brief Write mask per color channel (RGBA). */

		ShaderHandle		m_Shader{};       /* @brief Handle to the bound shader program. */

	};

	/* @brief Represents an immutable created graphics pipeline state object. */
	struct Pipeline {

		RasterizerState		m_Rasterizer{};   /* @brief Active rasterizer state. */
		DepthStencilState	m_DepthStencil{}; /* @brief Active depth and stencil state. */
		ScissorState		m_Scissor{};      /* @brief Active scissor state. */
		CullState			m_Cull{};         /* @brief Active face culling state. */
		BlendState			m_Blend{};        /* @brief Active blending state. */
		ColorMask			m_ColorMask{};    /* @brief Active color channel write mask. */

		ShaderHandle		m_Shader{};       /* @brief Handle to the compiled shader program associated with this pipeline. */

	};

} // namespace lum::rhi