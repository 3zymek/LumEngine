#pragma once
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_states.hpp"
namespace lum::rhi {

	struct alignas(8) PipelineHandle {

		PipelineID id = null_id<PipelineID>();
		PipelineID generation = 0;

	};

	struct PipelineDescriptor {

		RasterizerState			mRasterizer		{};
		DepthStencilState		mDepthStencil	{};
		ScissorState			mScissor		{};
		CullState				mCull			{};
		BlendState				mBlend			{};
		ColorMask				mColorMask		{};

		ShaderHandle			shader{};

	};

	struct Pipeline {

		RasterizerState			mRasterizer		{};
		DepthStencilState		mDepthStencil	{};
		ScissorState			mScissor		{};
		CullState				mCull			{};
		BlendState				mBlend			{};
		ColorMask				mColorMask		{};

		ShaderHandle			shader{};

	};

}