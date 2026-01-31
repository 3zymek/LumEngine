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

	// TO IMPLEMENT:
	void GLDevice::_CacheUniformsLocations() {




	}
	bool GLDevice::_IsValidBufferDescriptor(const BufferDescriptor& desc) noexcept {

		if (desc.bufferUsage == BufferUsage::Static) {

			if ((desc.mapFlags & ~(Mapflag::Coherent | Mapflag::Read)) != 0) {
				LUM_LOG_ERROR("Invalid buffer descriptor");
				return false;
			}

		}
		else if ((desc.mapFlags & Mapflag::Coherent) && !(desc.mapFlags & Mapflag::Persistent)) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}
	GLbitfield GLDevice::_TranslateMappingFlags(Mapflag flags) noexcept {
		GLbitfield flag = 0;

		if (flags & Mapflag::Persistent)			flag |= GL_MAP_PERSISTENT_BIT;
		if (flags & Mapflag::Write)					flag |= GL_MAP_WRITE_BIT;
		if (flags & Mapflag::Read)					flag |= GL_MAP_READ_BIT;
		if (flags & Mapflag::Coherent)				flag |= GL_MAP_COHERENT_BIT;
		if (flags & Mapflag::Invalidate_Buffer)		flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags & Mapflag::Invalidate_Range)		flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags & Mapflag::Unsynchronized)		flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}

	///////////////////////////////////////////////////
	/// Pipelines
	///////////////////////////////////////////////////

	PipelineHandle GLDevice::CreatePipeline(const PipelineDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mPipelines.dense_size() >= skMaxPipelines,
			"Max pipelines reached",
			PipelineHandle{}
		);

		Pipeline pipeline;
		std::memcpy(&pipeline, &desc, sizeof(desc));

		return mPipelines.create_handle(std::move(pipeline));
	}
	void GLDevice::DeletePipeline(PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.exists(pipeline), "Pipeline doesn't exist");

		mPipelines.delete_handle(pipeline);
	}
	void GLDevice::BindPipeline(const PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.exists(pipeline), "Pipeline doesn't exist");

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