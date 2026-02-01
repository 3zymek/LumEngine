#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	///////////////////////////////////////////////////
	/// Buffers
	///////////////////////////////////////////////////

	BufferHandle GLDevice::CreateVertexBuffer(const BufferDescriptor& desc) {

		if (!_IsValidBufferDescriptor(desc))
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
			| _TranslateMappingFlags(desc.mapFlags);

		glCreateBuffers(1, &buffer.handle.glHandle);
		glNamedBufferStorage(
			buffer.handle.glHandle,
			desc.size,
			desc.data,
			init_flags
		);

		LUM_LOG_INFO(std::format("Created vertex buffer {}", buffer.handle.glHandle));
		return mBuffers.create_handle(std::move(buffer));
	}

	BufferHandle GLDevice::CreateElementBuffer(const BufferDescriptor& desc) {

		if (!_IsValidBufferDescriptor(desc))
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
			| _TranslateMappingFlags(desc.mapFlags);

		glCreateBuffers(1, &buffer.handle.glHandle);
		glNamedBufferStorage(
			buffer.handle.glHandle,
			desc.size,
			desc.data,
			init_flags
		);

		LUM_LOG_INFO(std::format("Created element buffer {}", buffer.handle.glHandle));
		return mBuffers.create_handle(std::move(buffer));
	}

	BufferHandle GLDevice::CreateUniformBuffer(const BufferDescriptor& desc) {
		if (!_IsValidBufferDescriptor(desc))
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
		buffer.type = BufferType::Uniform;
		buffer.usage = desc.bufferUsage;

		GLbitfield init_flags =
			((buffer.usage == BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| _TranslateMappingFlags(desc.mapFlags);

		glCreateBuffers(1, &buffer.handle.glHandle);
		glNamedBufferStorage(
			buffer.handle.glHandle,
			desc.size,
			desc.data,
			init_flags
		);

		auto createdBuffer = mBuffers.create_handle(std::move(buffer));

		LUM_LOG_INFO(std::format("Created uniform buffer {}", createdBuffer.id));

		return createdBuffer;

	}

	void GLDevice::UpdateBuffer(const BufferHandle& vbo, cvptr data, usize offset, usize size) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), "Buffer does not exist");

		Buffer& buffer = mBuffers[vbo];

		LUM_HOTCHK_RETURN_VOID(offset + size > buffer.size, "Invalid offset or size");

		if (size == 0) size = buffer.size;

		LUM_HOTCHK_RETURN_VOID(
			buffer.usage != BufferUsage::Static,
			std::format("Buffer {} is static, cannot be updated", buffer.handle.glHandle)
		);

		LUM_HOTCHK_RETURN_VOID(
			(buffer.flags & Mapflag::Write),
			std::format("Buffer {} has no write flags enabled", buffer.handle.glHandle)
		);

		void* ptr =
			glMapNamedBufferRange(
				buffer.handle.glHandle,
				offset,
				size,
				GL_MAP_WRITE_BIT
			);

		LUM_HOTCHK_RETURN_VOID(ptr, "Failed during buffer mapping");

		std::memcpy(ptr, data, buffer.size);

		glUnmapNamedBuffer(buffer.handle.glHandle);

		LUM_LOG_DEBUG(std::format("Updated buffer {}", buffer.handle.glHandle));
	}

	void GLDevice::DeleteBuffer(BufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), "Buffer doesn't exist");

		Buffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.mapped, "Unable to delete buffer - still mapped");

		glDeleteBuffers(1, &buffer.handle.glHandle);
		
		mBuffers.delete_handle(vbo);

		LUM_LOG_INFO(std::format("Deleted buffer {}", buffer.handle.glHandle));
	}

	vptr GLDevice::MapBuffer(const BufferHandle& vbo, Mapflag flags, usize offset, usize size) {

		LUM_HOTCHK_RETURN_NPTR(mBuffers.exist(vbo), "Handle doesn't exist");

		Buffer& buffer = mBuffers[vbo];

		LUM_HOTCHK_RETURN_NPTR(offset + size < buffer.size || size < buffer.size, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		vptr ptr = glMapNamedBufferRange(buffer.handle.glHandle, offset, size, _TranslateMappingFlags(flags));

		LUM_ASSERT(ptr, "Failed to map buffer");

		LUM_LOG_DEBUG(std::format("Mapped buffer {}", buffer.handle.glHandle));
		return ptr;
	}

	void GLDevice::UnmapBuffer(const BufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), "Handle does not exist");

		Buffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.mapped, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.glHandle);

		LUM_LOG_DEBUG(std::format("Unmapped buffer {}", buffer.handle.glHandle));
	}

	void GLDevice::AttachElementBufferToLayout(const BufferHandle& ebo, const VertexLayoutHandle& vao) {
		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), "Layout doesn't exists");
		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ebo), "Buffer doesn't exists");

		glVertexArrayElementBuffer(mLayouts[vao].vao, mBuffers[ebo].handle.glHandle);

	}

	void GLDevice::SetUniformBufferBinding(const BufferHandle& ubo, int32 binding) {
		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ubo), "Uniform buffer doesn't exists");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mBuffers[ubo].handle.glHandle);

		LUM_LOG_INFO(std::format("Attached ubo {} to binding {}", ubo.id, binding));

	}
}