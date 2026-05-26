//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: OpenGL buffer management (VBO, EBO, UBO, SSBO)
//          Creation, mapping, updating, and binding operations
//
//=============================================================================//

#include "modules/rhi/backend/gl_device.hpp"

namespace lum::rhi::gl {

	BufferHandle GLDevice::CreateBuffer( const BufferCreateInfo& desc ) {

		LUM_ASSERT( validate_buffer_descriptor( desc ), "Invalid buffer descriptor" );

		Buffer buffer;

		buffer.mSize = desc.mSize;
		buffer.mType = desc.mBufferType;
		buffer.mFlags = desc.mMapFlags;
		buffer.mUsage = desc.mBufferUsage;

		GLbitfield initFlags =
			((buffer.mUsage == BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags( buffer.mFlags );

		glCreateBuffers( 1, &buffer.mHandle );

		glNamedBufferStorage(
			buffer.mHandle,
			buffer.mSize,
			desc.mData,
			initFlags
		);

		return mBuffers.Append( std::move( buffer ) );

	}

	void GLDevice::UpdateBuffer( BufferHandle buff, cvptr data, usize offset, usize size ) {

		LUM_ASSERT( IsValid( buff ), "Invalid buffer" );

		Buffer& buffer = mBuffers[ buff ];

		if (size == 0) size = buffer.mSize;

		LUM_ASSERT( offset + size <= buffer.mSize, "Invalid offset or size" );
		LUM_ASSERT( buffer.mUsage != BufferUsage::Static, "Buffer %d is static, cannot be updated" );
		LUM_ASSERT( buffer.mFlags.Has( MapFlag::Write ), "Buffer %d has no write flags enabled" );

		glNamedBufferSubData( buffer.mHandle, offset, size, data );

	}

	void GLDevice::Delete( BufferHandle& buff ) {

		LUM_RETURN_IF( !IsValid( buff ), LUM_SEV_DEBUG, "Invalid buffer" );

		Buffer& buffer = mBuffers[ buff ];
		UnmapBuffer( buff );

		glDeleteBuffers( 1, &buffer.mHandle );

		mBuffers.Remove( buff );

	}

	vptr GLDevice::MapBuffer( BufferHandle buff, Flags<MapFlag> flags, usize offset, usize size ) {

		LUM_RETURN_DEF_IF( !IsValid( buff ), LUM_SEV_WARN, "Invalid buffer" );

		Buffer& buffer = mBuffers[ buff ];

		LUM_ASSERT( offset + size <= buffer.mSize || size < buffer.mSize, "Invalid offset or size" );
		if (size == 0) size = buffer.mSize;

		vptr ptr = glMapNamedBufferRange( buffer.mHandle, offset, size, translate_mapping_flags( flags ) );

		LUM_ASSERT( ptr, "Failed to map buffer" );
		buffer.mMapped = true;

		return ptr;
	}

	void GLDevice::UnmapBuffer( BufferHandle buff ) {

		LUM_RETURN_IF( !IsValid( buff ), LUM_SEV_WARN, "Invalid buffer" );

		Buffer& buffer = mBuffers[ buff ];
		if (!buffer.mMapped) return;

		glUnmapNamedBuffer( buffer.mHandle );

	}

	void GLDevice::SetShaderStorageBinding( BufferHandle ssbo, uint32 binding ) {

		LUM_ASSERT( IsValid( ssbo ), "Invalid buffer" );

		const auto& buffer = mBuffers[ ssbo ];

		glBindBufferBase( GL_SHADER_STORAGE_BUFFER, binding, buffer.mHandle );

	}

	void GLDevice::AttachElementBufferToLayout( BufferHandle ebo, VertexLayoutHandle vao ) {

		LUM_ASSERT( mLayouts.Contains( vao ), "Invalid layout" );
		LUM_ASSERT( mBuffers.Contains( ebo ), "Invalid buffer" );

		glVertexArrayElementBuffer( mLayouts[ vao ].mHandle, mBuffers[ ebo ].mHandle );
		mLayouts[ vao ].mElementBuff = ebo;

	}

	void GLDevice::SetUniformBufferBinding( BufferHandle ubo, int32 binding ) {

		LUM_ASSERT( IsValid( ubo ), "Invalid buffer" );

		glBindBufferBase( GL_UNIFORM_BUFFER, binding, mBuffers[ ubo ].mHandle );

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