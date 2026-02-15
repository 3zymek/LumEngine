//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: RHI OpenGL implementation pipeline operations and management
// 
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	void GLDevice::bind_check_shader ( const Pipeline& pip ) noexcept {
		
		if (pip.mShader.id == null_id<ShaderID>())
			return;

		BindShader(pip.mShader);

	}
	void GLDevice::bind_check_rasterizer ( const Pipeline& pip ) noexcept {

		const auto& rast = pip.mRasterizer;

		SetTopology(rast.mPolygon.mTopologyMode, rast.mPolygon.mTopologyModeFaces);
		SetLineWidth(rast.mPolygon.mLineWidth);
		SetPointSize(rast.mPolygon.mPointSize);

		ToggleDepthBias(rast.mDepthBias.bEnable);

		SetDepthBiasSlope(rast.mDepthBias.mSlopeFactor);
		SetDepthBiasClamp(rast.mDepthBias.mClamp);

	}
	void GLDevice::bind_check_depth_stencil ( const Pipeline& pip ) noexcept {

		const auto& depth = pip.mDepthStencil.mDepth;
		const auto& stencil = pip.mDepthStencil.mStencil;

		ToggleDepthTest(depth.bEnabled);

		SetDepthFunc(depth.mCompare);

		ToggleStencilTest(stencil.bEnabled);
		SetStencilReference(stencil.mFront.mReference, Face::Front);
		SetStencilReference(stencil.mBack.mReference, Face::Back);
		SetStencilOp(stencil.mFront.mStencilFailOp, stencil.mFront.mDepthFailOp, stencil.mFront.mPassOp, Face::Front);
		SetStencilOp(stencil.mBack.mStencilFailOp, stencil.mBack.mDepthFailOp, stencil.mBack.mPassOp, Face::Back);
		
	}
	void GLDevice::bind_check_scissors ( const Pipeline& pip ) noexcept {

		const auto& scissors = pip.mScissor;

		ToggleScissors(pip.mScissor.bEnabled);
		SetScissors(scissors.x, scissors.y, scissors.mWidth, scissors.mHeight); // Default options

	}
	void GLDevice::bind_check_blend ( const Pipeline& pip ) noexcept {

		const auto& blend = pip.mBlend;

		ToggleBlend(blend.bEnabled);

		SetBlendFactors(blend.mSrcColorFactor, blend.mDstColorFactor, blend.mSrcAlphaFactor, blend.mDstAlphaFactor);
		SetBlendOp(blend.mColorOp, blend.mAlphaOp);

	}
	void GLDevice::bind_check_cull ( const Pipeline& pip ) noexcept {

		const auto& cull = pip.mCull;

		ToggleCull(cull.bEnabled);

		SetCullFace(cull.mFace);
		SetFrontFace(cull.mWindingOrder);

	}
	void GLDevice::bind_check_color_mask ( const Pipeline& pip ) noexcept {

		const auto& mask = pip.mColorMask;

		SetColorMask(mask);

	}




	PipelineHandle GLDevice::CreatePipeline ( const PipelineDescriptor& desc ) {

		LUM_HOTCHK_RETURN_CUSTOM(
			mPipelines.dense_size() <= skMaxPipelines,
			LUM_SEV_WARN,
			PipelineHandle{},
			"Max pipelines reached"
		);

		if (desc.mShader.id != null_id<PipelineID>() && !mShaders.exist(desc.mShader)) {
			LUM_LOG_ERROR("Shader %d doesn't exist", desc.mShader.id);
		}

		Pipeline pipeline;
		std::memcpy(&pipeline, &desc, sizeof(desc));

		return mPipelines.create_handle(std::move(pipeline));

	}
	void GLDevice::DeletePipeline(PipelineHandle& pipeline) {

		if (!mPipelines.exist(pipeline)) {
			return;
		}

		mPipelines.delete_handle(pipeline);

	}
	void GLDevice::BindPipeline(const PipelineHandle& pipeline) {

		if (!mPipelines.exist(pipeline)) {
			return;
		}

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