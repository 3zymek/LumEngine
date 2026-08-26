//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Pipeline configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once
#include "Rhi/RhiCommon.hpp"
#include "Rhi/Core/RhiStates.hpp"

namespace lum::rhi {

	struct PipelineCreateInfo {

		RasterizerState		m_Rasterizer		{};
		DepthStencilState		m_DepthStencil	{};
		ScissorState			m_Scissor		{};
		CullState				m_Cull			{};
		BlendState				m_Blend			{};
		ColorMask				m_ColorMask		{};

		ShaderHandle			m_Shader{};

	};

	struct Pipeline {

		RasterizerState		m_Rasterizer		{};
		DepthStencilState		m_DepthStencil	{};
		ScissorState			m_Scissor		{};
		CullState				m_Cull			{};
		BlendState				m_Blend			{};
		ColorMask				m_ColorMask		{};

		ShaderHandle			m_Shader{};

	};

}