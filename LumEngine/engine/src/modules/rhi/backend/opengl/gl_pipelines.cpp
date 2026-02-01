#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	///////////////////////////////////////////////////
	/// Private helpers
	///////////////////////////////////////////////////

	void GLDevice::_BindCheckRasterizer(const Pipeline& pip) noexcept {
		const auto& rast = pip.rasterizer;

		if (rast.topologyMode != mRasterizerState.topologyMode || rast.topologyModeFaces != mRasterizerState.topologyModeFaces) {
			glPolygonMode(
				skFacesLookup[lookup_cast(rast.topologyModeFaces)],
				skPolygonModeLookup[lookup_cast(rast.topologyMode)]
			);
			mRasterizerState = rast;
		}

		EnableDepthBias(rast.depthBias.bEnable);

		if (rast.depthBias.bEnable) {

			SetDepthBias(rast.depthBias.slopeFactor, rast.depthBias.constantBias);

		}

	}
	void GLDevice::_BindCheckDepthStencil(const Pipeline& pip) noexcept {

		const auto& depth = pip.depthStencil.depth;
		const auto& stencil = pip.depthStencil.stencil;

		EnableDepthTest(depth.bEnabled);

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
	void GLDevice::_BindCheckScissors(const Pipeline& pip) noexcept {

		const auto& scissors = pip.scissor;

		if (scissors.bEnabled != mEnabledStates.has(State::Scissor)) {

			if (scissors.bEnabled) {

				glEnable(GL_SCISSOR_TEST);
				mEnabledStates.enable(State::Scissor);

				SetScissor(scissors.x, scissors.y, scissors.width, scissors.height); // Default options

			}
			else {

				glDisable(GL_SCISSOR_TEST);
				mEnabledStates.disable(State::Scissor);

			}
		}

	}
	void GLDevice::_BindCheckBlend(const Pipeline& pip) noexcept {

		const auto& blend = pip.blend;

		EnableBlend(blend.bEnabled);

		if (blend.bEnabled) {

			SetBlendFactors(blend.srcColorFactor, blend.dstColorFactor, blend.srcAlphaFactor, blend.dstAlphaFactor);
			SetBlendOp(blend.colorOp, blend.alphaOp);

		}

	}
	void GLDevice::_BindCheckCull(const Pipeline& pip) noexcept {

		const auto& cull = pip.cull;

		EnableCull(cull.bEnabled);

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
			mPipelines.dense_size() >= skMaxPipelines,
			"Max pipelines reached",
			PipelineHandle{}
		);

		Pipeline pipeline;
		std::memcpy(&pipeline, &desc, sizeof(desc));

		return mPipelines.create_handle(std::move(pipeline));
	}
	void GLDevice::DeletePipeline(PipelineHandle& pipeline) {
		LUM_HOTCHK_RETURN_VOID(!mPipelines.exist(pipeline), "Pipeline doesn't exist");

		mPipelines.delete_handle(pipeline);
	}
	void GLDevice::BindPipeline(const PipelineHandle& pipeline) {
		LUM_HOTCHK_RETURN_VOID(!mPipelines.exist(pipeline), "Pipeline doesn't exist");

		if (pipeline.id != mCurrentPipeline.id && pipeline.generation != mCurrentPipeline.generation) return;
		mCurrentPipeline = pipeline;

		Pipeline& pip = mPipelines[pipeline];

		// Topology
		_BindCheckRasterizer(pip);

		// Depth & Stencil
		_BindCheckDepthStencil(pip);

		// Scissors
		_BindCheckScissors(pip);

		// Cull
		_BindCheckCull(pip);

		// Blend
		_BindCheckBlend(pip);

	}

}