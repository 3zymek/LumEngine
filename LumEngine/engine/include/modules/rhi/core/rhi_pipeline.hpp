#pragma once
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_states.hpp"
namespace lum::rhi {

	struct alignas(8) PipelineHandle {

		pipelineid id = max_val<pipelineid>();
		pipelineid generation = 0;

	};

	struct PipelineDescriptor {

		RasterizerState			rasterizer	{};
		DepthStencilState		depthStencil{};
		ScissorState			scissor		{};
		CullState				cull		{};

	};

	struct Pipeline {

		RasterizerState			rasterizer	{};
		DepthStencilState		depthStencil{};
		ScissorState			scissor		{};
		CullState				cull		{};

	};

}