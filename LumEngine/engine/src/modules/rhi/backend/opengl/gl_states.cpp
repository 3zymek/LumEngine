//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: RHI OpenGL implementation of state setters operations and management
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	// Scissors setters

	void GLDevice::ToggleScissors ( bool toggle ) {

		if (toggle == mEnabledStates.Has(RState::Scissor)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_SCISSOR_TEST);
			mEnabledStates.Enable(RState::Scissor);

		}
		else {

			glDisable(GL_SCISSOR_TEST);
			mEnabledStates.Disable(RState::Scissor);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsScissorEnabled ( ) const noexcept {
		return mEnabledStates.Has(RState::Scissor);
	}
	void GLDevice::SetScissors ( int32 x, int32 y, int32 width, int32 height ) {

		if (!mEnabledStates.Has(RState::Scissor)) {
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

		if (!mEnabledStates.Has(RState::Scissor)) {
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

		if (!mEnabledStates.Has(RState::Scissor)) {
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

		if (!mEnabledStates.Has(RState::Scissor)) {
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

		if (!mEnabledStates.Has(RState::Scissor)) {
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

		if (toggle == mEnabledStates.Has(RState::Cull)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {
			glEnable(GL_CULL_FACE);
			mEnabledStates.Enable(RState::Cull);
		}
		else {
			glDisable(GL_CULL_FACE);
			mEnabledStates.Disable(RState::Cull);
		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsCullEnabled ( ) const noexcept {
		return mEnabledStates.Has(RState::Cull);
	}
	void GLDevice::SetCullFace ( EFace face ) {

		if (!mEnabledStates.Has(RState::Cull)) {
			return;
		}

		if (face == mCullState.mFace) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glCullFace(skFacesLookup[LookupCast(face)]);

		mCullState.mFace = face;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetFrontFace ( EWindingOrder order ) {

		if (!mEnabledStates.Has(RState::Cull)) {
			return;
		}

		if (order == mCullState.mWindingOrder) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glFrontFace((order == EWindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.mWindingOrder = order;

		LUM_PROFILER_CACHE_MISS();

	}


	// Blend setters

	void GLDevice::ToggleBlend ( bool toggle ) {

		if (toggle == mEnabledStates.Has(RState::Blend)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {
			glEnable(GL_BLEND);
			mEnabledStates.Enable(RState::Blend);
			LUM_LOG_DEBUG("Blend enabled");
		}
		else {
			glDisable(GL_BLEND);
			mEnabledStates.Disable(RState::Blend);
			LUM_LOG_DEBUG("Blend disabled");
		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsBlendEnabled ( ) const noexcept {
		return mEnabledStates.Has(RState::Blend);
	}
	void GLDevice::SetBlendConstantColor ( ChannelRGBA rgba ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mBlendColor == rgba) {
			LUM_PROFILER_CACHE_HIT();
		}

		glBlendColor(rgba.r, rgba.g, rgba.b, rgba.a);

		mBlendState.mBlendColor = rgba;

		LUM_PROFILER_CACHE_MISS();
	
	}
	void GLDevice::SetBlendFactors ( EBlendFactor srcColor, EBlendFactor dstColor, EBlendFactor srcAlpha, EBlendFactor dstAlpha ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mSrcColorFactor == srcColor && mBlendState.mDstColorFactor == dstColor &&
			mBlendState.mSrcAlphaFactor == srcAlpha && mBlendState.mDstAlphaFactor == dstAlpha) 
		{
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[LookupCast(srcColor)],
			skBlendFactorLookup[LookupCast(dstColor)],
			skBlendFactorLookup[LookupCast(srcAlpha)],
			skBlendFactorLookup[LookupCast(dstAlpha)]
		);

		mBlendState.mSrcColorFactor = srcColor;
		mBlendState.mDstColorFactor = dstColor;
		mBlendState.mSrcAlphaFactor = srcAlpha;
		mBlendState.mDstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendColorFactors ( EBlendFactor srcColor, EBlendFactor dstColor ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mSrcColorFactor == srcColor && mBlendState.mDstColorFactor == dstColor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[LookupCast(srcColor)],
			skBlendFactorLookup[LookupCast(dstColor)],
			skBlendFactorLookup[LookupCast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mSrcColorFactor = srcColor;
		mBlendState.mDstColorFactor = dstColor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendAlphaFactors ( EBlendFactor srcAlpha, EBlendFactor dstAlpha ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mSrcAlphaFactor == srcAlpha && mBlendState.mDstAlphaFactor == dstAlpha) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[LookupCast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[LookupCast(srcAlpha)],
			skBlendFactorLookup[LookupCast(dstAlpha)]
		);

		mBlendState.mSrcAlphaFactor = srcAlpha;
		mBlendState.mDstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendSrcColorFactor ( EBlendFactor factor ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mSrcColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[LookupCast(factor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mSrcColorFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendDstColorFactor ( EBlendFactor factor ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mDstColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[LookupCast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[LookupCast(factor)],
			skBlendFactorLookup[LookupCast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mDstColorFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendSrcAlphaFactor ( EBlendFactor factor ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mSrcAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[LookupCast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[LookupCast(factor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstAlphaFactor)]
		);

		mBlendState.mSrcAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendDstAlphaFactor ( EBlendFactor factor ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mDstAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendFuncSeparate(
			skBlendFactorLookup[LookupCast(mBlendState.mSrcColorFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mDstColorFactor)],
			skBlendFactorLookup[LookupCast(mBlendState.mSrcAlphaFactor)],
			skBlendFactorLookup[LookupCast(factor)]
		);

		mBlendState.mDstAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendOp ( EBlendOp colorOp, EBlendOp alphaOp ) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mAlphaOp == alphaOp && mBlendState.mColorOp == colorOp) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[LookupCast(colorOp)],
			skBlendOpLookup[LookupCast(alphaOp)]
		);

		mBlendState.mAlphaOp = alphaOp;
		mBlendState.mColorOp = colorOp;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendColorOp ( EBlendOp op) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mColorOp == op) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[LookupCast(op)],
			skBlendOpLookup[LookupCast(mBlendState.mAlphaOp)]
		);

		mBlendState.mColorOp = op;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendAlphaOp ( EBlendOp op) {

		if (!mEnabledStates.Has(RState::Blend)) {
			return;
		}

		if (mBlendState.mAlphaOp == op) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[LookupCast(mBlendState.mColorOp)],
			skBlendOpLookup[LookupCast(op)]
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

		if (toggle == mEnabledStates.Has(RState::DepthWrite)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.Enable(RState::DepthWrite);
			LUM_LOG_DEBUG("Depth write enabled");

		}
		else {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.Disable(RState::DepthWrite);
			LUM_LOG_DEBUG("Depth write disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsDepthWriteEnabled ( ) const noexcept {
		return mEnabledStates.Has(RState::DepthWrite);
	}

	void GLDevice::ToggleDepthTest ( bool toggle ) {

		if (toggle == mEnabledStates.Has(RState::DepthTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_DEPTH_TEST);
			mEnabledStates.Enable(RState::DepthTest);
			LUM_LOG_DEBUG("Depth test enabled");

		}
		else {

			glDisable(GL_DEPTH_TEST);
			mEnabledStates.Disable(RState::DepthTest);
			LUM_LOG_DEBUG("Depth test disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsDepthTestEnabled ( )	const noexcept {
		return mEnabledStates.Has(RState::DepthTest);
	}
	void GLDevice::SetDepthFunc ( RCompareFlag func ) {

		if (!mEnabledStates.Has(RState::DepthTest)) {
			return;
		}

		if (mDepthStencilState.mDepth.mCompare == func) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glDepthFunc(skCompareFlagLookup[LookupCast(func)]);

		LUM_PROFILER_CACHE_MISS();

	}
	

	// Stencil setters

	void GLDevice::ToggleStencilTest ( bool toggle ) {

		if (toggle == mEnabledStates.Has(RState::StencilTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_STENCIL_TEST);
			mEnabledStates.Enable(RState::StencilTest);
			LUM_LOG_DEBUG("Stencil test enabled");

		}
		else {

			glDisable(GL_STENCIL_TEST);
			mEnabledStates.Disable(RState::StencilTest);
			LUM_LOG_DEBUG("Stencil test disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsStencilTestEnabled ( ) const noexcept {
		return mEnabledStates.Has(RState::StencilTest);
	}
	void GLDevice::SetStencilReference ( int32 ref, EFace face ) {

		if (!mEnabledStates.Has(RState::StencilTest)) {
			return;
		}

		if (face == EFace::Front) {

			auto& front = mDepthStencilState.mStencil.mFront;

			if (front.mReference == ref) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilFuncSeparate(
				GL_FRONT, 
				skCompareFlagLookup[LookupCast(front.mCompareFlag)], 
				ref, 
				front.mReadMask
			);

			front.mReference = ref;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == EFace::Back) {
			auto& back = mDepthStencilState.mStencil.mBack;

			if (back.mReference == ref) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilFuncSeparate(
				GL_BACK,
				skCompareFlagLookup[LookupCast(back.mCompareFlag)],
				ref,
				back.mReadMask
			);

			back.mReference = ref;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilReference(ref, EFace::Back);
			SetStencilReference(ref, EFace::Front);

		}

	}
	void GLDevice::SetStencilOp ( EStencilOp sfail, EStencilOp dpfail, EStencilOp dppass, EFace face ) {

		if (!mEnabledStates.Has(RState::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.mStencil;

		if (face == EFace::Front) {

			auto& front = stencil.mFront;
			
			if (front.mStencilFailOp == sfail && front.mDepthFailOp == dpfail && front.mPassOp == dppass) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}


			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[LookupCast(sfail)],
				skStencilOpLookup[LookupCast(dpfail)],
				skStencilOpLookup[LookupCast(dppass)]
			);

			front.mStencilFailOp = sfail;
			front.mDepthFailOp = dpfail;
			front.mPassOp = dppass;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == EFace::Back) {

			auto& back = stencil.mBack;

			if (back.mStencilFailOp == sfail && back.mDepthFailOp == dpfail && back.mPassOp == dppass) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[LookupCast(sfail)],
				skStencilOpLookup[LookupCast(dpfail)],
				skStencilOpLookup[LookupCast(dppass)]
			);

			back.mStencilFailOp = sfail;
			back.mDepthFailOp = dpfail;
			back.mPassOp = dppass;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOp(sfail, dpfail, dppass, EFace::Back);
			SetStencilOp(sfail, dpfail, dppass, EFace::Front);

		}

	}
	void GLDevice::SetStencilOpOnStencilFail ( EStencilOp op, EFace face ) {

		if (!mEnabledStates.Has(RState::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.mStencil;

		if (face == EFace::Front) {

			auto& front = stencil.mFront;

			if (front.mStencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[LookupCast(op)],
				skStencilOpLookup[LookupCast(front.mDepthFailOp)],
				skStencilOpLookup[LookupCast(front.mPassOp)]
			);

			front.mStencilFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == EFace::Back) {

			auto& back = stencil.mBack;

			if (back.mStencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[LookupCast(op)],
				skStencilOpLookup[LookupCast(back.mDepthFailOp)],
				skStencilOpLookup[LookupCast(back.mPassOp)]
			);

			back.mStencilFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOpOnStencilFail(op, EFace::Back);
			SetStencilOpOnStencilFail(op, EFace::Front);

		}

	}
	void GLDevice::SetStencilOpOnDepthFail ( EStencilOp op, EFace face ) {

		if (!mEnabledStates.Has(RState::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.mStencil;

		if (face == EFace::Front) {

			auto& front = stencil.mFront;

			if (front.mStencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[LookupCast(front.mStencilFailOp)],
				skStencilOpLookup[LookupCast(op)],
				skStencilOpLookup[LookupCast(front.mPassOp)]
			);

			front.mDepthFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == EFace::Back) {

			auto& back = stencil.mBack;

			if (back.mStencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[LookupCast(back.mStencilFailOp)],
				skStencilOpLookup[LookupCast(op)],
				skStencilOpLookup[LookupCast(back.mPassOp)]
			);

			back.mDepthFailOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOpOnDepthFail(op, EFace::Back);
			SetStencilOpOnDepthFail(op, EFace::Front);

		}

	}
	void GLDevice::SetStencilOpOnDepthPass ( EStencilOp op, EFace face ) {

		if (!mEnabledStates.Has(RState::StencilTest)) {
			return;
		}

		auto& stencil = mDepthStencilState.mStencil;

		if (face == EFace::Front) {

			auto& front = stencil.mFront;

			if (front.mPassOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				skStencilOpLookup[LookupCast(front.mStencilFailOp)],
				skStencilOpLookup[LookupCast(front.mDepthFailOp)],
				skStencilOpLookup[LookupCast(op)]
			);

			front.mPassOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else if (face == EFace::Back) {

			auto& back = stencil.mBack;

			if (back.mPassOp == op) {
				LUM_PROFILER_CACHE_HIT();
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				skStencilOpLookup[LookupCast(back.mStencilFailOp)],
				skStencilOpLookup[LookupCast(back.mDepthFailOp)],
				skStencilOpLookup[LookupCast(op)]
			);

			back.mPassOp = op;

			LUM_PROFILER_CACHE_MISS();

		}
		else {

			SetStencilOpOnDepthFail(op, EFace::Back);
			SetStencilOpOnDepthFail(op, EFace::Front);

		}

	}


	// Rasterizer setters

	void GLDevice::ToggleDepthBias ( bool toggle ) {

		if (toggle == mEnabledStates.Has(RState::DepthBias)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.Enable(RState::DepthBias);

		}
		else {

			glDisable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.Disable(RState::DepthBias);

		}

		LUM_PROFILER_CACHE_MISS();
	
	}
	bool GLDevice::IsDepthBiasEnabled ( ) const noexcept {
		return mEnabledStates.Has(RState::DepthBias);
	}
	void GLDevice::SetDepthBiasFactors ( float32 slope, float32 units ) {

		if (!mEnabledStates.Has(RState::DepthBias)) {
			return;
		}

		if (mRasterizerState.mDepthBias.mSlopeFactor == slope && mRasterizerState.mDepthBias.mUnits == units) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slope, units);

		mRasterizerState.mDepthBias.mSlopeFactor = slope;
		mRasterizerState.mDepthBias.mUnits = units;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasClamp ( float32 clamp ) {

		if (!mEnabledStates.Has(RState::DepthBias)) {
			return;
		}

		if (mRasterizerState.mDepthBias.mClamp == clamp) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffsetClamp(
			mRasterizerState.mDepthBias.mSlopeFactor,
			mRasterizerState.mDepthBias.mUnits,
			clamp
		); 

		mRasterizerState.mDepthBias.mClamp = clamp;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasSlope ( float32 slopeFactor ) {

		if (!mEnabledStates.Has(RState::DepthBias)) {
			return;
		}

		if (mRasterizerState.mDepthBias.mSlopeFactor == slopeFactor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slopeFactor, mRasterizerState.mDepthBias.mUnits);
		mRasterizerState.mDepthBias.mSlopeFactor = slopeFactor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasConstant ( float32 constantBias ) {

		if (!mEnabledStates.Has(RState::DepthBias)) {
			return;
		}

		if (mRasterizerState.mDepthBias.mUnits == constantBias) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(mRasterizerState.mDepthBias.mSlopeFactor, constantBias);
		mRasterizerState.mDepthBias.mUnits = constantBias;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetTopology ( ETopologyMode mode, EFace face ) {

		if (mRasterizerState.mPolygon.mTopologyMode == mode && mRasterizerState.mPolygon.mTopologyModeFaces == face) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonMode(
			skFacesLookup[LookupCast(face)],
			skTopologyModeLookup[LookupCast(mode)]
		);

		mRasterizerState.mPolygon.mTopologyMode = mode;
		mRasterizerState.mPolygon.mTopologyModeFaces = face;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetPointSize ( float32 size ) {

		if (mRasterizerState.mPolygon.mPointSize == size) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPointSize(size);

		mRasterizerState.mPolygon.mPointSize = size;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetLineWidth ( float32 width ) {

		if (mRasterizerState.mPolygon.mLineWidth == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glLineWidth(width);

		mRasterizerState.mPolygon.mLineWidth = width;

		LUM_PROFILER_CACHE_MISS();

	}


	// Multisample

	void GLDevice::ToggleMultisample(bool toggle) {

		if (toggle == mEnabledStates.Has(RState::Multisample)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_MULTISAMPLE);
			mEnabledStates.Enable(RState::Multisample);

		}
		else {

			glDisable(GL_MULTISAMPLE);
			mEnabledStates.Disable(RState::Multisample);

		}

		LUM_PROFILER_CACHE_MISS();

	}

	// TODO implement
	void GLDevice::ToggleSampleCoverage(bool toggle) {


	}
	void GLDevice::ToggleSampleAlphaToCoverage(bool toggle) {


	}
	void GLDevice::ToggleSampleAlphaToOne(bool toggle) {


	}
	void GLDevice::SetSampleCoverage(float32 value, bool invert) {


	}

}