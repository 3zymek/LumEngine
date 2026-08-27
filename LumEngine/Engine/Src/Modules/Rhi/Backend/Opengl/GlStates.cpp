//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: RHI OpenGL implementation of state setters operations and management
//
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	// Viewport setters

	void GLDevice::SetViewport( int32 x, int32 y, int32 width, int32 height ) {

		if (m_ViewportState.m_X == x && m_ViewportState.m_Y == y && m_ViewportState.m_Width == width && m_ViewportState.m_Height == height) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glViewport( x, y, width, height );

		m_ViewportState.m_X = x;
		m_ViewportState.m_Y = y;
		m_ViewportState.m_Width = width;
		m_ViewportState.m_Height = height;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetViewportX( int32 x ) {

		if (m_ViewportState.m_X == x) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glViewport(
			x,
			m_ViewportState.m_Y,
			m_ViewportState.m_Width,
			m_ViewportState.m_Height
		);

		m_ViewportState.m_X = x;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetViewportY( int32 y ) {

		if (m_ViewportState.m_Y == y) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glViewport(
			m_ViewportState.m_X,
			y,
			m_ViewportState.m_Width,
			m_ViewportState.m_Height
		);

		m_ViewportState.m_Y = y;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetViewportWidth( int32 width ) {

		if (m_ViewportState.m_Width == width) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glViewport(
			m_ViewportState.m_X,
			m_ViewportState.m_Y,
			width,
			m_ViewportState.m_Height
		);

		m_ViewportState.m_Width = width;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetViewportHeight( int32 height ) {

		if (m_ViewportState.m_Height == height) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glViewport(
			m_ViewportState.m_X,
			m_ViewportState.m_Y,
			m_ViewportState.m_Width,
			height
		);

		m_ViewportState.m_Height = height;

		LUM_PROFILER_CACHE_MISS( );

	}


	// Scissors setters

	void GLDevice::ToggleScissors( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::Scissor )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_SCISSOR_TEST );
			m_EnabledStates.Set( State::Scissor );

		}
		else {

			glDisable( GL_SCISSOR_TEST );
			m_EnabledStates.Unset( State::Scissor );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetScissors( int32 x, int32 y, int32 width, int32 height ) {

		if (m_ScissorState.m_X == x && m_ScissorState.m_Y == y && m_ScissorState.m_Width == width && m_ScissorState.m_Height == height) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		m_ScissorState.m_X = x;
		m_ScissorState.m_Y = y;
		m_ScissorState.m_Width = width;
		m_ScissorState.m_Height = height;

		glScissor( x, y, width, height );

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetScissorX( int32 x ) {

		if (m_ScissorState.m_X == x) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glScissor(
			x,
			m_ScissorState.m_Y,
			m_ScissorState.m_Width,
			m_ScissorState.m_Height
		);

		m_ScissorState.m_X = x;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetScissorY( int32 y ) {

		if (m_ScissorState.m_Y == y) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glScissor(
			m_ScissorState.m_X,
			y,
			m_ScissorState.m_Width,
			m_ScissorState.m_Height
		);

		m_ScissorState.m_Y = y;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetScissorWidth( int32 width ) {

		if (m_ScissorState.m_Width == width) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glScissor(
			m_ScissorState.m_X,
			m_ScissorState.m_Y,
			width,
			m_ScissorState.m_Height
		);

		m_ScissorState.m_Width = width;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetScissorHeight( int32 height ) {

		if (m_ScissorState.m_Height == height) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glScissor(
			m_ScissorState.m_X,
			m_ScissorState.m_Y,
			m_ScissorState.m_Width,
			height
		);

		m_ScissorState.m_Height = height;

		LUM_PROFILER_CACHE_MISS( );

	}


	// Cull setters

	void GLDevice::ToggleCull( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::Cull )) {

			LUM_PROFILER_CACHE_HIT( );
			return;

		}

		if (toggle) {

			glEnable( GL_CULL_FACE );
			m_EnabledStates.Set( State::Cull );

		}
		else {

			glDisable( GL_CULL_FACE );
			m_EnabledStates.Unset( State::Cull );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetCullFace( Face face ) {

		if (face == m_CullState.m_Face) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glCullFace( sk_FacesLookup[ LookupCast( face ) ] );

		m_CullState.m_Face = face;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetFrontFace( WindingOrder order ) {

		if (order == m_CullState.m_WindingOrder) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glFrontFace( (order == WindingOrder::Clockwise) ? GL_CW : GL_CCW );

		m_CullState.m_WindingOrder = order;

		LUM_PROFILER_CACHE_MISS( );

	}


	// Blend setters

	void GLDevice::ToggleBlend( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::Blend )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_BLEND );
			m_EnabledStates.Set( State::Blend );

		}
		else {

			glDisable( GL_BLEND );
			m_EnabledStates.Unset( State::Blend );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendConstantColor( Vector4 rgba ) {

		if (m_BlendState.m_BlendColor == rgba) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendColor( rgba.mR, rgba.mG, rgba.mB, rgba.mA );

		m_BlendState.m_BlendColor = rgba;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendFactors( BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha ) {


		if (m_BlendState.m_SrcColorFactor == srcColor && m_BlendState.m_DstColorFactor == dstColor &&
			 m_BlendState.m_SrcAlphaFactor == srcAlpha && m_BlendState.m_DstAlphaFactor == dstAlpha) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendFuncSeparate(
			sk_BlendFactorLookup[ LookupCast( srcColor ) ],
			sk_BlendFactorLookup[ LookupCast( dstColor ) ],
			sk_BlendFactorLookup[ LookupCast( srcAlpha ) ],
			sk_BlendFactorLookup[ LookupCast( dstAlpha ) ]
		);

		m_BlendState.m_SrcColorFactor = srcColor;
		m_BlendState.m_DstColorFactor = dstColor;
		m_BlendState.m_SrcAlphaFactor = srcAlpha;
		m_BlendState.m_DstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendColorFactors( BlendFactor srcColor, BlendFactor dstColor ) {

		if (m_BlendState.m_SrcColorFactor == srcColor && m_BlendState.m_DstColorFactor == dstColor) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendFuncSeparate(
			sk_BlendFactorLookup[ LookupCast( srcColor ) ],
			sk_BlendFactorLookup[ LookupCast( dstColor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcAlphaFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstAlphaFactor ) ]
		);

		m_BlendState.m_SrcColorFactor = srcColor;
		m_BlendState.m_DstColorFactor = dstColor;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendAlphaFactors( BlendFactor srcAlpha, BlendFactor dstAlpha ) {

		if (m_BlendState.m_SrcAlphaFactor == srcAlpha && m_BlendState.m_DstAlphaFactor == dstAlpha) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendFuncSeparate(
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( srcAlpha ) ],
			sk_BlendFactorLookup[ LookupCast( dstAlpha ) ]
		);

		m_BlendState.m_SrcAlphaFactor = srcAlpha;
		m_BlendState.m_DstAlphaFactor = dstAlpha;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendSrcColorFactor( BlendFactor factor ) {

		if (m_BlendState.m_SrcColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendFuncSeparate(
			sk_BlendFactorLookup[ LookupCast( factor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcAlphaFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstAlphaFactor ) ]
		);

		m_BlendState.m_SrcColorFactor = factor;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendDstColorFactor( BlendFactor factor ) {

		if (m_BlendState.m_DstColorFactor == factor) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendFuncSeparate(
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( factor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcAlphaFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstAlphaFactor ) ]
		);

		m_BlendState.m_DstColorFactor = factor;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendSrcAlphaFactor( BlendFactor factor ) {

		if (m_BlendState.m_SrcAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendFuncSeparate(
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( factor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstAlphaFactor ) ]
		);

		m_BlendState.m_SrcAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendDstAlphaFactor( BlendFactor factor ) {

		if (m_BlendState.m_DstAlphaFactor == factor) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendFuncSeparate(
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_DstColorFactor ) ],
			sk_BlendFactorLookup[ LookupCast( m_BlendState.m_SrcAlphaFactor ) ],
			sk_BlendFactorLookup[ LookupCast( factor ) ]
		);

		m_BlendState.m_DstAlphaFactor = factor;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendOp( BlendOp colorOp, BlendOp alphaOp ) {


		if (m_BlendState.m_AlphaOp == alphaOp && m_BlendState.m_ColorOp == colorOp) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendEquationSeparate(
			sk_BlendOpLookup[ LookupCast( colorOp ) ],
			sk_BlendOpLookup[ LookupCast( alphaOp ) ]
		);

		m_BlendState.m_AlphaOp = alphaOp;
		m_BlendState.m_ColorOp = colorOp;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendColorOp( BlendOp op ) {

		if (m_BlendState.m_ColorOp == op) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendEquationSeparate(
			sk_BlendOpLookup[ LookupCast( op ) ],
			sk_BlendOpLookup[ LookupCast( m_BlendState.m_AlphaOp ) ]
		);

		m_BlendState.m_ColorOp = op;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetBlendAlphaOp( BlendOp op ) {

		if (m_BlendState.m_AlphaOp == op) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glBlendEquationSeparate(
			sk_BlendOpLookup[ LookupCast( m_BlendState.m_ColorOp ) ],
			sk_BlendOpLookup[ LookupCast( op ) ]
		);

		m_BlendState.m_AlphaOp = op;

		LUM_PROFILER_CACHE_MISS( );

	}
	// TODO implement:
	void GLDevice::SetBlendFactorsForTarget( uint8 target ) {

	}
	void GLDevice::ToggleBlendForTarget( uint8 target, bool toggle ) {

	}


	// Depth setters

	void GLDevice::ToggleDepthWrite( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::DepthWrite )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glDepthMask( ( GLboolean ) toggle );
			m_EnabledStates.Set( State::DepthWrite );

		}
		else {

			glDepthMask( ( GLboolean ) toggle );
			m_EnabledStates.Unset( State::DepthWrite );

		}

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::ToggleDepthTest( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::DepthTest )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_DEPTH_TEST );
			m_EnabledStates.Set( State::DepthTest );

		}
		else {

			glDisable( GL_DEPTH_TEST );
			m_EnabledStates.Unset( State::DepthTest );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetDepthFunc( CompareFlag func ) {

		if (m_DepthStencilState.m_Depth.m_Compare == func) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glDepthFunc( sk_CompareFlagLookup[ LookupCast( func ) ] );

		LUM_PROFILER_CACHE_MISS( );

	}


	// Stencil setters

	void GLDevice::ToggleStencilTest( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::StencilTest )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_STENCIL_TEST );
			m_EnabledStates.Set( State::StencilTest );

		}
		else {

			glDisable( GL_STENCIL_TEST );
			m_EnabledStates.Unset( State::StencilTest );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetStencilReference( int32 ref, Face face ) {

		if (face == Face::Front) {

			StencilFaceState& front = m_DepthStencilState.m_Stencil.m_Front;

			if (front.m_Reference == ref) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilFuncSeparate(
				GL_FRONT,
				sk_CompareFlagLookup[ LookupCast( front.m_CompareFlag ) ],
				ref,
				front.m_ReadMask
			);

			front.m_Reference = ref;

			LUM_PROFILER_CACHE_MISS( );

		}
		else if (face == Face::Back) {

			StencilFaceState& back = m_DepthStencilState.m_Stencil.m_Back;

			if (back.m_Reference == ref) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilFuncSeparate(
				GL_BACK,
				sk_CompareFlagLookup[ LookupCast( back.m_CompareFlag ) ],
				ref,
				back.m_ReadMask
			);

			back.m_Reference = ref;

			LUM_PROFILER_CACHE_MISS( );

		}
		else {

			SetStencilReference( ref, Face::Back );
			SetStencilReference( ref, Face::Front );

		}

	}
	void GLDevice::SetStencilOp( StencilOp sfail, StencilOp dpfail, StencilOp dppass, Face face ) {

		auto& stencil = m_DepthStencilState.m_Stencil;

		if (face == Face::Front) {

			auto& front = stencil.m_Front;

			if (front.m_StencilFailOp == sfail && front.m_DepthFailOp == dpfail && front.m_PassOp == dppass) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}


			glStencilOpSeparate(
				GL_FRONT,
				sk_StencilOpLookup[ LookupCast( sfail ) ],
				sk_StencilOpLookup[ LookupCast( dpfail ) ],
				sk_StencilOpLookup[ LookupCast( dppass ) ]
			);

			front.m_StencilFailOp = sfail;
			front.m_DepthFailOp = dpfail;
			front.m_PassOp = dppass;

			LUM_PROFILER_CACHE_MISS( );

		}
		else if (face == Face::Back) {

			auto& back = stencil.m_Back;

			if (back.m_StencilFailOp == sfail && back.m_DepthFailOp == dpfail && back.m_PassOp == dppass) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				sk_StencilOpLookup[ LookupCast( sfail ) ],
				sk_StencilOpLookup[ LookupCast( dpfail ) ],
				sk_StencilOpLookup[ LookupCast( dppass ) ]
			);

			back.m_StencilFailOp = sfail;
			back.m_DepthFailOp = dpfail;
			back.m_PassOp = dppass;

			LUM_PROFILER_CACHE_MISS( );

		}
		else {

			SetStencilOp( sfail, dpfail, dppass, Face::Back );
			SetStencilOp( sfail, dpfail, dppass, Face::Front );

		}

	}
	void GLDevice::SetStencilOpOnStencilFail( StencilOp op, Face face ) {

		auto& stencil = m_DepthStencilState.m_Stencil;

		if (face == Face::Front) {

			auto& front = stencil.m_Front;

			if (front.m_StencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				sk_StencilOpLookup[ LookupCast( op ) ],
				sk_StencilOpLookup[ LookupCast( front.m_DepthFailOp ) ],
				sk_StencilOpLookup[ LookupCast( front.m_PassOp ) ]
			);

			front.m_StencilFailOp = op;

			LUM_PROFILER_CACHE_MISS( );

		}
		else if (face == Face::Back) {

			auto& back = stencil.m_Back;

			if (back.m_StencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				sk_StencilOpLookup[ LookupCast( op ) ],
				sk_StencilOpLookup[ LookupCast( back.m_DepthFailOp ) ],
				sk_StencilOpLookup[ LookupCast( back.m_PassOp ) ]
			);

			back.m_StencilFailOp = op;

			LUM_PROFILER_CACHE_MISS( );

		}
		else {

			SetStencilOpOnStencilFail( op, Face::Back );
			SetStencilOpOnStencilFail( op, Face::Front );

		}

	}
	void GLDevice::SetStencilOpOnDepthFail( StencilOp op, Face face ) {

		auto& stencil = m_DepthStencilState.m_Stencil;

		if (face == Face::Front) {

			auto& front = stencil.m_Front;

			if (front.m_StencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				sk_StencilOpLookup[ LookupCast( front.m_StencilFailOp ) ],
				sk_StencilOpLookup[ LookupCast( op ) ],
				sk_StencilOpLookup[ LookupCast( front.m_PassOp ) ]
			);

			front.m_DepthFailOp = op;

			LUM_PROFILER_CACHE_MISS( );

		}
		else if (face == Face::Back) {

			auto& back = stencil.m_Back;

			if (back.m_StencilFailOp == op) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				sk_StencilOpLookup[ LookupCast( back.m_StencilFailOp ) ],
				sk_StencilOpLookup[ LookupCast( op ) ],
				sk_StencilOpLookup[ LookupCast( back.m_PassOp ) ]
			);

			back.m_DepthFailOp = op;

			LUM_PROFILER_CACHE_MISS( );

		}
		else {

			SetStencilOpOnDepthFail( op, Face::Back );
			SetStencilOpOnDepthFail( op, Face::Front );

		}

	}
	void GLDevice::SetStencilOpOnDepthPass( StencilOp op, Face face ) {

		auto& stencil = m_DepthStencilState.m_Stencil;

		if (face == Face::Front) {

			StencilFaceState& front = stencil.m_Front;

			if (front.m_PassOp == op) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilOpSeparate(
				GL_FRONT,
				sk_StencilOpLookup[ LookupCast( front.m_StencilFailOp ) ],
				sk_StencilOpLookup[ LookupCast( front.m_DepthFailOp ) ],
				sk_StencilOpLookup[ LookupCast( op ) ]
			);

			front.m_PassOp = op;

			LUM_PROFILER_CACHE_MISS( );

		}
		else if (face == Face::Back) {

			StencilFaceState& back = stencil.m_Back;

			if (back.m_PassOp == op) {
				LUM_PROFILER_CACHE_HIT( );
				return;
			}

			glStencilOpSeparate(
				GL_BACK,
				sk_StencilOpLookup[ LookupCast( back.m_StencilFailOp ) ],
				sk_StencilOpLookup[ LookupCast( back.m_DepthFailOp ) ],
				sk_StencilOpLookup[ LookupCast( op ) ]
			);

			back.m_PassOp = op;

			LUM_PROFILER_CACHE_MISS( );

		}
		else {

			SetStencilOpOnDepthFail( op, Face::Back );
			SetStencilOpOnDepthFail( op, Face::Front );

		}

	}


	// Rasterizer setters

	void GLDevice::ToggleDepthBias( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::DepthBias )) {

			LUM_PROFILER_CACHE_HIT( );
			return;

		}

		if (toggle) {

			glEnable( GL_POLYGON_OFFSET_FILL );
			m_EnabledStates.Set( State::DepthBias );

		}
		else {

			glDisable( GL_POLYGON_OFFSET_FILL );
			m_EnabledStates.Unset( State::DepthBias );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetDepthBiasFactors( float32 slope, float32 units ) {

		if (m_RasterizerState.m_DepthBias.m_SlopeFactor == slope && m_RasterizerState.m_DepthBias.m_Units == units) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glPolygonOffset( slope, units );

		m_RasterizerState.m_DepthBias.m_SlopeFactor = slope;
		m_RasterizerState.m_DepthBias.m_Units = units;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetDepthBiasClamp( float32 clamp ) {

		if (m_RasterizerState.m_DepthBias.m_Clamp == clamp) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glPolygonOffsetClamp(
			m_RasterizerState.m_DepthBias.m_SlopeFactor,
			m_RasterizerState.m_DepthBias.m_Units,
			clamp
		);

		m_RasterizerState.m_DepthBias.m_Clamp = clamp;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetDepthBiasSlope( float32 slopeFactor ) {

		if (m_RasterizerState.m_DepthBias.m_SlopeFactor == slopeFactor) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glPolygonOffset( slopeFactor, m_RasterizerState.m_DepthBias.m_Units );
		m_RasterizerState.m_DepthBias.m_SlopeFactor = slopeFactor;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetDepthBiasConstant( float32 constantBias ) {

		if (m_RasterizerState.m_DepthBias.m_Units == constantBias) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glPolygonOffset( m_RasterizerState.m_DepthBias.m_SlopeFactor, constantBias );
		m_RasterizerState.m_DepthBias.m_Units = constantBias;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetTopology( TopologyMode mode, Face face ) {

		if (m_RasterizerState.m_Polygon.m_TopologyMode == mode && m_RasterizerState.m_Polygon.m_TopologyModeFaces == face) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glPolygonMode(
			sk_FacesLookup[ LookupCast( face ) ],
			sk_TopologyModeLookup[ LookupCast( mode ) ]
		);

		m_RasterizerState.m_Polygon.m_TopologyMode = mode;
		m_RasterizerState.m_Polygon.m_TopologyModeFaces = face;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetPointSize( float32 size ) {

		if (m_RasterizerState.m_Polygon.m_PointSize == size) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glPointSize( size );

		m_RasterizerState.m_Polygon.m_PointSize = size;

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetLineWidth( float32 width ) {

		if (m_RasterizerState.m_Polygon.m_LineWidth == width) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		glLineWidth( width );

		m_RasterizerState.m_Polygon.m_LineWidth = width;

		LUM_PROFILER_CACHE_MISS( );

	}


	// Multisample

	void GLDevice::ToggleMultisample( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::Multisample )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_MULTISAMPLE );
			m_EnabledStates.Set( State::Multisample );

		}
		else {

			glDisable( GL_MULTISAMPLE );
			m_EnabledStates.Unset( State::Multisample );

		}

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::ToggleSampleCoverage( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::SampleCoverage )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_SAMPLE_COVERAGE );
			m_EnabledStates.Set( State::SampleCoverage );

		}
		else {

			glDisable( GL_SAMPLE_COVERAGE );
			m_EnabledStates.Unset( State::SampleCoverage );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ToggleSampleAlphaToCoverage( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::SampleAlphaToCoverage )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE );
			m_EnabledStates.Set( State::SampleAlphaToCoverage );

		}
		else {

			glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
			m_EnabledStates.Unset( State::SampleAlphaToCoverage );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::ToggleSampleAlphaToOne( bool toggle ) {

		if (toggle == m_EnabledStates.Has( State::SampleAlphaToOne )) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		if (toggle) {

			glEnable( GL_SAMPLE_ALPHA_TO_ONE );
			m_EnabledStates.Set( State::SampleAlphaToOne );

		}
		else {

			glDisable( GL_SAMPLE_ALPHA_TO_ONE );
			m_EnabledStates.Unset( State::SampleAlphaToOne );

		}

		LUM_PROFILER_CACHE_MISS( );

	}
	void GLDevice::SetSampleCoverage( float32 value, bool invert ) {

		auto& msaaState = m_RasterizerState.mMsaa;

		if (value == msaaState.m_SampleCoverage && invert == msaaState.m_CoverageInvert) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		msaaState.m_SampleCoverage = value;
		msaaState.m_CoverageInvert = invert;

		glSampleCoverage( value, invert ? GL_TRUE : GL_FALSE );

		LUM_PROFILER_CACHE_MISS( );

	}

}