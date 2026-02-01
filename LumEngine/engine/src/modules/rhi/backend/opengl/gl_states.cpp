#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {
	// FINISHED
	void GLDevice::EnableScissors(bool enable) {

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

	// FINISHED
	void GLDevice::SetScissor(int32 x, int32 y, int32 width, int32 height) {
		LUM_HOTCHK_RETURN_VOID(!mEnabledStates.has(State::Scissor), "Scissor state not enabled");

		if (mScissorState.x == x && mScissorState.y == y && mScissorState.width == width && mScissorState.height == height)
			return;

		mScissorState.x = x;
		mScissorState.y = y;
		mScissorState.width = width;
		mScissorState.height = height;

		glScissor(x, y, width, height);

	}

	// FINISHED
	void GLDevice::SetViewport(int32 x, int32 y, int32 width, int32 height) {

		glViewport(x, y, width, height);

	}


	// Cull setters (FINISHED)

	// FINISHED
	void GLDevice::EnableCull(bool enable) {

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

	// FINISHED
	void GLDevice::SetCullFace(Face face) {
		LUM_HOTCHK_RETURN_VOID(!mEnabledStates.has(State::Cull), "Cull not enabled");

		if (face == mCullState.face)
			return;

		glCullFace(skFacesLookup[lookup_cast(face)]);
		mEnabledStates.enable(State::Cull);

		mCullState.face = face;

	}

	// FINISHED
	void GLDevice::SetCullWindingOrder(WindingOrder order) {
		LUM_HOTCHK_RETURN_VOID(!mEnabledStates.has(State::Cull), "Cull not enabled");

		if (order == mCullState.windingOrder)
			return;

		glFrontFace((order == WindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.windingOrder = order;

	}


	// Blend setters

	// FINISHED
	void GLDevice::EnableBlend(bool enable) {

		if (enable == mEnabledStates.has(State::Blend))
			return;

		if (enable) {
			glEnable(GL_BLEND);
			mEnabledStates.enable(State::Blend);
		}
		else {
			glDisable(GL_BLEND);
			mEnabledStates.disable(State::Blend);
		}

	}

	//virtual void SetBlendConstantColor(glm::vec4 rgba) = 0; IMPLEMENT

	// FINISHED
	void GLDevice::SetBlendFactors(BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha) {
		LUM_HOTCHK_RETURN_VOID(!mEnabledStates.has(State::Blend), "Blend not enabled");

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

	// FINISHED
	void GLDevice::SetBlendOp(BlendOp colorOp, BlendOp alphaOp) {
		LUM_HOTCHK_RETURN_VOID(!mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.alphaOp == alphaOp && mBlendState.colorOp == colorOp)
			return;

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(colorOp)],
			skBlendOpLookup[lookup_cast(alphaOp)]
		);

	}


	// Depth setters

	// FINISHED
	void GLDevice::EnableDepthWrite(bool enable) {

		if (enable == mEnabledStates.has(State::DepthWrite))
			return;

		if (enable) {

			glDepthMask((GLboolean)enable);
			mEnabledStates.enable(State::DepthWrite);

		}
		else {

			glDepthMask((GLboolean)enable);
			mEnabledStates.disable(State::DepthWrite);

		}

	}

	// FINISHED
	void GLDevice::EnableDepthTest(bool enable) {

		if (enable == mEnabledStates.has(State::DepthTest))
			return;

		if (enable) {

			glEnable(GL_DEPTH_TEST);
			mEnabledStates.enable(State::DepthTest);

		}
		else {

			glDisable(GL_DEPTH_TEST);
			mEnabledStates.disable(State::DepthTest);

		}

	}

	// FINISHED
	void GLDevice::SetDepthFunc(CompareFlag func) {
		LUM_HOTCHK_RETURN_VOID(!mEnabledStates.has(State::DepthTest), "Depth not enabled");

		if (func == mDepthStencilState.depth.compareFlag)
			return;

		glDepthFunc(skCompareFlagLookup[lookup_cast(func)]);

	}

	// FINISHED
	void GLDevice::EnableStencilTest(bool enable) {

		if (enable == mEnabledStates.has(State::StencilTest))
			return;

		if (enable) {

			glEnable(GL_STENCIL_TEST);
			mEnabledStates.enable(State::StencilTest);

		}
		else {

			glDisable(GL_STENCIL_TEST);
			mEnabledStates.disable(State::StencilTest);

		}


	}


	void GLDevice::SetStencilReference(int32 ref, Face face = Face::FrontBack) {


	}


	// Rasterizer setters

	// FINISHED
	void GLDevice::EnableDepthBias(bool enable) {

		if (enable == mEnabledStates.has(State::DepthBias))
			return;

		if (enable) {

			glEnable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.enable(State::DepthBias);

		}
		else {

			glDisable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.disable(State::DepthBias);

		}

	}

	// FINISHED
	void GLDevice::SetDepthBias(float32 slopFactor, float32 constantBias) {
		LUM_HOTCHK(!mEnabledStates.has(State::DepthBias), "Depth bias not enabled");

		if (slopFactor == mRasterizerState.depthBias.slopeFactor && constantBias == mRasterizerState.depthBias.constantBias)
			return;

		if (slopFactor == max_val<float32>())
			slopFactor = mRasterizerState.depthBias.slopeFactor;

		if (constantBias == max_val<float32>())
			constantBias = mRasterizerState.depthBias.constantBias;

		glPolygonOffset(slopFactor, constantBias);
		mRasterizerState.depthBias.slopeFactor = slopFactor;
		mRasterizerState.depthBias.constantBias = constantBias;

	}
}