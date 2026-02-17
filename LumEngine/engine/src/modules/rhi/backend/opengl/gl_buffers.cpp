//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL buffer management (VBO, EBO, UBO, SSBO)
//          Creation, mapping, updating, and binding operations
//
//=============================================================================//

#include "modules/rhi/backend/opengl/gl_device.hpp"

namespace lum::rhi::gl {

	RBufferHandle GLDevice::CreateVertexBuffer(const RBufferDescriptor& desc) {

		if (!is_valid_buffer_descriptor(desc))
			return RBufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return RBufferHandle{};
		}

		if (desc.mSize <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return RBufferHandle{};
		}

		RBuffer buffer;
		buffer.mSize = desc.mSize;
		buffer.mFlags = desc.mMapFlags;
		buffer.mType = RBufferType::Vertex;
		buffer.mUsage = desc.mBufferUsage;

		GLbitfield init_flags =
			((desc.mBufferUsage == RBufferUsage::Dynamic) ? GL_DYNAMIC_STORAGE_BIT : 0)
			| translate_mapping_flags(desc.mMapFlags);

		glCreateBuffers(1, &buffer.mHandle.gl);
		glNamedBufferStorage(
			buffer.mHandle.gl,
			desc.mSize,
			desc.mData,
			init_flags
		);

		auto createdBuffer = mBuffers.create_handle(std::move(buffer));

		LUM_LOG_INFO("Created vertex buffer %d", createdBuffer.id);

