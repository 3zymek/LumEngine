//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL samplers operations creation, binding
//
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	SamplerHandle GLDevice::CreateSampler( const SamplerCreateInfo& desc ) {

		LUM_ASSERT( m_Samplers.DenseSize( ) <= sk_MaxSamplers, "Max samplers reached" );

		Sampler sampler;

		glCreateSamplers( 1, &sampler.m_Handle );
		glSamplerParameteri( sampler.m_Handle, GL_TEXTURE_MAG_FILTER, (desc.m_MagFilter == SamplerMagFilter::Nearest) ? GL_NEAREST : GL_LINEAR );
		glSamplerParameteri( sampler.m_Handle, GL_TEXTURE_MIN_FILTER, sk_TextureMinFilterLookup[ LookupCast( desc.m_MinFilter ) ] );

		glSamplerParameteri( sampler.m_Handle, GL_TEXTURE_WRAP_S, sk_SamplerWrapLookup[ LookupCast( desc.m_WrapS ) ] );
		glSamplerParameteri( sampler.m_Handle, GL_TEXTURE_WRAP_T, sk_SamplerWrapLookup[ LookupCast( desc.m_WrapT ) ] );


		GLfloat maxAnisotropy = 1.0f;
		glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy );

		GLfloat finalAnisotropy = std::clamp( ( float32 ) desc.m_Anisotropy, 1.0f, ( float32 ) maxAnisotropy );
		glSamplerParameterf( sampler.m_Handle, GL_TEXTURE_MAX_ANISOTROPY, finalAnisotropy );

		SamplerHandle samplerHandle = m_Samplers.Append( std::move( sampler ) );

		return samplerHandle;
	}

	void GLDevice::BindSampler( SamplerHandle sampler, uint16 binding ) {

		LUM_ASSERT( binding < sk_MaxSamplerUnits, "Sampler binding out of range" );
		if (!IsValid( sampler )) return;

		if (m_CurrentSamplers[ binding ] == sampler) {
			LUM_PROFILER_CACHE_HIT( );
			return;
		}

		m_CurrentSamplers[ binding ] = sampler;

		glBindSampler( binding, m_Samplers[ sampler ].m_Handle );

		LUM_PROFILER_CACHE_MISS( );

	}

	void GLDevice::Delete( SamplerHandle sampler ) {

		LUM_RETURN_IF( !IsValid( sampler ), LUM_SEV_WARN, "Invalid sampler" );

		glDeleteSamplers( 1, &m_Samplers[ sampler ].m_Handle );
		m_Samplers.Remove( sampler );

	}

}