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

		RasterizerState		mRasterizer		{};
		DepthStencilState		mDepthStencil	{};
		ScissorState			mScissor		{};
		CullState				mCull			{};
		BlendState				mBlend			{};
		ColorMask				mColorMask		{};

		ShaderHandle			mShader{};

	};

	struct Pipeline {

		RasterizerState		mRasterizer		{};
		DepthStencilState		mDepthStencil	{};
		ScissorState			mScissor		{};
		CullState				mCull			{};
		BlendState				mBlend			{};
		ColorMask				mColorMask		{};

		ShaderHandle			mShader{};

	};

}