		return createdBuffer;
	}

	RBufferHandle GLDevice::CreateElementBuffer(const RBufferDescriptor& desc) {

		if (!is_valid_buffer_descriptor(desc))
			return RBufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return RBufferHandle{};
		}

		if (desc.mSize <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return RBufferHandle{};
		}

		RBuffer buffer;
		buffer.mSize = desc.mSize;
		buffer.mFlags = desc.mMapFlags;
		buffer.mType = RBufferType::Element;
		buffer.mUsage = desc.mBufferUsage;

		GLbitfield init_flags =
			((buffer.mUsage == RBufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags(desc.mMapFlags);

		glCreateBuffers(1, &buffer.mHandle.gl);
		glNamedBufferStorage(
			buffer.mHandle.gl,
			desc.mSize,
			desc.mData,
			init_flags
		);


		auto createdBuffer = mBuffers.create_handle(std::move(buffer));

		LUM_LOG_INFO("Created element buffer %d", createdBuffer.id);
		
		return createdBuffer;
	}

	RBufferHandle GLDevice::CreateUniformBuffer(const RBufferDescriptor& desc) {
		if (!is_valid_buffer_descriptor(desc))
			return RBufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return RBufferHandle{};
		}

		if (desc.mSize <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return RBufferHandle{};
		}

		RBuffer ubo;
		ubo.mSize = desc.mSize;
		ubo.mFlags = desc.mMapFlags;
		ubo.mType = RBufferType::Uniform;
		ubo.mUsage = desc.mBufferUsage;

		GLbitfield initFlags =
			((ubo.mUsage == RBufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags(desc.mMapFlags);

		glCreateBuffers(1, &ubo.mHandle.gl);
		glNamedBufferStorage(
			ubo.mHandle.gl,
			desc.mSize,
			desc.mData,
			initFlags
		);

		auto createdBuffer = mBuffers.create_handle(std::move(ubo));

		LUM_LOG_INFO("Created uniform buffer %d", createdBuffer.id);

		return createdBuffer;

	}

	RBufferHandle GLDevice::CreateShaderStorageBuffer(const RBufferDescriptor& desc) {

		if (!is_valid_buffer_descriptor(desc))
			return RBufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return RBufferHandle{};
		}

		if (desc.mSize <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return RBufferHandle{};
		}


		RBuffer ssbo;
		glCreateBuffers(1, &ssbo.mHandle.gl);

		ssbo.mSize = desc.mSize;
		ssbo.mFlags = desc.mMapFlags;
		ssbo.mType = RBufferType::ShaderStorage;
		ssbo.mUsage = desc.mBufferUsage;

		GLbitfield initFlags =
			((ssbo.mUsage == RBufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| translate_mapping_flags(desc.mMapFlags);

		glNamedBufferStorage(
			ssbo.mHandle.gl,
			ssbo.mSize,
			desc.mData,
			initFlags
		);

		auto createdBuffer = mBuffers.create_handle(std::move(ssbo));

		LUM_LOG_INFO("Created shader storage buffer %d", createdBuffer.id);

		return createdBuffer;

	}

	void GLDevice::UpdateBuffer(const RBufferHandle& vbo, cvptr data, usize offset, usize size) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), LUM_SEV_DEBUG, "Buffer does not exist");

		RBuffer& buffer = mBuffers[vbo];

		LUM_HOTCHK_RETURN_VOID(offset + size < buffer.mSize, LUM_SEV_WARN, "Invalid offset or size");

		if (size == 0) size = buffer.mSize;

		LUM_HOTCHK_RETURN_VOID(
			buffer.mUsage != RBufferUsage::Static,
			LUM_SEV_WARN,
			"Buffer %d is static, cannot be updated",
			vbo.id
		);

		LUM_HOTCHK_RETURN_VOID(
			buffer.mFlags.Has(RMapflag::Write),
			LUM_SEV_WARN,
			"Buffer %d has no write flags enabled",
			vbo.id
		);

		void* ptr =
			glMapNamedBufferRange(
				buffer.mHandle.gl,
				offset,
				size,
				GL_MAP_WRITE_BIT
			);

		LUM_HOTCHK_RETURN_VOID(ptr, LUM_SEV_WARN, "Failed during buffer mapping");

		std::memcpy(ptr, data, buffer.mSize);

		glUnmapNamedBuffer(buffer.mHandle.gl);

		LUM_LOG_DEBUG("Updated buffer %d", vbo.id);
	}

	void GLDevice::DeleteBuffer(RBufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		RBuffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.bMapped, LUM_SEV_WARN, "Unable to delete buffer - still mapped");

		glDeleteBuffers(1, &buffer.mHandle.gl);
		
		mBuffers.delete_handle(vbo);

		LUM_LOG_INFO("Deleted buffer %d", vbo.id);
	}

	vptr GLDevice::MapBuffer(const RBufferHandle& vbo, Flags<RMapflag> flags, usize offset, usize size) {

		LUM_HOTCHK_RETURN_NPTR(mBuffers.exist(vbo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		RBuffer& buffer = mBuffers[vbo];

		LUM_HOTCHK_RETURN_NPTR(offset + size < buffer.mSize || size < buffer.mSize, LUM_SEV_WARN, "Invalid offset or size");
		if (size <= 0) size = buffer.mSize;

		vptr ptr = glMapNamedBufferRange(buffer.mHandle.gl, offset, size, translate_mapping_flags(flags));

		LUM_ASSERT(ptr, "Failed to map buffer");

		LUM_LOG_DEBUG("Mapped buffer %d", vbo.id);
		return ptr;
	}

	void GLDevice::UnmapBuffer(const RBufferHandle& vbo) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(vbo), LUM_SEV_WARN, "Buffer does not exist");

		RBuffer& buffer = mBuffers[vbo];
		LUM_HOTCHK_RETURN_VOID(buffer.bMapped, LUM_SEV_WARN, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.mHandle.gl);

		LUM_LOG_DEBUG("Unmapped buffer %d", vbo.id);
	}

	void GLDevice::SetShaderStorageBinding(const RBufferHandle& ssbo, uint32 binding) {

		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ssbo), LUM_SEV_WARN, "Buffer does not exist");

		const auto& buffer = mBuffers[ssbo];

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer.mHandle.gl);

	}

	void GLDevice::AttachElementBufferToLayout(const RBufferHandle& ebo, const RVertexLayoutHandle& vao) {

		LUM_HOTCHK_RETURN_VOID(mLayouts.exist(vao), LUM_SEV_DEBUG, "Layout doesn't exist");
		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ebo), LUM_SEV_DEBUG, "Buffer doesn't exist");

		glVertexArrayElementBuffer(mLayouts[vao].mHandle, mBuffers[ebo].mHandle.gl);

		LUM_LOG_DEBUG("Attached EBO %d to VAO %d", ebo.id, vao.id);

	}

	void GLDevice::SetUniformBufferBinding(const RBufferHandle& ubo, int32 binding) {
		LUM_HOTCHK_RETURN_VOID(mBuffers.exist(ubo), LUM_SEV_DEBUG, "Uniform buffer doesn't exist");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mBuffers[ubo].mHandle.gl);

		LUM_LOG_DEBUG("Attached UBO %d to binding %d", ubo.id, binding);

	}
}