#include "Render/Postprocess/Antialiasing/Taa.hpp"

namespace lum::render {

	void TemporalAntiAliasing::Initialize( rhi::IRenderDevice& dev ) {

		m_RenderDev = dev;

	}

	void TemporalAntiAliasing::EnsureFrameTex( const rhi::TextureCreateInfo& desc ) {

		if (m_Width == desc.m_Width && m_Height == desc.m_Height && m_PreviousFrameTex != 0) {
			return;
		}

		if(m_PreviousFrameTex != 0)
			m_RenderDev( ).Delete( m_PreviousFrameTex );

		m_Width = desc.m_Width;
		m_Height = desc.m_Height;
		m_PreviousFrameTex = m_RenderDev( ).CreateTexture( desc );

	}

	Matrix4 TemporalAntiAliasing::ApplyJitter( const Matrix4& projection ) {
		
		if (m_Width == 0 || m_Height == 0) {
			m_CurrentJitter = Vector2( 0.0f, 0.0f );
			return projection;
		}

		static const Vector2 skOffsets[ ] = {
			{ 0.500000f,  0.333333f },
			{ 0.250000f,  0.666667f },
			{ 0.750000f,  0.111111f },
			{ 0.125000f,  0.444444f },
			{ 0.625000f,  0.777778f },
			{ 0.375000f,  0.222222f },
			{ 0.875000f,  0.555556f },
			{ 0.125000f,  0.888889f },
		};

		uint32 index = m_FrameIndex % 8;
		Vector2 rawJitter = skOffsets[ index ] - Vector2( 0.5f, 0.5f );
		m_CurrentJitter = rawJitter / Vector2( SafeCast<float32>( m_Width ), SafeCast<float32>( m_Height ) );

		Matrix4 jittered = projection;
		jittered[ 3 ][ 0 ] += m_CurrentJitter.m_X * 2.0f;
		jittered[ 3 ][ 1 ] += m_CurrentJitter.m_Y * 2.0f;

		m_FrameIndex++;

		return jittered;

	}


}