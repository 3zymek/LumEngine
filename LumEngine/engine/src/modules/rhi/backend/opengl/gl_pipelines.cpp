#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	///////////////////////////////////////////////////
	/// Private helpers
	///////////////////////////////////////////////////

	void GLDevice::bind_check_shader(const Pipeline& pip) noexcept {
		
		if (pip.shader.id == null_id<ShaderID>())
			return;

		BindShader(pip.shader);

	}

	void GLDevice::bind_check_rasterizer(const Pipeline& pip) noexcept {
		const auto& rast = pip.mRasterizer;

		if (rast.topologyMode != mRasterizerState.topologyMode || rast.topologyModeFaces != mRasterizerState.topologyModeFaces) {
			glPolygonMode(
				skFacesLookup[lookup_cast(rast.topologyModeFaces)],
				skTopologyModeLookup[lookup_cast(rast.topologyMode)]
			);
			mRasterizerState = rast;
		}

		ToggleDepthBias(rast.depthBias.bEnable);

		if (rast.depthBias.bEnable) {

			SetDepthBiasSlope(rast.depthBias.slopeFactor);

		}

	}
	void GLDevice::bind_check_depth_stencil(const Pipeline& pip) noexcept {

		const auto& depth = pip.mDepthStencil.depth;
		const auto& stencil = pip.mDepthStencil.stencil;

		ToggleDepthTest(depth.bEnabled);

		if (depth.bEnabled) {

			SetDepthFunc(depth.compareFlag);

		}


		if (stencil.bEnabled != mDepthStencilState.stencil.bEnabled) {

			if (stencil.bEnabled) {

				glEnable(GL_STENCIL_TEST);
				mEnabledStates.enable(State::StencilTest);

			}
			else {

				glDisable(GL_STENCIL_TEST);
				mEnabledStates.disable(State::StencilTest);

			}


		}

	}
	void GLDevice::bind_check_scissors(const Pipeline& pip) noexcept {

		const auto& scissors = pip.mScissor;

		if (scissors.bEnabled != mEnabledStates.has(State::Scissor)) {

			if (scissors.bEnabled) {

				glEnable(GL_SCISSOR_TEST);
				mEnabledStates.enable(State::Scissor);

				SetScissors(scissors.x, scissors.y, scissors.width, scissors.height); // Default options

			}
			else {

				glDisable(GL_SCISSOR_TEST);
				mEnabledStates.disable(State::Scissor);

			}
		}

	}
	void GLDevice::bind_check_blend(const Pipeline& pip) noexcept {

		const auto& blend = pip.mBlend;

		ToggleBlend(blend.bEnabled);

		if (blend.bEnabled) {

			SetBlendFactors(blend.srcColorFactor, blend.dstColorFactor, blend.srcAlphaFactor, blend.dstAlphaFactor);
			SetBlendOp(blend.colorOp, blend.alphaOp);

		}

	}
	void GLDevice::bind_check_cull(const Pipeline& pip) noexcept {

		const auto& cull = pip.mCull;

		ToggleCull(cull.bEnabled);

		if (cull.bEnabled) {

			SetCullFace(cull.face);
			SetCullWindingOrder(cull.windingOrder);

		}

	}


	///////////////////////////////////////////////////
	/// Pipelines
	///////////////////////////////////////////////////

	PipelineHandle GLDevice::CreatePipeline(const PipelineDescriptor& desc) {
		LUM_HOTCHK_RETURN_CUSTOM(
			mPipelines.dense_size() <= skMaxPipelines,
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
		LUM_HOTCHK_RETURN_VOID(mPipelines.exist(pipeline), "Pipeline doesn't exist");

		mPipelines.delete_handle(pipeline);
	}
	void GLDevice::BindPipeline(const PipelineHandle& pipeline) {
		LUM_HOTCHK_RETURN_VOID(mPipelines.exist(pipeline), "Pipeline doesn't exist");

		if (pipeline.id != mCurrentPipeline.id && pipeline.generation != mCurrentPipeline.generation) return;
		mCurrentPipeline = pipeline;

		Pipeline& pip = mPipelines[pipeline];

		bind_check_shader(pip);

		// Topology
		bind_check_rasterizer(pip);

		// Depth & Stencil
		bind_check_depth_stencil(pip);

		// Scissors
		bind_check_scissors(pip);

		// Cull
		bind_check_cull(pip);

		// Blend
		bind_check_blend(pip);

	}

}