// ************************************
// LumEngine Copyright (C) 2026 3zymek
// All rights reserved.
// Implementation of state managements from RHI
// ************************************

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	// Scissors setters

	void GLDevice::ToggleScissors ( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::Scissor)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_SCISSOR_TEST);
			mEnabledStates.Enable(State::Scissor);

		}
		else {

			glDisable(GL_SCISSOR_TEST);
			mEnabledStates.Disable(State::Scissor);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsScissorEnabled ( ) const noexcept {
		return mEnabledStates.Has(State::Scissor);
	}
	void GLDevice::SetScissors ( int32 x, int32 y, int32 width, int32 height ) {

		if (!mEnabledStates.Has(State::Scissor)) {
			return;
		}

		if (mScissorState.x == x && mScissorState.y == y && mScissorState.mWidth == width && mScissorState.mHeight == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		mScissorState.x = x;
		mScissorState.y = y;
		mScissorState.mWidth = width;
		mScissorState.mHeight = height;

		glScissor(x, y, width, height);

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorX ( int32 x ) {

		if (!mEnabledStates.Has(State::Scissor)) {
			return;
		}

		if (mScissorState.x == x) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			x,
			mScissorState.y,
			mScissorState.mWidth,
			mScissorState.mHeight
		);

		mScissorState.x = x;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorY ( int32 y ) {

		if (!mEnabledStates.Has(State::Scissor)) {
			return;
		}

		if (mScissorState.y == y) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			mScissorState.x,
			y,
			mScissorState.mWidth,
			mScissorState.mHeight
		);

		mScissorState.y = y;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorWidth ( int32 width ) {

		if (!mEnabledStates.Has(State::Scissor)) {
			return;
		}

		if (mScissorState.mWidth == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			mScissorState.x,
			mScissorState.y,
			width,
			mScissorState.mHeight
		);

		mScissorState.mWidth = width;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetScissorHeight ( int32 height ) {

		if (!mEnabledStates.Has(State::Scissor)) {
			return;
		}

		if (mScissorState.mHeight == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glScissor(
			mScissorState.x,
			mScissorState.y,
			mScissorState.mWidth,
			height
		);

		mScissorState.mHeight = height;

		LUM_PROFILER_CACHE_MISS();

	}


	// Cull setters

	void GLDevice::ToggleCull ( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::Cull)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {
			glEnable(GL_CULL_FACE);
			mEnabledStates.Enable(State::Cull);
		}
		else {
			glDisable(GL_CULL_FACE);
			mEnabledStates.Disable(State::Cull);
		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsCullEnabled ( ) const noexcept {
		return mEnabledStates.Has(State::Cull);
	}
	void GLDevice::SetCullFace ( Face face ) {

		if (!mEnabledStates.Has(State::Cull)) {
			return;
		}

		if (face == mCullState.mFace) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glCullFace(skFacesLookup[lookup_cast(face)]);

		mCullState.mFace = face;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetFrontFace ( WindingOrder order ) {

		if (!mEnabledStates.Has(State::Cull)) {
			return;
		}

		if (order == mCullState.mWindingOrder) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glFrontFace((order == WindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.mWindingOrder = order;

		LUM_PROFILER_CACHE_MISS();

	}


	// Blend setters

	void GLDevice::ToggleBlend ( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::Blend)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {
			glEnable(GL_BLEND);
			mEnabledStates.Enable(State::Blend);
			LUM_LOG_DEBUG("Blend enabled");
		}
		else {
			glDisable(GL_BLEND);
			mEnabledStates.Disable(State::Blend);
			LUM_LOG_DEBUG("Blend disabled");
		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsBlendEnabled ( ) const noexcept {
		return mEnabledStates.Has(State::Blend);
	}
	void GLDevice::SetBlendConstantColor ( ChannelRGBA rgba ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mBlendColor == rgba) {
			LUM_PROFILER_CACHE_HIT();
		}

		glBlendColor(rgba.r, rgba.g, rgba.b, rgba.a);

		mBlendState.mBlendColor = rgba;

		LUM_PROFILER_CACHE_MISS();
	
	}
	void GLDevice::SetBlendFactors ( BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mSrcColorFactor == srcColor && mBlendState.mDstColorFactor == dstColor &&
			mBlendState.mSrcAlphaFactor == srcAlpha && mBlendState.mDstAlphaFactor == dstAlpha) 
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

		mBlendState.mSrcColorFactor = srcColor;
		mBlendState.mDstColorFactor = dstColor;
		mBlendState.mSrcAlphaFactor = srcAlpha;
		mBlendState.mDstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendColorFactors ( BlendFactor srcColor, BlendFactor dstColor ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mSrcColorFactor == srcColor && mBlendState.mDstColorFactor == dstColor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(srcColor)],
			skBlendFactorLookup[lookup_cast(dstColor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mSrcColorFactor = srcColor;
		mBlendState.mDstColorFactor = dstColor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendAlphaFactors ( BlendFactor srcAlpha, BlendFactor dstAlpha ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mSrcAlphaFactor == srcAlpha && mBlendState.mDstAlphaFactor == dstAlpha) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[lookup_cast(srcAlpha)],
			skBlendFactorLookup[lookup_cast(dstAlpha)]
		);

		mBlendState.mSrcAlphaFactor = srcAlpha;
		mBlendState.mDstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendSrcColorFactor ( BlendFactor factor ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mSrcColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(factor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mSrcColorFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendDstColorFactor ( BlendFactor factor ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mDstColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[lookup_cast(factor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mDstColorFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendSrcAlphaFactor ( BlendFactor factor ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mSrcAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[lookup_cast(factor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mSrcAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendDstAlphaFactor ( BlendFactor factor ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mDstAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[lookup_cast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[lookup_cast(factor)]
		);

		mBlendState.mDstAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendOp ( BlendOp colorOp, BlendOp alphaOp ) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mAlphaOp == alphaOp && mBlendState.mColorOp == colorOp) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(colorOp)],
			skBlendOpLookup[lookup_cast(alphaOp)]
		);

		mBlendState.mAlphaOp = alphaOp;
		mBlendState.mColorOp = colorOp;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendColorOp ( BlendOp op) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mColorOp == op) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(op)],
			skBlendOpLookup[lookup_cast(mBlendState.mAlphaOp)]
		);

		mBlendState.mColorOp = op;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendAlphaOp ( BlendOp op) {

		if (!mEnabledStates.Has(State::Blend)) {
			return;
		}

		if (mBlendState.mAlphaOp == op) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(mBlendState.mColorOp)],
			skBlendOpLookup[lookup_cast(op)]
		);

		mBlendState.mAlphaOp = op;

		LUM_PROFILER_CACHE_MISS();

	}
	// TODO implement:
	void GLDevice::SetBlendFactorsForTarget ( uint8 target ) {

	}
	void GLDevice::ToggleBlendForTarget(uint8 target, bool toggle ) {

	}


	// Depth setters

	void GLDevice::ToggleDepthWrite ( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::DepthWrite)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.Enable(State::DepthWrite);
			LUM_LOG_DEBUG("Depth write enabled");

		}
		else {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.Disable(State::DepthWrite);
			LUM_LOG_DEBUG("Depth write disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsDepthWriteEnabled ( ) const noexcept {
		return mEnabledStates.Has(State::DepthWrite);
	}

	void GLDevice::ToggleDepthTest ( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::DepthTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_DEPTH_TEST);
			mEnabledStates.Enable(State::DepthTest);
			LUM_LOG_DEBUG("Depth test enabled");

		}
		else {

			glDisable(GL_DEPTH_TEST);
			mEnabledStates.Disable(State::DepthTest);
			LUM_LOG_DEBUG("Depth test disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsDepthTestEnabled ( )	const noexcept {
		return mEnabledStates.Has(State::DepthTest);
	}
	void GLDevice::SetDepthFunc ( CompareFlag func ) {

		if (!mEnabledStates.Has(State::DepthTest)) {
			return;
		}

		if (mDepthStencilState.depth.mCompare == func) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glDepthFunc(skCompareFlagLookup[lookup_cast(func)]);

		LUM_PROFILER_CACHE_MISS();

	}
	

	// Stencil setters

	void GLDevice::ToggleStencilTest ( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::StencilTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_STENCIL_TEST);
			mEnabledStates.Enable(State::StencilTest);
			LUM_LOG_DEBUG("Stencil test enabled");

		}
		else {

			glDisable(GL_STENCIL_TEST);
			mEnabledStates.Disable(State::StencilTest);
			LUM_LOG_DEBUG("Stencil test disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsStencilTestEnabled ( ) const noexcept {
		return mEnabledStates.Has(State::StencilTest);
	}
	void GLDevice::SetStencilReference ( int32 ref, Face face = Face::FrontBack ) {

		if (!mEnabledStates.Has(State::StencilTest)) {
			return;
		}

		if (face == Face::Front) {

			auto& front = mDepthStencilState.stencil.mFront;

			if (front.reference == ref) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilFuncSeparate(
				GL_FRONT, 
				skCompareFlagLookup[lookup_cast(front.compareFlag)], 
				ref, 
				front.readMask
			);

			front.reference = ref;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == Face::Back) {
			auto& back = mDepthStencilState.stencil.mBack;

			if (back.reference == ref) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilFuncSeparate(
				GL_BACK,
				skCompareFlagLookup[lookup_cast(back.compareFlag)],
				ref,
				back.readMask
			);

			back.reference = ref;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilReference(ref, Face::Back);
			SetStencilReference(ref, Face::Front);

		}

	}
	void GLDevice::SetStencilOp ( StencilOp sfail, StencilOp dpfail, StencilOp dppass, Face face ) {

		if (!mEnabledStates.Has(State::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.mFront;
			
			if (front.stencilFailOp == sfail && front.depthFailOp == dpfail && front.passOp == dppass) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}


			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[lookup_cast(sfail)],
				skStencilOpLookup[lookup_cast(dpfail)],
				skStencilOpLookup[lookup_cast(dppass)]
			);

			front.stencilFailOp = sfail;
			front.depthFailOp = dpfail;
			front.passOp = dppass;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == Face::Back) {

			auto& back = stencil.mBack;

			if (back.stencilFailOp == sfail && back.depthFailOp == dpfail && back.passOp == dppass) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[lookup_cast(sfail)],
				skStencilOpLookup[lookup_cast(dpfail)],
				skStencilOpLookup[lookup_cast(dppass)]
			);

			back.stencilFailOp = sfail;
			back.depthFailOp = dpfail;
			back.passOp = dppass;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOp(sfail, dpfail, dppass, Face::Back);
			SetStencilOp(sfail, dpfail, dppass, Face::Front);

		}

	}
	void GLDevice::SetStencilOpOnStencilFail ( StencilOp op, Face face ) {

		if (!mEnabledStates.Has(State::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.mFront;

			if (front.stencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[lookup_cast(op)],
				skStencilOpLookup[lookup_cast(front.depthFailOp)],
				skStencilOpLookup[lookup_cast(front.passOp)]
			);

			front.stencilFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == Face::Back) {

			auto& back = stencil.mBack;

			if (back.stencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[lookup_cast(op)],
				skStencilOpLookup[lookup_cast(back.depthFailOp)],
				skStencilOpLookup[lookup_cast(back.passOp)]
			);

			back.stencilFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOpOnStencilFail(op, Face::Back);
			SetStencilOpOnStencilFail(op, Face::Front);

		}

	}
	void GLDevice::SetStencilOpOnDepthFail ( StencilOp op, Face face ) {

		if (!mEnabledStates.Has(State::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.mFront;

			if (front.stencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[lookup_cast(front.stencilFailOp)],
				skStencilOpLookup[lookup_cast(op)],
				skStencilOpLookup[lookup_cast(front.passOp)]
			);

			front.depthFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == Face::Back) {

			auto& back = stencil.mBack;

			if (back.stencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[lookup_cast(back.stencilFailOp)],
				skStencilOpLookup[lookup_cast(op)],
				skStencilOpLookup[lookup_cast(back.passOp)]
			);

			back.depthFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOpOnDepthFail(op, Face::Back);
			SetStencilOpOnDepthFail(op, Face::Front);

		}

	}
	void GLDevice::SetStencilOpOnDepthPass ( StencilOp op, Face face ) {

		if (!mEnabledStates.Has(State::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.mFront;

			if (front.passOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[lookup_cast(front.stencilFailOp)],
				skStencilOpLookup[lookup_cast(front.depthFailOp)],
				skStencilOpLookup[lookup_cast(op)]
			);

			front.passOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == Face::Back) {

			auto& back = stencil.mBack;

			if (back.passOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[lookup_cast(back.stencilFailOp)],
				skStencilOpLookup[lookup_cast(back.depthFailOp)],
				skStencilOpLookup[lookup_cast(op)]
			);

			back.passOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOpOnDepthFail(op, Face::Back);
			SetStencilOpOnDepthFail(op, Face::Front);

		}

	}


	// Rasterizer setters

	void GLDevice::ToggleDepthBias ( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::DepthBias)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.Enable(State::DepthBias);

		}
		else {

			glDisable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.Disable(State::DepthBias);

		}

		LUM_PROFILER_CACHE_MISS();
	
	}
	bool GLDevice::IsDepthBiasEnabled ( ) const noexcept {
		return mEnabledStates.Has(State::DepthBias);
	}
	void GLDevice::SetDepthBiasFactors ( float32 slope, float32 units ) {

		if (!mEnabledStates.Has(State::DepthBias)) {
			return;
		}

		if (mRasterizerState.depthBias.slopeFactor == slope && mRasterizerState.depthBias.units == units) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slope, units);

		mRasterizerState.depthBias.slopeFactor = slope;
		mRasterizerState.depthBias.units = units;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasClamp ( float32 clamp ) {

		if (!mEnabledStates.Has(State::DepthBias)) {
			return;
		}

		if (mRasterizerState.depthBias.clamp == clamp) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffsetClamp(
			mRasterizerState.depthBias.slopeFactor,
			mRasterizerState.depthBias.units,
			clamp
		); 

		mRasterizerState.depthBias.clamp = clamp;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasSlope ( float32 slopeFactor ) {

		if (!mEnabledStates.Has(State::DepthBias)) {
			return;
		}

		if (mRasterizerState.depthBias.slopeFactor == slopeFactor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slopeFactor, mRasterizerState.depthBias.units);
		mRasterizerState.depthBias.slopeFactor = slopeFactor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasConstant ( float32 constantBias ) {

		if (!mEnabledStates.Has(State::DepthBias)) {
			return;
		}

		if (mRasterizerState.depthBias.units == constantBias) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(mRasterizerState.depthBias.slopeFactor, constantBias);
		mRasterizerState.depthBias.units = constantBias;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetTopology ( TopologyMode mode, Face face ) {

		if (mRasterizerState.polygon.topologyMode == mode && mRasterizerState.polygon.topologyModeFaces == face) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonMode(
			skFacesLookup[lookup_cast(face)],
			skTopologyModeLookup[lookup_cast(mode)]
		);

		mRasterizerState.polygon.topologyMode = mode;
		mRasterizerState.polygon.topologyModeFaces = face;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetPointSize ( float32 size ) {

		if (mRasterizerState.polygon.pointSize == size) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPointSize(size);

		mRasterizerState.polygon.pointSize = size;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetLineWidth ( float32 width ) {

		if (mRasterizerState.polygon.lineWidth == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glLineWidth(width);

		mRasterizerState.polygon.lineWidth = width;

		LUM_PROFILER_CACHE_MISS();

	}


	// Multisample

	void GLDevice::ToggleMultisample(bool toggle) {

		if (toggle == mEnabledStates.Has(State::Multisample)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_MULTISAMPLE);
			mEnabledStates.Enable(State::Multisample);

		}
		else {

			glDisable(GL_MULTISAMPLE);
			mEnabledStates.Disable(State::Multisample);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::ToggleSampleCoverage(bool toggle) {


	}
	void GLDevice::ToggleSampleAlphaToCoverage(bool toggle) {


	}
	void GLDevice::ToggleSampleAlphaToOne(bool toggle) {


	}
	void GLDevice::SetSampleCoverage(float32 value, bool invert) {


	}

}