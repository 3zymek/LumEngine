#pragma once
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_states.hpp"
namespace lum::rhi {

	struct alignas(8) PipelineHandle {

		pipelineid id = max_val<pipelineid>();
		pipelineid generation = 0;

	};

	struct PipelineDescriptor {

		RasterizerState rasterizer	{};
		DepthState		depth		{};
		StencilState	stencil		{};
		ScissorState	scissor		{};
		CullState		cull		{};
		
		
		bool blendEnabled = false;


		bool cullEnabled = false;

	};

	struct Pipeline {

		RasterizerState rasterizer	{};
		DepthState		depth		{};
		StencilState	stencil		{};
		ScissorState	scissor		{};
		CullState		cull		{};


		bool blendEnabled = false;


		bool cullEnabled = false;

	};

}