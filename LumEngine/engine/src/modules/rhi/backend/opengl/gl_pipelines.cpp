// ************************************
// LumEngine Copyright (C) 2026 3zymek
// All rights reserved.
// Pipelines implementation for OpenGL RHI
// ************************************

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	void GLDevice::bind_check_shader ( const Pipeline& pip ) noexcept {
		
		if (pip.shader.id == null_id<ShaderID>())
			return;

		BindShader(pip.shader);

	}

	void GLDevice::bind_check_rasterizer ( const Pipeline& pip ) noexcept {

		const auto& rast = pip.mRasterizer;

		SetTopology(rast.polygon.topologyMode, rast.polygon.topologyModeFaces);
		SetLineWidth(rast.polygon.lineWidth);
		SetPointSize(rast.polygon.pointSize);

		ToggleDepthBias(rast.depthBias.bEnable);

		SetDepthBiasSlope(rast.depthBias.slopeFactor);
		SetDepthBiasClamp(rast.depthBias.clamp);

	}
	void GLDevice::bind_check_depth_stencil ( const Pipeline& pip ) noexcept {

		const auto& depth = pip.mDepthStencil.depth;
		const auto& stencil = pip.mDepthStencil.stencil;

		ToggleDepthTest(depth.bEnabled);

		SetDepthFunc(depth.compareFlag);

		ToggleStencilTest(stencil.bEnabled);
		SetStencilReference(stencil.front.reference, Face::Front);
		SetStencilReference(stencil.back.reference, Face::Back);
		SetStencilOp(stencil.front.stencilFailOp, stencil.front.depthFailOp, stencil.front.passOp, Face::Front);
		SetStencilOp(stencil.back.stencilFailOp, stencil.back.depthFailOp, stencil.back.passOp, Face::Back);
		
	}
	void GLDevice::bind_check_scissors ( const Pipeline& pip ) noexcept {

		const auto& scissors = pip.mScissor;

		ToggleScissors(pip.mScissor.bEnabled);
		SetScissors(scissors.x, scissors.y, scissors.width, scissors.height); // Default options

	}
	void GLDevice::bind_check_blend ( const Pipeline& pip ) noexcept {

		const auto& blend = pip.mBlend;

		ToggleBlend(blend.bEnabled);

		SetBlendFactors(blend.srcColorFactor, blend.dstColorFactor, blend.srcAlphaFactor, blend.dstAlphaFactor);
		SetBlendOp(blend.colorOp, blend.alphaOp);

	}
	void GLDevice::bind_check_cull ( const Pipeline& pip ) noexcept {

		const auto& cull = pip.mCull;

		ToggleCull(cull.bEnabled);

		SetCullFace(cull.face);
		SetFrontFace(cull.windingOrder);

	}
	void GLDevice::bind_check_color_mask ( const Pipeline& pip ) noexcept {

		const auto& mask = pip.mColorMask;

		SetColorMask(mask);

	}


	///////////////////////////////////////////////////
	/// Pipelines
	///////////////////////////////////////////////////

	PipelineHandle GLDevice::CreatePipeline ( const PipelineDescriptor& desc ) {

		LUM_HOTCHK_RETURN_CUSTOM(
			mPipelines.dense_size() <= skMaxPipelines,
			LUM_SEV_WARN,
			PipelineHandle{},
			"Max pipelines reached"
		);

		if (desc.shader.id != null_id<PipelineID>() && !mShaders.exist(desc.shader)) {
			LUM_LOG_ERROR("Shader %d doesn't exist", desc.shader.id);
		}

		Pipeline pipeline;
		std::memcpy(&pipeline, &desc, sizeof(desc));

		return mPipelines.create_handle(std::move(pipeline));

	}
	void GLDevice::DeletePipeline(PipelineHandle& pipeline) {

		LUM_HOTCHK_RETURN_VOID(mPipelines.exist(pipeline), LUM_SEV_WARN, "Pipeline doesn't exist");

		mPipelines.delete_handle(pipeline);

	}
	void GLDevice::BindPipeline(const PipelineHandle& pipeline) {

		LUM_HOTCHK_RETURN_VOID(mPipelines.exist(pipeline), LUM_SEV_WARN, "Pipeline doesn't exist");

		if (pipeline == mCurrentPipeline) { 
			LUM_PROFILER_CACHE_HIT();
			return;
		}
		mCurrentPipeline = pipeline;

		Pipeline& pip = mPipelines[pipeline];
	   
		// Shader
		bind_check_shader ( pip );

		// Color mask
		bind_check_color_mask ( pip );

		// Topology
		bind_check_rasterizer ( pip );

		// Depth & Stencil
		bind_check_depth_stencil ( pip );

		// Scissors
		bind_check_scissors ( pip );

		// Cull
		bind_check_cull ( pip );

		// Blend
		bind_check_blend ( pip );

	}

}