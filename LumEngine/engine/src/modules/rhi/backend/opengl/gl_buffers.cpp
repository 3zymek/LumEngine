//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL buffer management (VBO, EBO, UBO, SSBO)
//          Creation, mapping, updating, and binding operations
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RBufferHandle GLDevice::CreateBuffer( const FBufferDescriptor& desc ) {

		LUM_ASSERT(validate_buffer_descriptor(desc), "Invalid buffer descriptor");

		FBuffer buffer;

		buffer.mSize = desc.mSize;
		buffer.mType = desc.mBufferType;
		buffer.mFlags = desc.mMapFlags;
		buffer.mUsage = desc.mBufferUsage;

		GLbitfield initFlags =
			((buffer.mUsage == BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags(buffer.mFlags);

		glCreateBuffers(1, &buffer.mHandle);

		glNamedBufferStorage(
			buffer.mHandle,
			buffer.mSize,
			desc.mData,
			initFlags
		);

		RBufferHandle createdBuffer = mBuffers.Append(std::move(buffer));

		LUM_LOG_INFO("Created buffer %d", createdBuffer.mID);

		return createdBuffer;

	}

	void GLDevice::UpdateBuffer(const RBufferHandle& vbo, cvptr data, usize offset, usize size) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.Contains(vbo), LUM_SEV_DEBUG, "Buffer does not exist");

		FBuffer& buffer = mBuffers[vbo];

		if (size == 0) size = buffer.mSize;

		LUM_HOTCHK_RETURN_VOID(offset + size <= buffer.mSize, LUM_SEV_WARN, "Invalid offset or size");

		LUM_HOTCHK_RETURN_VOID(
			buffer.mUsage != BufferUsage::Static,
			LUM_SEV_WARN,
			"Buffer %d is static, cannot be updated",
			vbo.mID
		);

		LUM_HOTCHK_RETURN_VOID(
			buffer.mFlags.Has(MapFlag::Write),
			LUM_SEV_WARN,
			"Buffer %d has no write flags enabled",
			vbo.mID
		);

		vptr ptr =
			glMapNamedBufferRange(
				buffer.mHandle,
				offset,
				size,
				GL_MAP_WRITE_BIT
			);

		LUM_HOTCHK_RETURN_VOID(ptr != nullptr, LUM_SEV_WARN, "Failed during buffer mapping");

		std::memcpy(ptr, data, size);

		glUnmapNamedBuffer(buffer.mHandle);

		LUM_LOG_DEBUG("Updated buffer %d", vbo.mID);
	}

	void GLDevice::DeleteBuffer(RBufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.Contains(vbo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		FBuffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.bMapped, LUM_SEV_WARN, "Unable to delete buffer - still mapped");

		glDeleteBuffers(1, &buffer.mHandle);
		
		mBuffers.Remove(vbo);

		LUM_LOG_INFO("Deleted buffer %d", vbo.mID);
	}

	vptr GLDevice::MapBuffer(const RBufferHandle& vbo, Flags<MapFlag> flags, usize offset, usize size) {

		LUM_HOTCHK_RETURN_NPTR(mBuffers.Contains(vbo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		FBuffer& buffer = mBuffers[vbo];

		LUM_HOTCHK_RETURN_NPTR(offset + size < buffer.mSize || size < buffer.mSize, LUM_SEV_WARN, "Invalid offset or size");
		if (size <= 0) size = buffer.mSize;

		vptr ptr = glMapNamedBufferRange(buffer.mHandle, offset, size, translate_mapping_flags(flags));

		LUM_ASSERT(ptr, "Failed to map buffer");

		LUM_LOG_DEBUG("Mapped buffer %d", vbo.mID);
		return ptr;
	}

	void GLDevice::UnmapBuffer(const RBufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.Contains(vbo), LUM_SEV_WARN, "Buffer does not exist");

		FBuffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.bMapped, LUM_SEV_WARN, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.mHandle);

		LUM_LOG_DEBUG("Unmapped buffer %d", vbo.mID);
	}

	void GLDevice::SetShaderStorageBinding(const RBufferHandle& ssbo, uint32 binding) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.Contains(ssbo), LUM_SEV_WARN, "Buffer does not exist");

		const auto& buffer = mBuffers[ssbo];

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer.mHandle);

	}

	void GLDevice::AttachElementBufferToLayout(const RBufferHandle& ebo, const RVertexLayoutHandle& vao) {

		LUM_HOTCHK_RETURN_VOID(mLayouts.Contains(vao), LUM_SEV_DEBUG, "Layout doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mBuffers.Contains(ebo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		glVertexArrayElementBuffer(mLayouts[vao].mHandle, mBuffers[ebo].mHandle);
		mLayouts[vao].mElementBuff = ebo;

		

		LUM_LOG_DEBUG("Attached EBO %d to VAO %d", ebo.mID, vao.mID);

	}

	void GLDevice::SetUniformBufferBinding(const RBufferHandle& ubo, int32 binding) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.Contains(ubo), LUM_SEV_DEBUG, "Uniform buffer doesn't exist");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mBuffers[ubo].mHandle);

		LUM_LOG_DEBUG("Attached UBO %d to binding %d", ubo.mID, binding);

	}


}