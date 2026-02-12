#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	///////////////////////////////////////////////////
	/// Buffers
	///////////////////////////////////////////////////

	BufferHandle GLDevice::CreateVertexBuffer(const BufferDescriptor& desc) {

		if (!is_valid_buffer_descriptor(desc))
			return BufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return BufferHandle{};
		}

		Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.mapFlags;
		buffer.type = BufferType::Vertex;
		buffer.usage = desc.bufferUsage;

		GLbitfield init_flags =
			((desc.bufferUsage == BufferUsage::Dynamic) ? GL_DYNAMIC_STORAGE_BIT : 0)
			| translate_mapping_flags(desc.mapFlags);

		glCreateBuffers(1, &buffer.handle.glHandle);
		glNamedBufferStorage(
			buffer.handle.glHandle,
			desc.size,
			desc.data,
			init_flags
		);

		auto createdBuffer = mBuffers.create_handle(std::move(buffer));

		LUM_LOG_INFO("Created vertex buffer %d", createdBuffer.id);

		return createdBuffer;
	}

	BufferHandle GLDevice::CreateElementBuffer(const BufferDescriptor& desc) {

		if (!is_valid_buffer_descriptor(desc))
			return BufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return BufferHandle{};
		}

		Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.mapFlags;
		buffer.type = BufferType::Element;
		buffer.usage = desc.bufferUsage;

		GLbitfield init_flags =
			((buffer.usage == BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags(desc.mapFlags);

		glCreateBuffers(1, &buffer.handle.glHandle);
		glNamedBufferStorage(
			buffer.handle.glHandle,
			desc.size,
			desc.data,
			init_flags
		);


		auto createdBuffer = mBuffers.create_handle(std::move(buffer));

		LUM_LOG_INFO("Created element buffer %d", createdBuffer.id);
		
		return createdBuffer;
	}

	BufferHandle GLDevice::CreateUniformBuffer(const BufferDescriptor& desc) {
		if (!is_valid_buffer_descriptor(desc))
			return BufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return BufferHandle{};
		}

		Buffer ubo;
		ubo.size = desc.size;
		ubo.flags = desc.mapFlags;
		ubo.type = BufferType::Uniform;
		ubo.usage = desc.bufferUsage;

		GLbitfield initFlags =
			((ubo.usage == BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags(desc.mapFlags);

		glCreateBuffers(1, &ubo.handle.glHandle);
		glNamedBufferStorage(
			ubo.handle.glHandle,
			desc.size,
			desc.data,
			initFlags
		);

		auto createdBuffer = mBuffers.create_handle(std::move(ubo));

		LUM_LOG_INFO("Created uniform buffer %d", createdBuffer.id);

		return createdBuffer;

	}

	BufferHandle GLDevice::CreateShaderStorageBuffer(const BufferDescriptor& desc) {

		if (!is_valid_buffer_descriptor(desc))
			return BufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return BufferHandle{};
		}


		Buffer ssbo;
		glCreateBuffers(1, &ssbo.handle.glHandle);

		ssbo.size = desc.size;
		ssbo.flags = desc.mapFlags;
		ssbo.type = BufferType::ShaderStorage;
		ssbo.usage = desc.bufferUsage;

		GLbitfield initFlags =
			((ssbo.usage == BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags(desc.mapFlags);

		glNamedBufferStorage(
			ssbo.handle.glHandle,
			ssbo.size,
			desc.data,
			initFlags
		);

		auto createdBuffer = mBuffers.create_handle(std::move(ssbo));

		LUM_LOG_INFO("Created shader storage buffer %d", createdBuffer.id);

		return createdBuffer;

	}

	void GLDevice::UpdateBuffer(const BufferHandle& vbo, cvptr data, usize offset, usize size) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), LUM_SEV_DEBUG, "Buffer does not exist");

		Buffer& buffer = mBuffers[vbo];

		LUM_HOTCHK_RETURN_VOID(offset + size < buffer.size, LUM_SEV_WARN, "Invalid offset or size");

		if (size == 0) size = buffer.size;

		LUM_HOTCHK_RETURN_VOID(
			buffer.usage != BufferUsage::Static,
			LUM_SEV_WARN,
			"Buffer %d is static, cannot be updated",
			vbo.id
		);

		LUM_HOTCHK_RETURN_VOID(
			buffer.flags.has(Mapflag::Write),
			LUM_SEV_WARN,
			"Buffer %d has no write flags enabled",
			vbo.id
		);

		void* ptr =
			glMapNamedBufferRange(
				buffer.handle.glHandle,
				offset,
				size,
				GL_MAP_WRITE_BIT
			);

		LUM_HOTCHK_RETURN_VOID(ptr, LUM_SEV_WARN, "Failed during buffer mapping");

		std::memcpy(ptr, data, buffer.size);

		glUnmapNamedBuffer(buffer.handle.glHandle);

		LUM_LOG_DEBUG("Updated buffer %d", vbo.id);
	}

	void GLDevice::DeleteBuffer(BufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		Buffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.bMapped, LUM_SEV_WARN, "Unable to delete buffer - still mapped");

		glDeleteBuffers(1, &buffer.handle.glHandle);
		
		mBuffers.delete_handle(vbo);

		LUM_LOG_INFO("Deleted buffer %d", vbo.id);
	}

	vptr GLDevice::MapBuffer(const BufferHandle& vbo, Flags<Mapflag> flags, usize offset, usize size) {

		LUM_HOTCHK_RETURN_NPTR(mBuffers.exist(vbo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		Buffer& buffer = mBuffers[vbo];

		LUM_HOTCHK_RETURN_NPTR(offset + size < buffer.size || size < buffer.size, LUM_SEV_WARN, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		vptr ptr = glMapNamedBufferRange(buffer.handle.glHandle, offset, size, translate_mapping_flags(flags));

		LUM_ASSERT(ptr, "Failed to map buffer");

		LUM_LOG_DEBUG("Mapped buffer %d", vbo.id);
		return ptr;
	}

	void GLDevice::UnmapBuffer(const BufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), LUM_SEV_WARN, "Buffer does not exist");

		Buffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.bMapped, LUM_SEV_WARN, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.glHandle);

		LUM_LOG_DEBUG("Unmapped buffer %d", vbo.id);
	}

	void GLDevice::SetShaderStorageBinding(const BufferHandle& ssbo, uint32 binding) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ssbo), LUM_SEV_WARN, "Buffer does not exist");

		const auto& buffer = mBuffers[ssbo];

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer.handle.glHandle);

	}

	void GLDevice::AttachElementBufferToLayout(const BufferHandle& ebo, const VertexLayoutHandle& vao) {

		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), LUM_SEV_DEBUG, "Layout doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ebo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		glVertexArrayElementBuffer(mLayouts[vao].vao, mBuffers[ebo].handle.glHandle);

		LUM_LOG_DEBUG("Attached EBO %d to VAO %d", ebo.id, vao.id);

	}

	void GLDevice::SetUniformBufferBinding(const BufferHandle& ubo, int32 binding) {
		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ubo), LUM_SEV_DEBUG, "Uniform buffer doesn't exist");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mBuffers[ubo].handle.glHandle);

		LUM_LOG_DEBUG("Attached UBO %d to binding %d", ubo.id, binding);

	}
}