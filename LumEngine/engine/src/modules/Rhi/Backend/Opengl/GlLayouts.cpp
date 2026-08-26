//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL vertex layout (VAO) creation and deletion.
//
//=============================================================================//

#include "Rhi/Backend/GlDevice.hpp"
#include "Rhi/RhiCommon.hpp"

namespace lum::rhi::gl {

	VertexLayoutHandle GLDevice::CreateVertexLayout( const VertexLayoutCreateInfo& desc, BufferHandle vbo ) {

		LUM_ASSERT( IsValid( vbo ), "Invalid buffer" );
		LUM_ASSERT( desc.m_Attributes.size( ) > 0, "Vertex layout has no attributes" );

		VertexLayout layout;
		Buffer& buffer = m_Buffers[ vbo ];

		glCreateVertexArrays( 1, &layout.m_Handle );
		glVertexArrayVertexBuffer(
			layout.m_Handle,
			desc.m_Binding,
			buffer.m_Handle,
			desc.m_Offset,
			desc.m_Stride
		);

		for (int32 i = 0; i < desc.m_Attributes.size( ); i++) {

			glVertexArrayAttribFormat(
				layout.m_Handle,
				desc.m_Attributes[ i ].m_ShaderLocation,
				detail::kDataFormatLookup[ LookupCast( desc.m_Attributes[ i ].m_Format ) ],
				GL_FLOAT,
				GL_FALSE,
				desc.m_Attributes[ i ].m_RelativeOffset
			);

			glVertexArrayAttribBinding(
				layout.m_Handle,
				desc.m_Attributes[ i ].m_ShaderLocation,
				desc.m_Binding
			);

			glEnableVertexArrayAttrib( layout.m_Handle, desc.m_Attributes[ i ].m_ShaderLocation );

		}

		return m_Layouts.Append( std::move( layout ) );

	}
	void GLDevice::Delete( VertexLayoutHandle& layout ) {

		LUM_RETURN_IF( !IsValid( layout ), LUM_SEV_WARN, "Invalid layout" );
		glDeleteVertexArrays( 1, &m_Layouts[ layout ].m_Handle );
		m_Layouts.Remove( layout );

	}
}