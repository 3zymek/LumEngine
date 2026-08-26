//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: RHI OpenGL implementation pipeline operations and management
// 
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	void GLDevice::bind_check_shader( const Pipeline& pip ) noexcept {

		if (pip.m_Shader.m_Id == NullID<ShaderID>( ))
			return;

		BindShader( pip.m_Shader );

	}
	void GLDevice::bind_check_rasterizer( const Pipeline& pip ) noexcept {

		const auto& rast = pip.m_Rasterizer;

		SetTopology( rast.m_Polygon.m_TopologyMode, rast.m_Polygon.m_TopologyModeFaces );
		SetLineWidth( rast.m_Polygon.m_LineWidth );
		SetPointSize( rast.m_Polygon.m_PointSize );

		ToggleDepthBias( rast.m_DepthBias.bEnable );

		SetDepthBiasSlope( rast.m_DepthBias.m_SlopeFactor );
		SetDepthBiasClamp( rast.m_DepthBias.m_Clamp );

	}
	void GLDevice::bind_check_depth_stencil( const Pipeline& pip ) noexcept {

		const auto& depth = pip.m_DepthStencil.m_Depth;
		const auto& stencil = pip.m_DepthStencil.m_Stencil;

		ToggleDepthTest( depth.bEnabled );

		SetDepthFunc( depth.m_Compare );

		ToggleStencilTest( stencil.bEnabled );
		SetStencilReference( stencil.m_Front.m_Reference, Face::Front );
		SetStencilReference( stencil.m_Back.m_Reference, Face::Back );
		SetStencilOp( stencil.m_Front.m_StencilFailOp, stencil.m_Front.m_DepthFailOp, stencil.m_Front.m_PassOp, Face::Front );
		SetStencilOp( stencil.m_Back.m_StencilFailOp, stencil.m_Back.m_DepthFailOp, stencil.m_Back.m_PassOp, Face::Back );

	}
	void GLDevice::bind_check_scissors( const Pipeline& pip ) noexcept {

		const auto& scissors = pip.m_Scissor;

		ToggleScissors( pip.m_Scissor.bEnabled );
		SetScissors( scissors.x, scissors.y, scissors.m_Width, scissors.m_Height ); // Default options

	}
	void GLDevice::bind_check_blend( const Pipeline& pip ) noexcept {

		const auto& blend = pip.m_Blend;

		ToggleBlend( blend.bEnabled );

		SetBlendFactors( blend.m_SrcColorFactor, blend.m_DstColorFactor, blend.m_SrcAlphaFactor, blend.m_DstAlphaFactor );
		SetBlendOp( blend.m_ColorOp, blend.m_AlphaOp );

	}
	void GLDevice::bind_check_cull( const Pipeline& pip ) noexcept {

		const auto& cull = pip.m_Cull;

		ToggleCull( cull.bEnabled );

		SetCullFace( cull.m_Face );
		SetFrontFace( cull.m_WindingOrder );

	}
	void GLDevice::bind_check_color_mask( const Pipeline& pip ) noexcept {

		const auto& mask = pip.m_ColorMask;

		SetColorMask( mask );

	}




	PipelineHandle GLDevice::CreatePipeline( const PipelineCreateInfo& desc ) {

		LUM_ASSERT( m_Pipelines.DenseSize( ) <= skMaxPipelines, "Max pipelines reached" );

		if (desc.m_Shader.m_Id != NullID<ShaderID>( ) && !IsValid( desc.m_Shader )) {
			LUM_LOG_ERROR( "Shader %d doesn't exist", desc.m_Shader.m_Id );
		}

		Pipeline pipeline;
		std::memcpy( &pipeline, &desc, sizeof( desc ) );

		return m_Pipelines.Append( std::move( pipeline ) );

	}
	void GLDevice::Delete( PipelineHandle& pipeline ) {

		LUM_RETURN_IF( !IsValid( pipeline ), LUM_SEV_WARN, "Invalid pipeline" );

		m_Pipelines.Remove( pipeline );

	}
	void GLDevice::BindPipeline( PipelineHandle pipeline ) {

		if (!IsValid( pipeline )) return;

		if (pipeline == m_CurrentPipeline) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}
		m_CurrentPipeline = pipeline;

		Pipeline& pip = m_Pipelines[ pipeline ];

		// Shader
		bind_check_shader( pip );

		// Color mask
		bind_check_color_mask( pip );

		// Topology
		bind_check_rasterizer( pip );

		// Depth & Stencil
		bind_check_depth_stencil( pip );

		// Scissors
		bind_check_scissors( pip );

		// Cull
		bind_check_cull( pip );

		// Blend
		bind_check_blend( pip );

	}

}