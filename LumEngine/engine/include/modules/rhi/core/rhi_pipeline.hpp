//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Pipeline configuration for the Rendering Hardware Interface
//
//=============================================================================//
#pragma once
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_states.hpp"

namespace lum::rhi {

	struct FPipelineDescriptor {

		FRasterizerState		mRasterizer		{};
		FDepthStencilState		mDepthStencil	{};
		FScissorState			mScissor		{};
		FCullState				mCull			{};
		FBlendState				mBlend			{};
		FColorMask				mColorMask		{};

		RShaderHandle			mShader{};

	};

	struct FPipeline {

		FRasterizerState		mRasterizer		{};
		FDepthStencilState		mDepthStencil	{};
		FScissorState			mScissor		{};
		FCullState				mCull			{};
		FBlendState				mBlend			{};
		FColorMask				mColorMask		{};

		RShaderHandle			mShader{};

	};

}