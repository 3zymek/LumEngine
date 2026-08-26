//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL buffer management (VBO, EBO, UBO, SSBO)
//          Creation, mapping, updating, and binding operations
//
//=============================================================================//

#include "Modules/Rhi/Backend/GlDevice.hpp"

namespace lum::rhi::gl {

	BufferHandle GLDevice::CreateBuffer( const BufferCreateInfo& desc ) {

		LUM_ASSERT( validate_buffer_descriptor( desc ), "Invalid buffer descriptor" );

		Buffer buffer;

		buffer.m_Size = desc.m_Size;
		buffer.m_Type = desc.m_BufferType;
		buffer.m_Flags = desc.m_MapFlags;
		buffer.m_Usage = desc.m_BufferUsage;

		GLbitfield initFlags =
			((buffer.m_Usage == BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags( buffer.m_Flags );

		glCreateBuffers( 1, &buffer.m_Handle );

		glNamedBufferStorage(
			buffer.m_Handle,
			buffer.m_Size,
			desc.m_Data,
			initFlags
		);

		return m_Buffers.Append( std::move( buffer ) );

	}

	void GLDevice::UpdateBuffer( BufferHandle buff, const void* data, usize offset, usize size ) {

		LUM_ASSERT( IsValid( buff ), "Invalid buffer" );

		Buffer& buffer = m_Buffers[ buff ];

		if (size == 0) size = buffer.m_Size;

		LUM_ASSERT( offset + size <= buffer.m_Size, "Invalid offset or size" );
		LUM_ASSERT( buffer.m_Usage != BufferUsage::Static, "Buffer %d is static, cannot be updated" );
		LUM_ASSERT( buffer.m_Flags.Has( MapFlag::Write ), "Buffer %d has no write flags enabled" );

		glNamedBufferSubData( buffer.m_Handle, offset, size, data );

	}

	void GLDevice::Delete( BufferHandle& buff ) {

		LUM_RETURN_IF( !IsValid( buff ), LUM_SEV_DEBUG, "Invalid buffer" );

		Buffer& buffer = m_Buffers[ buff ];
		UnmapBuffer( buff );

		glDeleteBuffers( 1, &buffer.m_Handle );

		m_Buffers.Remove( buff );

	}

	void* GLDevice::MapBuffer( BufferHandle buff, Flags<MapFlag> flags, usize offset, usize size ) {

		LUM_RETURN_DEF_IF( !IsValid( buff ), LUM_SEV_WARN, "Invalid buffer" );

		Buffer& buffer = m_Buffers[ buff ];

		LUM_ASSERT( offset + size <= buffer.m_Size || size < buffer.m_Size, "Invalid offset or size" );
		if (size == 0) size = buffer.m_Size;

		void* ptr = glMapNamedBufferRange( buffer.m_Handle, offset, size, translate_mapping_flags( flags ) );

		LUM_ASSERT( ptr, "Failed to map buffer" );
		buffer.m_Mapped = true;

		return ptr;
	}

	void GLDevice::UnmapBuffer( BufferHandle buff ) {

		LUM_RETURN_IF( !IsValid( buff ), LUM_SEV_WARN, "Invalid buffer" );

		Buffer& buffer = m_Buffers[ buff ];
		if (!buffer.m_Mapped) return;

		glUnmapNamedBuffer( buffer.m_Handle );

	}

	void GLDevice::SetShaderStorageBinding( BufferHandle ssbo, uint32 binding ) {

		LUM_ASSERT( IsValid( ssbo ), "Invalid buffer" );

		const auto& buffer = m_Buffers[ ssbo ];

		glBindBufferBase( GL_SHADER_STORAGE_BUFFER, binding, buffer.m_Handle );

	}

	void GLDevice::AttachElementBufferToLayout( BufferHandle ebo, VertexLayoutHandle vao ) {

		LUM_ASSERT( m_Layouts.Contains( vao ), "Invalid layout" );
		LUM_ASSERT( m_Buffers.Contains( ebo ), "Invalid buffer" );

		glVertexArrayElementBuffer( m_Layouts[ vao ].m_Handle, m_Buffers[ ebo ].m_Handle );
		m_Layouts[ vao ].m_ElementBuff = ebo;

	}

	void GLDevice::SetUniformBufferBinding( BufferHandle ubo, int32 binding ) {

		LUM_ASSERT( IsValid( ubo ), "Invalid buffer" );

		glBindBufferBase( GL_UNIFORM_BUFFER, binding, m_Buffers[ ubo ].m_Handle );

	}

	GLbitfield GLDevice::translate_mapping_flags( Flags<MapFlag> flags ) noexcept {
		GLbitfield flag = 0;

		if (flags.Has( MapFlag::None ))						return 0;
		if (flags.Has( MapFlag::Persistent ))					flag |= GL_MAP_PERSISTENT_BIT;
		if (flags.Has( MapFlag::Write ))						flag |= GL_MAP_WRITE_BIT;
		if (flags.Has( MapFlag::Read ))						flag |= GL_MAP_READ_BIT;
		if (flags.Has( MapFlag::Coherent ))					flag |= GL_MAP_COHERENT_BIT;
		if (flags.Has( MapFlag::Invalidate_Buffer ))			flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags.Has( MapFlag::Invalidate_Range ))			flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags.Has( MapFlag::Unsynchronized ))				flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}


}