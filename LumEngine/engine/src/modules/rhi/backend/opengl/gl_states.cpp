#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {


	void GLDevice::ToggleScissors(bool enable) {

		if (enable == mEnabledStates.has(State::Scissor)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (enable) {

			glEnable(GL_SCISSOR_TEST);
			mEnabledStates.enable(State::Scissor);

		}
		else {

			glDisable(GL_SCISSOR_TEST);
			mEnabledStates.disable(State::Scissor);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsScissorEnabled() const noexcept {
		return mEnabledStates.has(State::Scissor);
	}
	void GLDevice::SetScissors(int32 x, int32 y, int32 width, int32 height) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), "Scissor state not enabled");

		if (mScissorState.x == x && mScissorState.y == y && mScissorState.width == width && mScissorState.height == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mScissorState.x = x;
		mScissorState.y = y;
		mScissorState.width = width;
		mScissorState.height = height;

		glScissor(x, y, width, height);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorX(int32 x) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), "Scissor not enabled");

		if (mScissorState.x == x) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			x,
			mScissorState.y,
			mScissorState.width,
			mScissorState.height
		);

		mScissorState.x = x;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorY(int32 y) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), "Scissor not enabled");

		if (mScissorState.y == y) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			mScissorState.x,
			y,
			mScissorState.width,
			mScissorState.height
		);

		mScissorState.y = y;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorWidth(int32 width) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), "Scissor not enabled");

		if (mScissorState.width == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			mScissorState.x,
			mScissorState.y,
			width,
			mScissorState.height
		);

		mScissorState.width = width;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorHeight(int32 height) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), "Scissor not enabled");

		if (mScissorState.height == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			mScissorState.x,
			mScissorState.y,
			mScissorState.width,
			height
		);

		mScissorState.height = height;

		LUM_PROFILER_CACHE_MISS();

	}



	// Cull setters

	void GLDevice::ToggleCull(bool enable) {

		if (enable == mEnabledStates.has(State::Cull)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (enable) {
			glEnable(GL_CULL_FACE);
			mEnabledStates.enable(State::Cull);
		}
		else {
			glDisable(GL_CULL_FACE);
			mEnabledStates.disable(State::Cull);
		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsCullEnabled() const noexcept {
		return mEnabledStates.has(State::Cull);
	}
	void GLDevice::SetCullFace(Face face) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Cull), "Cull not enabled");

		if (face == mCullState.face) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glCullFace(skFacesLookup[lookup_cast(face)]);

		mCullState.face = face;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetFrontFace(WindingOrder order) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Cull), "Cull not enabled");

		if (order == mCullState.windingOrder) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glFrontFace((order == WindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.windingOrder = order;

		LUM_PROFILER_CACHE_MISS();

	}




	// Blend setters

	void GLDevice::ToggleBlend(bool enable) {

		if (enable == mEnabledStates.has(State::Blend)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (enable) {
			glEnable(GL_BLEND);
			mEnabledStates.enable(State::Blend);
			LUM_LOG_DEBUG("Blend enabled");
		}
		else {
			glDisable(GL_BLEND);
			mEnabledStates.disable(State::Blend);
			LUM_LOG_DEBUG("Blend disabled");
		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsBlendEnabled() const noexcept {
		return mEnabledStates.has(State::Blend);
	}
	void GLDevice::SetBlendConstantColor(ChannelRGBA rgba) {

	}
	void GLDevice::SetBlendFactors(BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcColorFactor == srcColor && mBlendState.dstColorFactor == dstColor &&
			mBlendState.srcAlphaFactor == srcAlpha && mBlendState.dstAlphaFactor == dstAlpha) 
		{
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(srcColor)],
			skBlendFactorLookup[lookup_cast(dstColor)],
			skBlendFactorLookup[lookup_cast(srcAlpha)],
			skBlendFactorLookup[lookup_cast(dstAlpha)]
		);

		mBlendState.srcColorFactor = srcColor;
		mBlendState.dstColorFactor = dstColor;
		mBlendState.srcAlphaFactor = srcAlpha;
		mBlendState.dstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendColorFactors(BlendFactor srcColor, BlendFactor dstColor) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcColorFactor == srcColor && mBlendState.dstColorFactor == dstColor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(srcColor)],
			skBlendFactorLookup[lookup_cast(dstColor)],
			skBlendFactorLookup[lookup_cast(mBlendState.srcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstAlphaFactor)]
		);

		mBlendState.srcColorFactor = srcColor;
		mBlendState.dstColorFactor = dstColor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendAlphaFactors(BlendFactor srcAlpha, BlendFactor dstAlpha) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcAlphaFactor == srcAlpha && mBlendState.dstAlphaFactor == dstAlpha) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.srcColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstColorFactor)],
			skBlendFactorLookup[lookup_cast(srcAlpha)],
			skBlendFactorLookup[lookup_cast(dstAlpha)]
		);

		mBlendState.srcAlphaFactor = srcAlpha;
		mBlendState.dstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendSrcColorFactor(BlendFactor factor) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(factor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.srcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstAlphaFactor)]
		);

		mBlendState.srcColorFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendDstColorFactor(BlendFactor factor) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.dstColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.srcColorFactor)],
			skBlendFactorLookup[lookup_cast(factor)],
			skBlendFactorLookup[lookup_cast(mBlendState.srcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstAlphaFactor)]
		);

		mBlendState.dstColorFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendSrcAlphaFactor(BlendFactor factor) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.srcColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstColorFactor)],
			skBlendFactorLookup[lookup_cast(factor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstAlphaFactor)]
		);

		mBlendState.srcAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendDstAlphaFactor(BlendFactor factor) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.dstAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.srcColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.srcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(factor)]
		);

		mBlendState.dstAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendOp(BlendOp colorOp, BlendOp alphaOp) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.alphaOp == alphaOp && mBlendState.colorOp == colorOp) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(colorOp)],
			skBlendOpLookup[lookup_cast(alphaOp)]
		);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendColorOp(BlendOp op) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.colorOp == op) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(op)],
			skBlendOpLookup[lookup_cast(mBlendState.alphaOp)]
		);

		mBlendState.colorOp = op;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendAlphaOp(BlendOp op) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.alphaOp == op) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(mBlendState.colorOp)],
			skBlendOpLookup[lookup_cast(op)]
		);

		mBlendState.alphaOp = op;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendFactorsForTarget(uint8 target) {

	}
	void GLDevice::ToggleBlendForTarget(uint8 target, bool enable) {

	}


	// Depth setters

	void GLDevice::ToggleDepthWrite(bool enable) {

		if (enable == mEnabledStates.has(State::DepthWrite)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (enable) {

			glDepthMask((GLboolean)enable);
			mEnabledStates.enable(State::DepthWrite);
			LUM_LOG_DEBUG("Depth write enabled");

		}
		else {

			glDepthMask((GLboolean)enable);
			mEnabledStates.disable(State::DepthWrite);
			LUM_LOG_DEBUG("Depth write disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsDepthWriteEnabled() const noexcept {
		return mEnabledStates.has(State::DepthWrite);
	}

	void GLDevice::ToggleDepthTest(bool enable) {

		if (enable == mEnabledStates.has(State::DepthTest))
			return;

		if (enable) {

			glEnable(GL_DEPTH_TEST);
			mEnabledStates.enable(State::DepthTest);
			LUM_LOG_DEBUG("Depth test enabled");

		}
		else {

			glDisable(GL_DEPTH_TEST);
			mEnabledStates.disable(State::DepthTest);
			LUM_LOG_DEBUG("Depth test disabled");

		}

	}
	bool GLDevice::IsDepthTestEnabled()	const noexcept {
		return mEnabledStates.has(State::DepthTest);
	}
	void GLDevice::SetDepthFunc(CompareFlag func) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::DepthTest), "Depth not enabled");

		if (mDepthStencilState.depth.compareFlag == func)
			return;

		glDepthFunc(skCompareFlagLookup[lookup_cast(func)]);

	}
	



	// Stencil setters

	void GLDevice::ToggleStencilTest(bool enable) {

		if (enable == mEnabledStates.has(State::StencilTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (enable) {

			glEnable(GL_STENCIL_TEST);
			mEnabledStates.enable(State::StencilTest);
			LUM_LOG_DEBUG("Stencil test enabled");

		}
		else {

			glDisable(GL_STENCIL_TEST);
			mEnabledStates.disable(State::StencilTest);
			LUM_LOG_DEBUG("Stencil test disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsStencilTestEnabled() const noexcept {
		return mEnabledStates.has(State::StencilTest);
	}
	void GLDevice::SetStencilReference(int32 ref, Face face = Face::FrontBack) {

	}
	void GLDevice::SetStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass, Face face) {

	}
	void GLDevice::SetStencilOpOnStencilFail(StencilOp op, Face face) {

	}
	void GLDevice::SetStencilOpOnDepthFail(StencilOp op, Face face) {

	}
	void GLDevice::SetStencilOpOnDepthPass(StencilOp op, Face face) {

	}


	// Rasterizer setters

	void GLDevice::ToggleDepthBias(bool enable) {

		if (enable == mEnabledStates.has(State::DepthBias)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (enable) {

			glEnable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.enable(State::DepthBias);
			LUM_LOG_DEBUG("Depth bias enabled");

		}
		else {

			glDisable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.disable(State::DepthBias);
			LUM_LOG_DEBUG("Depth bias disabled");

		}

		LUM_PROFILER_CACHE_MISS();
	
	}
	bool GLDevice::IsDepthBiasEnabled() const noexcept {
		return mEnabledStates.has(State::DepthBias);
	}
	void GLDevice::SetDepthBiasFactors(float32 slope, float32 constant) {

	}
	void GLDevice::SetDepthBiasClamp(float32 clamp) {

	}
	void GLDevice::SetDepthBiasSlope(float32 slopeFactor) {

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), "Depth bias not enabled");

		if (mRasterizerState.depthBias.slopeFactor == slopeFactor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slopeFactor, mRasterizerState.depthBias.constantBias);
		mRasterizerState.depthBias.slopeFactor = slopeFactor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasConstant(float32 constantBias) {

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), "Depth bias not enabled");

		if (mRasterizerState.depthBias.constantBias == constantBias) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(mRasterizerState.depthBias.slopeFactor, constantBias);
		mRasterizerState.depthBias.constantBias = constantBias;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetTopology(TopologyMode mode, Face face) {

		if (mRasterizerState.topologyMode == mode && mRasterizerState.topologyModeFaces == face) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonMode(
			skFacesLookup[lookup_cast(face)],
			skTopologyModeLookup[lookup_cast(mode)]
		);

		mRasterizerState.topologyMode = mode;
		mRasterizerState.topologyModeFaces = face;

		LUM_PROFILER_CACHE_MISS();

	}
}