#include "rhi/backend/opengl/gl_device.hpp"
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "window_context/window.hpp"
namespace lum::gl {

	///////////////////////////////////////////////////
	/// Buffers
	///////////////////////////////////////////////////

	rhi::BufferHandle		GL_Device::CreateVertexBuffer	( const rhi::BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (m_buffers.DenseSize() >= MAX_BUFFERS) {
			LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LOG_WARN("Invalid buffer size");
			return rhi::BufferHandle{};
		}

		rhi::Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.map_flags;
		buffer.type = rhi::BufferType::Vertex;
		buffer.usage = desc.buffer_usage;

		GLbitfield init_flags =
			((buffer.usage == rhi::BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| TranslateMappingFlags(desc.map_flags);

		glCreateBuffers(1, &buffer.handle.gl_handle);
		glNamedBufferStorage(
			buffer.handle.gl_handle,
			desc.size,
			desc.data,
			init_flags
		);

		LOG_DEBUG(std::format("Created buffer {}", buffer.handle.gl_handle));
		return m_buffers.CreateHandle(std::move(buffer));
	}
	rhi::BufferHandle		GL_Device::CreateElementBuffer	( const rhi::BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (m_buffers.DenseSize() >= MAX_BUFFERS) {
			LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LOG_WARN("Invalid buffer size");
			return rhi::BufferHandle{};
		}

		rhi::Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.map_flags;
		buffer.type = rhi::BufferType::Element;
		buffer.usage = desc.buffer_usage;

		GLbitfield init_flags =
			((buffer.usage == rhi::BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| TranslateMappingFlags(desc.map_flags);

		glCreateBuffers(1, &buffer.handle.gl_handle);
		glNamedBufferStorage(
			buffer.handle.gl_handle,
			desc.size,
			desc.data,
			init_flags
		);

		LOG_DEBUG(std::format("Created buffer {}", buffer.handle.gl_handle));
		return m_buffers.CreateHandle(std::move(buffer));
	}
	rhi::BufferHandle		GL_Device::CreateUniformBuffer	( const rhi::BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (m_buffers.DenseSize() >= MAX_BUFFERS) {
			LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LOG_WARN("Invalid buffer size");
			return rhi::BufferHandle{};
		}

		rhi::Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.map_flags;
		buffer.type = rhi::BufferType::Uniform;
		buffer.usage = desc.buffer_usage;

		GLbitfield init_flags =
			((buffer.usage == rhi::BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| TranslateMappingFlags(desc.map_flags);

		glCreateBuffers(1, &buffer.handle.gl_handle);
		glNamedBufferStorage(
			buffer.handle.gl_handle,
			desc.size,
			desc.data,
			init_flags
		);

		LOG_DEBUG(std::format("Created buffer {}", buffer.handle.gl_handle));
		return m_buffers.CreateHandle(std::move(buffer));
	}
	void					GL_Device::UpdateBuffer	( rhi::BufferHandle vbo, const void* data, size_t offset, size_t size ) {

		HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Buffer does not exist");

		rhi::Buffer& buffer = m_buffers[vbo];

		HOTPATH_ASSERT_VOID(offset + size > buffer.size, "Invalid offset or size");
		if (size == 0) size = buffer.size;
		HOTPATH_ASSERT_VOID(
			buffer.usage == rhi::BufferUsage::Static, 
			std::format("Buffer {} is static, cannot be updated", buffer.handle.gl_handle)
		);
		HOTPATH_ASSERT_VOID(
			!(buffer.flags & rhi::map_flags::Write), 
			std::format("Buffer {} has no write flags enabled", buffer.handle.gl_handle)
		);

		void* ptr =
			glMapNamedBufferRange(
				buffer.handle.gl_handle,
				offset,
				size,
				GL_MAP_WRITE_BIT
			);

		HOTPATH_ASSERT_VOID(!ptr, "Error during mapping");

		std::memcpy(ptr, data, buffer.size);
		glUnmapNamedBuffer(buffer.handle.gl_handle);

		LOG_DEBUG(std::format("Updated buffer {}", buffer.handle.gl_handle));
	}
	void					GL_Device::DeleteBuffer	( rhi::BufferHandle& vbo ) {

		HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Buffer doesn't exist");

		auto& buffer = m_buffers[vbo];
		HOTPATH_ASSERT_VOID(buffer.mapped, "Unable to delete buffer - still mapped");
		glDeleteBuffers(1, &buffer.handle.gl_handle);
		m_buffers.DeleteHandle(vbo);

		LOG_DEBUG(std::format("Deleted buffer {}", buffer.handle.gl_handle));
	
	}
	void*					GL_Device::MapBuffer	( rhi::BufferHandle vbo, rhi::MapFlag flags, size_t offset, size_t size ) {

		HOTPATH_ASSERT_NULLPTR(!m_buffers.Exists(vbo), "Handle doesn't exist");

		rhi::Buffer& buffer = m_buffers[vbo];

		HOTPATH_ASSERT_NULLPTR(offset + size > buffer.size || size >  buffer.size, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		void* ptr = glMapNamedBufferRange(buffer.handle.gl_handle, offset, size, TranslateMappingFlags(flags));
		
		HOTPATH_ASSERT_NULLPTR(!ptr, "Error during mapping");

		LOG_DEBUG(std::format("Mapped buffer {}", buffer.handle.gl_handle));
		return ptr;
	}
	void					GL_Device::UnmapBuffer	( rhi::BufferHandle vbo ) {

		HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Handle does not exist");

		rhi::Buffer& buffer = m_buffers[vbo];
		HOTPATH_ASSERT_VOID(!buffer.mapped, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.gl_handle);

		LOG_DEBUG(std::format("Unmapped buffer {}", buffer.handle.gl_handle));
	}
	void					GL_Device::AttachElementBufferToLayout( rhi::BufferHandle ebo, rhi::VertexLayoutHandle vao ) {
		HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Layout doesn't exists");
		HOTPATH_ASSERT_VOID(!m_buffers.Exists(ebo), "Buffer doesn't exists");

		glVertexArrayElementBuffer(m_layouts[vao].vao, m_buffers[ebo].handle.gl_handle);

	}

	///////////////////////////////////////////////////
	/// Layouts
	///////////////////////////////////////////////////

	rhi::VertexLayoutHandle GL_Device::CreateVertexLayout( const rhi::VertexLayoutDescriptor& desc, rhi::BufferHandle vbo ) {

		HOTPATH_ASSERT_CUSTOM(!m_buffers.Exists(vbo), "Buffer doesn't exists", rhi::VertexLayoutHandle{});
		HOTPATH_ASSERT_CUSTOM(desc.attributes.size() <= 0, "VertexLayout has no attributes", rhi::VertexLayoutHandle{});

		rhi::VertexLayout layout;
		rhi::Buffer& buffer = m_buffers[vbo];

		glCreateVertexArrays(1, &layout.vao);
		glVertexArrayVertexBuffer(
			layout.vao,
			desc.binding,
			buffer.handle.gl_handle,
			desc.offset,
			desc.stride
		);

		for (int i = 0; i < desc.attributes.size(); i++) {

			glVertexArrayAttribFormat(
				layout.vao,
				desc.attributes[i].shader_location,
				desc.attributes[i].format,
				GL_FLOAT,
				GL_FALSE,
				desc.attributes[i].relative_offset
			);

			glVertexArrayAttribBinding(
				layout.vao,
				desc.attributes[i].shader_location,
				desc.binding
			);

			glEnableVertexArrayAttrib(layout.vao, desc.attributes[i].shader_location);

		}

		return m_layouts.CreateHandle(std::move(layout));

	}
	void					GL_Device::DeleteVertexLayout( rhi::VertexLayoutHandle& layout ) {


	}


	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	rhi::ShaderHandle	GL_Device::CreateShader	( const rhi::ShaderDescriptor& desc ) {
		if (m_shaders.DenseSize() >= MAX_SHADERS) {
			LOG_ERROR("Max shaders reached");
			return rhi::ShaderHandle{};
		}
		std::string&& vfile = AssetService::LoadShader(desc.vertex_source);
		const char* vcstr = vfile.c_str();

		std::string&& ffile = AssetService::LoadShader(desc.fragment_source);
		const char* fcstr = ffile.c_str();

		rhi::Shader shader;

		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vshader, 1, &vcstr, nullptr);
		glShaderSource(fshader, 1, &fcstr, nullptr);

		glCompileShader(vshader);
		glCompileShader(fshader);

		shader.handle = glCreateProgram();

		glAttachShader(shader.handle, vshader);
		glAttachShader(shader.handle, fshader);

		glDeleteShader(vshader);
		glDeleteShader(fshader);

		glLinkProgram(shader.handle);

		return m_shaders.CreateHandle(std::move(shader));
	}
	void				GL_Device::BindShader	( rhi::ShaderHandle shader ) {
		HOTPATH_ASSERT_VOID(!m_shaders.Exists(shader), "Shader doesn't exists");

		glUseProgram(m_shaders[shader].handle);

	}
	void				GL_Device::DeleteShader	( rhi::ShaderHandle& shader ) {

	}


	///////////////////////////////////////////////////
	/// Textures
	///////////////////////////////////////////////////

	rhi::TextureHandle	GL_Device::CreateTexture2D(const rhi::TextureDescriptor& desc) {
		if (m_textures.DenseSize() >= MAX_TEXTURES) {
			LOG_ERROR("Max textures reached");
			return rhi::TextureHandle{};
		}

		rhi::Texture texture;


		return m_textures.CreateHandle(std::move(texture));
	}
	rhi::TextureHandle	GL_Device::CreateTexture3D(const rhi::TextureDescriptor& desc) {
		rhi::Texture texture;


		return m_textures.CreateHandle(std::move(texture));
	}
	void			GL_Device::DeleteTexture(rhi::TextureHandle& texture) {

	}
	void			GL_Device::BindTexture(rhi::TextureHandle texture) {

	}


	///////////////////////////////////////////////////
	/// Other
	///////////////////////////////////////////////////

	void GL_Device::Draw(rhi::VertexLayoutHandle vao, uint32_t vertex_count) {
		HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(m_layouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	}
	void GL_Device::DrawElements(rhi::VertexLayoutHandle vao, uint32_t indices_count) {
		HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(m_layouts[vao].vao);
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);

	}
	void GL_Device::BeginFrame() {

		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

	}
	void GL_Device::EndFrame() {

		glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeWindow()));

	}


	///////////////////////////////////////////////////
	/// Private helpers
	///////////////////////////////////////////////////

	bool		GL_Device::IsValidBufferDescriptor	( const rhi::BufferDescriptor& desc ) {

		if (desc.buffer_usage == rhi::BufferUsage::Static) {

			if ((desc.map_flags & ~(rhi::map_flags::Coherent | rhi::map_flags::Read)) != 0) {
				LOG_ERROR("Invalid buffer descriptor");
				return false;
			}

		}
		else if ((desc.map_flags & rhi::map_flags::Coherent) && !(desc.map_flags & rhi::map_flags::Persistent)) {
			LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}
	GLbitfield	GL_Device::TranslateMappingFlags	( rhi::MapFlag flags ) {
		GLbitfield flag = 0;

		if (flags & rhi::map_flags::Persistent)			flag |= GL_MAP_PERSISTENT_BIT;
		if (flags & rhi::map_flags::Write)				flag |= GL_MAP_WRITE_BIT;
		if (flags & rhi::map_flags::Read)				flag |= GL_MAP_READ_BIT;
		if (flags & rhi::map_flags::Coherent)			flag |= GL_MAP_COHERENT_BIT;
		if (flags & rhi::map_flags::Invalidate_Buffer)	flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags & rhi::map_flags::Invalidate_Range)	flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags & rhi::map_flags::Unsynchronized)		flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}

}