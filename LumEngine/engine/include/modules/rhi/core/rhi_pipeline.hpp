#pragma once
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_states.hpp"
namespace lum::rhi {

	struct alignas(8) PipelineHandle {

		PipelineID id = null_id<PipelineID>();
		PipelineID generation = 0;

	};

	struct PipelineDescriptor {

		RasterizerState			rasterizer	{};
		DepthStencilState		depthStencil{};
		ScissorState			scissor		{};
		CullState				cull		{};
		BlendState				blend		{};

		ShaderHandle			shader{};

	};

	struct Pipeline {

		RasterizerState			rasterizer	{};
		DepthStencilState		depthStencil{};
		ScissorState			scissor		{};
		CullState				cull		{};
		BlendState				blend		{};

		ShaderHandle			shader{};

	};

}