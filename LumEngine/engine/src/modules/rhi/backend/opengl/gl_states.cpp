#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	// Scissors setters

	void GLDevice::ToggleScissors ( bool toggle ) {

		if (toggle == mEnabledStates.has(State::Scissor)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_SCISSOR_TEST);
			mEnabledStates.enable(State::Scissor);

		}
		else {

			glDisable(GL_SCISSOR_TEST);
			mEnabledStates.disable(State::Scissor);

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsScissorEnabled ( ) const noexcept {
		return mEnabledStates.has(State::Scissor);
	}
	void GLDevice::SetScissors ( int32 x, int32 y, int32 width, int32 height ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), LUM_SEV_DEBUG, "Scissor state not enabled");

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
	void GLDevice::SetScissorX ( int32 x ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), LUM_SEV_DEBUG, "Scissor not enabled");

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
	void GLDevice::SetScissorY ( int32 y ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), LUM_SEV_DEBUG, "Scissor not enabled");

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
	void GLDevice::SetScissorWidth ( int32 width ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), LUM_SEV_DEBUG, "Scissor not enabled");

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
	void GLDevice::SetScissorHeight ( int32 height ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Scissor), LUM_SEV_DEBUG, "Scissor not enabled");

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

	void GLDevice::ToggleCull ( bool toggle ) {

		if (toggle == mEnabledStates.has(State::Cull)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {
			glEnable(GL_CULL_FACE);
			mEnabledStates.enable(State::Cull);
		}
		else {
			glDisable(GL_CULL_FACE);
			mEnabledStates.disable(State::Cull);
		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsCullEnabled ( ) const noexcept {
		return mEnabledStates.has(State::Cull);
	}
	void GLDevice::SetCullFace ( Face face ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Cull), LUM_SEV_DEBUG, "Cull not enabled");

		if (face == mCullState.face) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glCullFace(skFacesLookup[lookup_cast(face)]);

		mCullState.face = face;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetFrontFace ( WindingOrder order ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Cull), LUM_SEV_DEBUG, "Cull not enabled");

		if (order == mCullState.windingOrder) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glFrontFace((order == WindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.windingOrder = order;

		LUM_PROFILER_CACHE_MISS();

	}


	// Blend setters

	void GLDevice::ToggleBlend ( bool toggle ) {

		if (toggle == mEnabledStates.has(State::Blend)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {
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
	bool GLDevice::IsBlendEnabled ( ) const noexcept {
		return mEnabledStates.has(State::Blend);
	}
	void GLDevice::SetBlendConstantColor ( ChannelRGBA rgba ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

		if (mBlendState.blendColor == rgba) {
			LUM_PROFILER_CACHE_HIT();
		}

		glBlendColor(rgba.r, rgba.g, rgba.b, rgba.a);

		mBlendState.blendColor = rgba;

		LUM_PROFILER_CACHE_MISS();
	
	}
	void GLDevice::SetBlendFactors ( BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendColorFactors ( BlendFactor srcColor, BlendFactor dstColor ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendAlphaFactors ( BlendFactor srcAlpha, BlendFactor dstAlpha ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendSrcColorFactor ( BlendFactor factor ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendDstColorFactor ( BlendFactor factor ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendSrcAlphaFactor ( BlendFactor factor ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendDstAlphaFactor ( BlendFactor factor ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendOp ( BlendOp colorOp, BlendOp alphaOp ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

		if (mBlendState.alphaOp == alphaOp && mBlendState.colorOp == colorOp) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(colorOp)],
			skBlendOpLookup[lookup_cast(alphaOp)]
		);

		mBlendState.alphaOp = alphaOp;
		mBlendState.colorOp = colorOp;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetBlendColorOp ( BlendOp op) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendAlphaOp ( BlendOp op) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::Blend), LUM_SEV_DEBUG, "Blend not enabled");

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
	void GLDevice::SetBlendFactorsForTarget ( uint8 target ) {

	}
	void GLDevice::ToggleBlendForTarget(uint8 target, bool toggle ) {

	}


	// Depth setters

	void GLDevice::ToggleDepthWrite ( bool toggle ) {

		if (toggle == mEnabledStates.has(State::DepthWrite)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.enable(State::DepthWrite);
			LUM_LOG_DEBUG("Depth write enabled");

		}
		else {

			glDepthMask((GLboolean)toggle);
			mEnabledStates.disable(State::DepthWrite);
			LUM_LOG_DEBUG("Depth write disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsDepthWriteEnabled ( ) const noexcept {
		return mEnabledStates.has(State::DepthWrite);
	}

	void GLDevice::ToggleDepthTest ( bool toggle ) {

		if (toggle == mEnabledStates.has(State::DepthTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_DEPTH_TEST);
			mEnabledStates.enable(State::DepthTest);
			LUM_LOG_DEBUG("Depth test enabled");

		}
		else {

			glDisable(GL_DEPTH_TEST);
			mEnabledStates.disable(State::DepthTest);
			LUM_LOG_DEBUG("Depth test disabled");

		}

		LUM_PROFILER_CACHE_MISS();

	}
	bool GLDevice::IsDepthTestEnabled ( )	const noexcept {
		return mEnabledStates.has(State::DepthTest);
	}
	void GLDevice::SetDepthFunc ( CompareFlag func ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::DepthTest), LUM_SEV_DEBUG, "Depth not enabled");

		if (mDepthStencilState.depth.compareFlag == func) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glDepthFunc(skCompareFlagLookup[lookup_cast(func)]);

		LUM_PROFILER_CACHE_MISS();

	}
	

	// Stencil setters

	void GLDevice::ToggleStencilTest ( bool toggle ) {

		if (toggle == mEnabledStates.has(State::StencilTest)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

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
	bool GLDevice::IsStencilTestEnabled ( ) const noexcept {
		return mEnabledStates.has(State::StencilTest);
	}
	void GLDevice::SetStencilReference ( int32 ref, Face face = Face::FrontBack ) {

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::StencilTest), LUM_SEV_DEBUG, "Stencil not enabled");

		if (face == Face::Front) {

			auto& front = mDepthStencilState.stencil.front;

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
			auto& back = mDepthStencilState.stencil.back;

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

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::StencilTest), LUM_SEV_DEBUG, "Stencil not enabled");

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.front;
			
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

			auto& back = stencil.back;

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

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::StencilTest), LUM_SEV_DEBUG, "Stencil not enabled");

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.front;

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

			auto& back = stencil.back;

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

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::StencilTest), LUM_SEV_DEBUG, "Stencil not enabled");

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.front;

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

			auto& back = stencil.back;

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

		LUM_HOTCHK_RETURN_VOID(mEnabledStates.has(State::StencilTest), LUM_SEV_DEBUG, "Stencil not enabled");

		auto& stencil = mDepthStencilState.stencil;

		if (face == Face::Front) {

			auto& front = stencil.front;

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

			auto& back = stencil.back;

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

		if (toggle == mEnabledStates.has(State::DepthBias)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.enable(State::DepthBias);

		}
		else {

			glDisable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.disable(State::DepthBias);

		}

		LUM_PROFILER_CACHE_MISS();
	
	}
	bool GLDevice::IsDepthBiasEnabled ( ) const noexcept {
		return mEnabledStates.has(State::DepthBias);
	}
	void GLDevice::SetDepthBiasFactors ( float32 slope, float32 units ) {

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), LUM_SEV_DEBUG, "Depth bias not enabled");

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

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), LUM_SEV_DEBUG, "Depth bias not enabled");

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

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), LUM_SEV_DEBUG, "Depth bias not enabled");

		if (mRasterizerState.depthBias.slopeFactor == slopeFactor) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		glPolygonOffset(slopeFactor, mRasterizerState.depthBias.units);
		mRasterizerState.depthBias.slopeFactor = slopeFactor;

		LUM_PROFILER_CACHE_MISS();

	}
	void GLDevice::SetDepthBiasConstant ( float32 constantBias ) {

		LUM_HOTCHK(mEnabledStates.has(State::DepthBias), LUM_SEV_DEBUG, "Depth bias not enabled");

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

		if (toggle == mEnabledStates.has(State::Multisample)) {
			LUM_PROFILER_CACHE_HIT();
			return;
		}

		if (toggle) {

			glEnable(GL_MULTISAMPLE);
			mEnabledStates.enable(State::Multisample);

		}
		else {

			glDisable(GL_MULTISAMPLE);
			mEnabledStates.disable(State::Multisample);

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