//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Pipeline configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_states.hpp"

namespace lum::rhi {

	struct PipelineDescriptor {

		RasterizerState			mRasterizer		{};
		DepthStencilState		mDepthStencil	{};
		ScissorState			mScissor		{};
		CullState				mCull			{};
		BlendState				mBlend			{};
		ColorMask				mColorMask		{};

		ShaderHandle			mShader{};

	};

	struct Pipeline {

		RasterizerState			mRasterizer		{};
		DepthStencilState		mDepthStencil	{};
		ScissorState			mScissor		{};
		CullState				mCull			{};
		BlendState				mBlend			{};
		ColorMask				mColorMask		{};

		ShaderHandle			mShader{};

	};

}