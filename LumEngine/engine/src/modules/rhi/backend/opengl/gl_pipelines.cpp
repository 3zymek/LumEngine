//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: RHI OpenGL implementation pipeline operations and management
// 
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	void GLDevice::bind_check_shader ( const RPipeline& pip ) noexcept {
		
		if (pip.mShader.mID == null_id<RShaderID>())
			return;

		BindShader(pip.mShader);

	}
	void GLDevice::bind_check_rasterizer ( const RPipeline& pip ) noexcept {

		const auto& rast = pip.mRasterizer;

		SetTopology(rast.mPolygon.mTopologyMode, rast.mPolygon.mTopologyModeFaces);
		SetLineWidth(rast.mPolygon.mLineWidth);
		SetPointSize(rast.mPolygon.mPointSize);

		ToggleDepthBias(rast.mDepthBias.bEnable);

		SetDepthBiasSlope(rast.mDepthBias.mSlopeFactor);
		SetDepthBiasClamp(rast.mDepthBias.mClamp);

	}
	void GLDevice::bind_check_depth_stencil ( const RPipeline& pip ) noexcept {

		const auto& depth = pip.mDepthStencil.mDepth;
		const auto& stencil = pip.mDepthStencil.mStencil;

		ToggleDepthTest(depth.bEnabled);

		SetDepthFunc(depth.mCompare);

		ToggleStencilTest(stencil.bEnabled);
		SetStencilReference(stencil.mFront.mReference, RFace::Front);
		SetStencilReference(stencil.mBack.mReference, RFace::Back);
		SetStencilOp(stencil.mFront.mStencilFailOp, stencil.mFront.mDepthFailOp, stencil.mFront.mPassOp, RFace::Front);
		SetStencilOp(stencil.mBack.mStencilFailOp, stencil.mBack.mDepthFailOp, stencil.mBack.mPassOp, RFace::Back);
		
	}
	void GLDevice::bind_check_scissors ( const RPipeline& pip ) noexcept {

		const auto& scissors = pip.mScissor;

		ToggleScissors(pip.mScissor.bEnabled);
		SetScissors(scissors.x, scissors.y, scissors.mWidth, scissors.mHeight); // Default options

	}
	void GLDevice::bind_check_blend ( const RPipeline& pip ) noexcept {

		const auto& blend = pip.mBlend;

		ToggleBlend(blend.bEnabled);

		SetBlendFactors(blend.mSrcColorFactor, blend.mDstColorFactor, blend.mSrcAlphaFactor, blend.mDstAlphaFactor);
		SetBlendOp(blend.mColorOp, blend.mAlphaOp);

	}
	void GLDevice::bind_check_cull ( const RPipeline& pip ) noexcept {

		const auto& cull = pip.mCull;

		ToggleCull(cull.bEnabled);

		SetCullFace(cull.mFace);
		SetFrontFace(cull.mWindingOrder);

	}
	void GLDevice::bind_check_color_mask ( const RPipeline& pip ) noexcept {

		const auto& mask = pip.mColorMask;

		SetColorMask(mask);

	}




	RPipelineHandle GLDevice::CreatePipeline ( const RPipelineDescriptor& desc ) {

		LUM_HOTCHK_RETURN_CUSTOM(
			mPipelines.DenseSize() <= skMaxPipelines,
			LUM_SEV_WARN,
			{},
			"Max pipelines reached"
		);

		if (desc.mShader.mID != null_id<RShaderID>() && !mShaders.Contains(desc.mShader)) {
			LUM_LOG_ERROR("Shader %d doesn't exist", desc.mShader.mID);
		}

		RPipeline pipeline;
		std::memcpy(&pipeline, &desc, sizeof(desc));

		return mPipelines.Append(std::move(pipeline));

	}
	void GLDevice::DeletePipeline(RPipelineHandle& pipeline) {

		if (!mPipelines.Contains(pipeline)) {
			return;
		}

		mPipelines.Remove(pipeline);

	}
	void GLDevice::BindPipeline(const RPipelineHandle& pipeline) {

		if (!mPipelines.Contains(pipeline)) {
			return;
		}

		if (pipeline == mCurrentPipeline) { 
			LUM_PROFILER_CACHE_HIT();
			return;
		}
		mCurrentPipeline = pipeline;

		RPipeline& pip = mPipelines[pipeline];
	   
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