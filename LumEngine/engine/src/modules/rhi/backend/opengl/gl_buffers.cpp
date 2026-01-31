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

		LUM_LOG_INFO(std::format("Created uniform buffer {}", buffer.handle.glHandle));
		return mBuffers.create_handle(std::move(buffer));
	}

	void GLDevice::UpdateBuffer(const BufferHandle& vbo, cvptr data, usize offset, usize size) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(vbo), "Buffer does not exist");

		Buffer& buffer = mBuffers[vbo];

		LUM_HOTPATH_ASSERT_VOID(offset + size > buffer.size, "Invalid offset or size");
		if (size == 0) size = buffer.size;
		LUM_HOTPATH_ASSERT_VOID(
			buffer.usage == BufferUsage::Static,
			std::format("Buffer {} is static, cannot be updated", buffer.handle.glHandle)
		);
		LUM_HOTPATH_ASSERT_VOID(
			!(buffer.flags & Mapflag::Write),
			std::format("Buffer {} has no write flags enabled", buffer.handle.glHandle)
		);

		void* ptr =
			glMapNamedBufferRange(
				buffer.handle.glHandle,
				offset,
				size,
				GL_MAP_WRITE_BIT
			);

		LUM_HOTPATH_ASSERT_VOID(!ptr, "Error during mapping");

		std::memcpy(ptr, data, buffer.size);
		glUnmapNamedBuffer(buffer.handle.glHandle);

		LUM_LOG_DEBUG(std::format("Updated buffer {}", buffer.handle.glHandle));
	}

	void GLDevice::DeleteBuffer(BufferHandle& vbo) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(vbo), "Buffer doesn't exist");

		auto& buffer = mBuffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(buffer.mapped, "Unable to delete buffer - still mapped");
		glDeleteBuffers(1, &buffer.handle.glHandle);
		mBuffers.delete_handle(vbo);

		LUM_LOG_INFO(std::format("Deleted buffer {}", buffer.handle.glHandle));

	}

	vptr GLDevice::MapBuffer(const BufferHandle& vbo, Mapflag flags, usize offset, usize size) {

		LUM_HOTPATH_ASSERT_NULLPTR(!mBuffers.exists(vbo), "Handle doesn't exist");

		Buffer& buffer = mBuffers[vbo];

		LUM_HOTPATH_ASSERT_NULLPTR(offset + size > buffer.size || size > buffer.size, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		vptr ptr = glMapNamedBufferRange(buffer.handle.glHandle, offset, size, _TranslateMappingFlags(flags));

		LUM_HOTPATH_ASSERT_NULLPTR(!ptr, "Error during mapping");

		LUM_LOG_DEBUG(std::format("Mapped buffer {}", buffer.handle.glHandle));
		return ptr;
	}

	void GLDevice::UnmapBuffer(const BufferHandle& vbo) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(vbo), "Handle does not exist");

		Buffer& buffer = mBuffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(!buffer.mapped, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.glHandle);

		LUM_LOG_DEBUG(std::format("Unmapped buffer {}", buffer.handle.glHandle));
	}

	void GLDevice::AttachElementBufferToLayout(const BufferHandle& ebo, const VertexLayoutHandle& vao) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.exists(vao), "Layout doesn't exists");
		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(ebo), "Buffer doesn't exists");

		glVertexArrayElementBuffer(mLayouts[vao].vao, mBuffers[ebo].handle.glHandle);

	}

	void GLDevice::SetUniformBufferBinding(const BufferHandle& ubo, int32 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(ubo), "Uniform buffer doesn't exists");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mBuffers[ubo].handle.glHandle);

	}
}