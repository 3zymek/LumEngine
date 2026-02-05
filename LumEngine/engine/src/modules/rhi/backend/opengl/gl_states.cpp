#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {


	void GLDevice::ToggleScissors(bool enable) {

		if (enable == mEnabledStates.has(State::Scissor))
			return;

		if (enable) {

			glEnable(GL_SCISSOR_TEST);
			mEnabledStates.enable(State::Scissor);

		}
		else {

			glDisable(GL_SCISSOR_TEST);
			mEnabledStates.disable(State::Scissor);

		}


	}
	void GLDevice::SetScissors(int32 x, int32 y, int32 width, int32 height) {

		LUM_HOTCHK_RETURN_VOID(!mEnabledStates.has(State::Scissor), "Scissor state not enabled");

		if (mScissorState.x == x && mScissorState.y == y && mScissorState.width == width && mScissorState.height == height)
			return;

		mScissorState.x = x;
		mScissorState.y = y;
		mScissorState.width = width;
		mScissorState.height = height;

		glScissor(x, y, width, height);

	}
	void GLDevice::SetScissorX(int32 x) {

	}
	void GLDevice::SetScissorY(int32 y) {

	}
	void GLDevice::SetScissorWidth(int32 width) {

	}
	void GLDevice::SetScissorHeight(int32 height) {

	}



	// Cull setters

	void GLDevice::ToggleCull(bool enable) {

		if (enable == mEnabledStates.has(State::Cull))
			return;

		if (enable) {
			glEnable(GL_CULL_FACE);
			mEnabledStates.enable(State::Cull);
		}
		else {
			glDisable(GL_CULL_FACE);
			mEnabledStates.disable(State::Cull);
		}


	}
	void GLDevice::SetCullFace(Face face) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Cull), "Cull not enabled");

		if (face == mCullState.face)
			return;

		glCullFace(skFacesLookup[lookup_cast(face)]);

		mCullState.face = face;

	}
	void GLDevice::SetCullWindingOrder(WindingOrder order) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Cull), "Cull not enabled");

		if (order == mCullState.windingOrder)
			return;

		glFrontFace((order == WindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.windingOrder = order;

	}




	// Blend setters

	void GLDevice::ToggleBlend(bool enable) {

		if (enable == mEnabledStates.has(State::Blend))
			return;

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

	}
	//virtual void SetBlendConstantColor(glm::vec4 rgba) = 0; IMPLEMENT
	void GLDevice::SetBlendFactors(BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcColorFactor == srcColor && mBlendState.dstColorFactor == dstColor &&
			mBlendState.srcAlphaFactor == srcAlpha && mBlendState.dstAlphaFactor == dstAlpha)
			return;

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

	}
	void GLDevice::SetBlendColorFactors(BlendFactor srcColor, BlendFactor dstColor) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcColorFactor == srcColor && mBlendState.dstColorFactor == dstColor)
			return;

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(srcColor)],
			skBlendFactorLookup[lookup_cast(dstColor)],
			skBlendFactorLookup[lookup_cast(mBlendState.srcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstAlphaFactor)]
		);

		mBlendState.srcColorFactor = srcColor;
		mBlendState.dstColorFactor = dstColor;

	}
	void GLDevice::SetBlendAlphaFactors(BlendFactor srcAlpha, BlendFactor dstAlpha) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcAlphaFactor == srcAlpha && mBlendState.dstAlphaFactor == dstAlpha)
			return;

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.srcColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.dstColorFactor)],
			skBlendFactorLookup[lookup_cast(srcAlpha)],
			skBlendFactorLookup[lookup_cast(dstAlpha)]
		);

		mBlendState.srcAlphaFactor = srcAlpha;
		mBlendState.dstAlphaFactor = dstAlpha;

	}
	void GLDevice::SetBlendSrcColorFactor(BlendFactor factor) {

	}
	void GLDevice::SetBlendDstColorFactor(BlendFactor factor) {

	}
	void GLDevice::SetBlendSrcAlphaFactor(BlendFactor factor) {

	}
	void GLDevice::SetBlendDstAlphaFactor(BlendFactor factor) {

	}
	void GLDevice::SetBlendOp(BlendOp colorOp, BlendOp alphaOp) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.alphaOp == alphaOp && mBlendState.colorOp == colorOp)
			return;

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(colorOp)],
			skBlendOpLookup[lookup_cast(alphaOp)]
		);

	}
	void GLDevice::SetBlendColorOp(BlendOp op) {

	}
	void GLDevice::SetBlendAlphaOp(BlendOp op) {

	}



	// Depth setters

	void GLDevice::ToggleDepthWrite(bool enable) {

		if (enable == mEnabledStates.has(State::DepthWrite))
			return;

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
	void GLDevice::SetDepthFunc(CompareFlag func) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::DepthTest), "Depth not enabled");

		if (mDepthStencilState.depth.compareFlag == func)
			return;

		glDepthFunc(skCompareFlagLookup[lookup_cast(func)]);

	}
	



	// Stencil setters

	void GLDevice::ToggleStencilTest(bool enable) {

		if (enable == mEnabledStates.has(State::StencilTest))
			return;

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

		if (enable == mEnabledStates.has(State::DepthBias))
			return;

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

	}
	void GLDevice::SetDepthBiasFactors(float32 slope, float32 constant) {

	}
	void GLDevice::SetDepthBiasClamp(float32 clamp) {

	}
	void GLDevice::SetDepthBiasSlope(float32 slopeFactor) {

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), "Depth bias not enabled");

		if (mRasterizerState.depthBias.slopeFactor == slopeFactor)
			return;

		glPolygonOffset(slopeFactor, mRasterizerState.depthBias.constantBias);
		mRasterizerState.depthBias.slopeFactor = slopeFactor;

	}
	void GLDevice::SetDepthBiasConstant(float32 constantBias) {

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), "Depth bias not enabled");

		if (mRasterizerState.depthBias.constantBias == constantBias)
			return;

		glPolygonOffset(mRasterizerState.depthBias.slopeFactor, constantBias);
		mRasterizerState.depthBias.constantBias = constantBias;

	}
	void GLDevice::SetTopology(TopologyMode mode, Face face) {

		if (mRasterizerState.topologyMode == mode && mRasterizerState.topologyModeFaces == face)
			return;

		glPolygonMode(
			skFacesLookup[lookup_cast(face)],
			skTopologyModeLookup[lookup_cast(mode)]
		);

		mRasterizerState.topologyMode = mode;
		mRasterizerState.topologyModeFaces = face;

	}
}