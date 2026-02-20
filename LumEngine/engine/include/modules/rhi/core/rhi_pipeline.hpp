//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Pipeline configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_states.hpp"

namespace lum::rhi {

	struct RPipelineDescriptor {

		RRasterizerState		mRasterizer		{};
		RDepthStencilState		mDepthStencil	{};
		RScissorState			mScissor		{};
		RCullState				mCull			{};
		RBlendState				mBlend			{};
		RColorMask				mColorMask		{};

		RShaderHandle			mShader{};

	};

	struct RPipeline {

		RRasterizerState			mRasterizer		{};
		RDepthStencilState		mDepthStencil	{};
		RScissorState			mScissor		{};
		RCullState				mCull			{};
		RBlendState				mBlend			{};
		RColorMask				mColorMask		{};

		RShaderHandle			mShader{};

	};

}