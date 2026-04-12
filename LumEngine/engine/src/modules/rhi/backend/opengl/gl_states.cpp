//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: RHI OpenGL implementation of state setters operations and management
//
//=============================================================================//

#include "modules/rhi/backend/gl_device.hpp"

namespace lum::rhi::gl {

	// Viewport setters

	void GLDevice::SetViewport( int32 x, int32 y, int32 width, int32 height ) {

		if (mViewportState.x == x && mViewportState.y == y && mViewportState.mWidth == width && mViewportState.mHeight == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(x, y, width, height);

		mViewportState.x = x;
		mViewportState.y = y;
		mViewportState.mWidth = width;
		mViewportState.mHeight = height;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportX( int32 x ) {

		if (mViewportState.x == x) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			x,
			mViewportState.y,
			mViewportState.mWidth,
			mViewportState.mHeight
		);

		mViewportState.x = x;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportY( int32 y ) {

		if (mViewportState.y == y) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			y,
			mViewportState.mWidth,
			mViewportState.mHeight
		);

		mViewportState.y = y;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportWidth( int32 width ) {

		if (mViewportState.mWidth == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			mViewportState.y,
			width,
			mViewportState.mHeight
		);

		mViewportState.mWidth = width;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetViewportHeight( int32 height ) {

		if (mViewportState.mHeight == height) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glViewport(
			mViewportState.x,
			mViewportState.y,
			mViewportState.mWidth,
			height
		);

		mViewportState.mHeight = height;

		LUM_PROFILER_CACHE_MISS();

	}


	// Scissors setters

	void GLDevice::ToggleScissors( bool toggle ) {

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
	void GLDevice::SetScissors( int32 x, int32 y, int32 width, int32 height ) {

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
	void GLDevice::SetScissorX( int32 x ) {

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
	void GLDevice::SetScissorY( int32 y ) {

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
	void GLDevice::SetScissorWidth( int32 width ) {

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
	void GLDevice::SetScissorHeight( int32 height ) {

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

	void GLDevice::ToggleCull( bool toggle ) {

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
	void GLDevice::SetCullFace ( Face face ) {

		if (face == mCullState.mFace) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glCullFace(skFacesLookup[LookupCast(face)]);

		mCullState.mFace = face;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetFrontFace( WindingOrder order ) {

		if (order == mCullState.mWindingOrder) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glFrontFace((order == WindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.mWindingOrder = order;

		LUM_PROFILER_CACHE_MISS();

	}


	// Blend setters

	void GLDevice::ToggleBlend( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::Blend)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_BLEND);
			mEnabledStates.Enable(State::Blend);

		}
		else {

			glDisable(GL_BLEND);
			mEnabledStates.Disable(State::Blend);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendConstantColor( ChannelRGBA rgba ) {

		if (mBlendState.mBlendColor == rgba) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendColor(rgba.r, rgba.g, rgba.b, rgba.a);

		mBlendState.mBlendColor = rgba;

		LUM_PROFILER_CACHE_MISS();
	
	}
	void GLDevice::SetBlendFactors( BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha ) {


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
	void GLDevice::SetBlendColorFactors( BlendFactor srcColor, BlendFactor dstColor ) {

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
	void GLDevice::SetBlendAlphaFactors( BlendFactor srcAlpha, BlendFactor dstAlpha ) {

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
	void GLDevice::SetBlendSrcColorFactor( BlendFactor factor ) {

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
	void GLDevice::SetBlendDstColorFactor( BlendFactor factor ) {

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
	void GLDevice::SetBlendSrcAlphaFactor( BlendFactor factor ) {

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
	void GLDevice::SetBlendDstAlphaFactor( BlendFactor factor ) {

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
	void GLDevice::SetBlendOp( BlendOp colorOp, BlendOp alphaOp ) {


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
	void GLDevice::SetBlendColorOp( BlendOp op) {

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
	void GLDevice::SetBlendAlphaOp( BlendOp op) {

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
	void GLDevice::SetBlendFactorsForTarget( uint8 target ) {

	}
	void GLDevice::ToggleBlendForTarget( uint8 target, bool toggle ) {

	}


	// Depth setters

	void GLDevice::ToggleDepthWrite( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::DepthWrite)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.Enable(State::DepthWrite);

		}
		else {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.Disable(State::DepthWrite);

		}

		LUM_PROFILER_CACHE_MISS();

	}

	void GLDevice::ToggleDepthTest( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::DepthTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_DEPTH_TEST);
			mEnabledStates.Enable(State::DepthTest);

		}
		else {

			glDisable(GL_DEPTH_TEST);
			mEnabledStates.Disable(State::DepthTest);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthFunc ( CompareFlag func ) {

		if (mDepthStencilState.mDepth.mCompare == func) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glDepthFunc(skCompareFlagLookup[LookupCast(func)]);

		LUM_PROFILER_CACHE_MISS();

	}
	

	// Stencil setters

	void GLDevice::ToggleStencilTest( bool toggle ) {

		if (toggle == mEnabledStates.Has(State::StencilTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_STENCIL_TEST);
			mEnabledStates.Enable(State::StencilTest);

		}
		else {

			glDisable(GL_STENCIL_TEST);
			mEnabledStates.Disable(State::StencilTest);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetStencilReference( int32 ref, Face face ) {

		if (face == Face::Front) {

			FStencilFaceState& front = mDepthStencilState.mStencil.mFront;

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
		else if (face == Face::Back) {

			FStencilFaceState& back = mDepthStencilState.mStencil.mBack;

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

			SetStencilReference(ref, Face::Back);
			SetStencilReference(ref, Face::Front);

		}

	}
	void GLDevice::SetStencilOp( StencilOp sfail, StencilOp dpfail, StencilOp dppass, Face face ) {

		auto& stencil = mDepthStencilState.mStencil;

		if (face == Face::Front) {

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
		else if (face == Face::Back) {

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

			SetStencilOp(sfail, dpfail, dppass, Face::Back);
			SetStencilOp(sfail, dpfail, dppass, Face::Front);

		}

	}
	void GLDevice::SetStencilOpOnStencilFail( StencilOp op, Face face ) {

		auto& stencil = mDepthStencilState.mStencil;

		if (face == Face::Front) {

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
		else if (face == Face::Back) {

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

			SetStencilOpOnStencilFail(op, Face::Back);
			SetStencilOpOnStencilFail(op, Face::Front);

		}

	}
	void GLDevice::SetStencilOpOnDepthFail( StencilOp op, Face face ) {

		auto& stencil = mDepthStencilState.mStencil;

		if (face == Face::Front) {

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
		else if (face == Face::Back) {

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

			SetStencilOpOnDepthFail(op, Face::Back);
			SetStencilOpOnDepthFail(op, Face::Front);

		}

	}
	void GLDevice::SetStencilOpOnDepthPass( StencilOp op, Face face ) {

		auto& stencil = mDepthStencilState.mStencil;

		if (face == Face::Front) {

			FStencilFaceState& front = stencil.mFront;

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
		else if (face == Face::Back) {

			FStencilFaceState& back = stencil.mBack;

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

			SetStencilOpOnDepthFail(op, Face::Back);
			SetStencilOpOnDepthFail(op, Face::Front);

		}

	}


	// Rasterizer setters

	void GLDevice::ToggleDepthBias( bool toggle ) {

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
	void GLDevice::SetDepthBiasFactors( float32 slope, float32 units ) {

		if (mRasterizerState.mDepthBias.mSlopeFactor == slope && mRasterizerState.mDepthBias.mUnits == units) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slope, units);

		mRasterizerState.mDepthBias.mSlopeFactor = slope;
		mRasterizerState.mDepthBias.mUnits = units;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasClamp( float32 clamp ) {

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
	void GLDevice::SetDepthBiasSlope( float32 slopeFactor ) {

		if (mRasterizerState.mDepthBias.mSlopeFactor == slopeFactor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slopeFactor, mRasterizerState.mDepthBias.mUnits);
		mRasterizerState.mDepthBias.mSlopeFactor = slopeFactor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasConstant( float32 constantBias ) {

		if (mRasterizerState.mDepthBias.mUnits == constantBias) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(mRasterizerState.mDepthBias.mSlopeFactor, constantBias);
		mRasterizerState.mDepthBias.mUnits = constantBias;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetTopology( TopologyMode mode, Face face ) {

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
	void GLDevice::SetPointSize( float32 size ) {

		if (mRasterizerState.mPolygon.mPointSize == size) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPointSize(size);

		mRasterizerState.mPolygon.mPointSize = size;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetLineWidth( float32 width ) {

		if (mRasterizerState.mPolygon.mLineWidth == width) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glLineWidth(width);

		mRasterizerState.mPolygon.mLineWidth = width;

		LUM_PROFILER_CACHE_MISS();

	}


	// Multisample

	void GLDevice::ToggleMultisample( bool toggle ) {

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

	// TODO implement
	void GLDevice::ToggleSampleCoverage( bool toggle ) {


	}
	void GLDevice::ToggleSampleAlphaToCoverage( bool toggle ) {


	}
	void GLDevice::ToggleSampleAlphaToOne( bool toggle ) {


	}
	void GLDevice::SetSampleCoverage( float32 value, bool invert ) {


	}

}