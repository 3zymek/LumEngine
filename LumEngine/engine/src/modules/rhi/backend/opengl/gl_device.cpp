#include "rhi/backend/opengl/gl_device.hpp"
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "window_context/window.hpp"
#include "rhi/rhi_common.hpp"
namespace lum::gl {

	///////////////////////////////////////////////////
	/// Buffers
	///////////////////////////////////////////////////

	rhi::BufferHandle		GL_Device::CreateVertexBuffer	( const BufferDescriptor& desc ) {
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
	rhi::BufferHandle		GL_Device::CreateElementBuffer	( const BufferDescriptor& desc ) {
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
	rhi::BufferHandle		GL_Device::CreateUniformBuffer	( const BufferDescriptor& desc ) {
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
	void					GL_Device::UpdateBuffer			( const BufferHandle& vbo, cvptr data, size_t offset, size_t size ) {

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
	void					GL_Device::DeleteBuffer			( BufferHandle& vbo ) {

		HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Buffer doesn't exist");

		auto& buffer = m_buffers[vbo];
		HOTPATH_ASSERT_VOID(buffer.mapped, "Unable to delete buffer - still mapped");
		glDeleteBuffers(1, &buffer.handle.gl_handle);
		m_buffers.DeleteHandle(vbo);

		LOG_DEBUG(std::format("Deleted buffer {}", buffer.handle.gl_handle));
	
	}
	vptr					GL_Device::MapBuffer			( const BufferHandle& vbo, MapFlag flags, size_t offset, size_t size ) {

		HOTPATH_ASSERT_NULLPTR(!m_buffers.Exists(vbo), "Handle doesn't exist");

		rhi::Buffer& buffer = m_buffers[vbo];

		HOTPATH_ASSERT_NULLPTR(offset + size > buffer.size || size >  buffer.size, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		vptr ptr = glMapNamedBufferRange(buffer.handle.gl_handle, offset, size, TranslateMappingFlags(flags));
		
		HOTPATH_ASSERT_NULLPTR(!ptr, "Error during mapping");

		LOG_DEBUG(std::format("Mapped buffer {}", buffer.handle.gl_handle));
		return ptr;
	}
	void					GL_Device::UnmapBuffer			( const BufferHandle& vbo ) {

		HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Handle does not exist");

		rhi::Buffer& buffer = m_buffers[vbo];
		HOTPATH_ASSERT_VOID(!buffer.mapped, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.gl_handle);

		LOG_DEBUG(std::format("Unmapped buffer {}", buffer.handle.gl_handle));
	}
	void					GL_Device::AttachElementBufferToLayout( const BufferHandle& ebo, const VertexLayoutHandle& vao ) {
		HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Layout doesn't exists");
		HOTPATH_ASSERT_VOID(!m_buffers.Exists(ebo), "Buffer doesn't exists");

		glVertexArrayElementBuffer(m_layouts[vao].vao, m_buffers[ebo].handle.gl_handle);

	}
	void					GL_Device::SetUniformBufferBinding(const BufferHandle& ubo, int binding) {
		HOTPATH_ASSERT_VOID(!m_buffers.Exists(ubo), "Uniform buffer doesn't exists");

		rhi::Buffer buffer = m_buffers[ubo];
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer.handle.gl_handle);

	}

	///////////////////////////////////////////////////
	/// Layouts
	///////////////////////////////////////////////////

	rhi::VertexLayoutHandle GL_Device::CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo ) {

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
	void					GL_Device::DeleteVertexLayout( VertexLayoutHandle& layout ) {


	}

	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	rhi::ShaderHandle	GL_Device::CreateShader	( const ShaderDescriptor& desc ) {
		if (m_shaders.DenseSize() >= MAX_SHADERS) {
			LOG_ERROR("Max shaders reached");
			return rhi::ShaderHandle{};
		}
		std::string&& vfile = AssetService::LoadShader(desc.vertex_source);
		cstr vcstr = vfile.c_str();

		std::string&& ffile = AssetService::LoadShader(desc.fragment_source);
		cstr fcstr = ffile.c_str();

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
	void				GL_Device::BindShader	( const ShaderHandle& shader ) {
		HOTPATH_ASSERT_VOID(!m_shaders.Exists(shader), "Shader doesn't exists");

		glUseProgram(m_shaders[shader].handle);

	}
	void				GL_Device::DeleteShader	( ShaderHandle& shader ) {

	}
	void				GL_Device::SetMat4		( const ShaderHandle& shader, cstr location, const glm::mat4& mat ) {

		GLuint loc = glGetUniformLocation(m_shaders[shader].handle, location);
		if (loc == -1) LOG_ERROR(std::format("Couldn't localize uniform named {}", location));
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

	}
	void				GL_Device::Setf			( const ShaderHandle& shader, cstr location, float value ) {

	}
	void				GL_Device::Seti			( const ShaderHandle& shader, cstr location, int value ) {

	}
	void				GL_Device::SetVec3		( const ShaderHandle& shader, cstr location, const glm::vec4& vec ) {

	}
	void				GL_Device::SetVec3		( const ShaderHandle& shader, cstr location, const glm::vec3& vec ) {

	}
	void				GL_Device::SetVec3		( const ShaderHandle& shader, cstr location, const glm::vec2& vec ) {

	}

	///////////////////////////////////////////////////
	/// Textures
	///////////////////////////////////////////////////

	rhi::TextureHandle	GL_Device::CreateTexture2D(const TextureDescriptor& desc) {
		if (m_textures.DenseSize() >= MAX_TEXTURES) {
			LOG_ERROR("Max textures reached");
			return rhi::TextureHandle{};
		}

		rhi::Texture texture;
		TextureData data = AssetService::LoadTexture(desc.filename);
		texture.height = data.height;
		texture.width = data.width;
		texture.mag_filters = desc.mag_filter;
		texture.min_filters = desc.min_filter;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.gl_handle);

		int mipmaps = (desc.min_filter != rhi::TextureMinFilter::Linear && desc.min_filter != rhi::TextureMinFilter::Nearest) ?
			rhi::mipmap_lvls(texture.width, texture.height) : 1;

		glTextureStorage2D(
			texture.handle.gl_handle,
			mipmaps,
			GL_RGBA8,
			texture.width,
			texture.height
		);

		glTextureSubImage2D(
			texture.handle.gl_handle, 
			0, 
			0, 
			0, 
			texture.width, 
			texture.height, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			data.pixels.data()
		);

		if (mipmaps > 1)
			glGenerateTextureMipmap(texture.handle.gl_handle);
		
		GLfloat max_anisotropy;
		float anisotropy = desc.anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
		if (anisotropy < max_anisotropy && anisotropy > 0)
			glTextureParameteri(texture.handle.gl_handle, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);

		glTextureParameteri(texture.handle.gl_handle, GL_TEXTURE_MIN_FILTER, TranslateTextureMinFilter(desc.min_filter));
		glTextureParameteri(texture.handle.gl_handle, GL_TEXTURE_MAG_FILTER, TranslateTextureMagFilter(desc.mag_filter));
		glTextureParameteri(texture.handle.gl_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture.handle.gl_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return m_textures.CreateHandle(std::move(texture));
	}
	// TO IMPLEMENT:
	rhi::TextureHandle	GL_Device::CreateTexture3D(const TextureDescriptor& desc ) {

		rhi::Texture texture;
		return m_textures.CreateHandle(std::move(texture));
	}
	void			GL_Device::DeleteTexture( TextureHandle& texture ) {

	}
	void			GL_Device::BindTexture( const TextureHandle& texture ) {

		rhi::Texture& tex = m_textures[texture];

		glBindTextureUnit(0, tex.handle.gl_handle);

	}


	///////////////////////////////////////////////////
	/// Other
	///////////////////////////////////////////////////

	void GL_Device::Draw(const VertexLayoutHandle& vao, uint32_t vertex_count) {
		HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(m_layouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	}
	void GL_Device::DrawElements( const VertexLayoutHandle& vao, uint32_t indices_count) {
		HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(m_layouts[vao].vao);
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);

	}
	void GL_Device::BeginFrame() {

		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

	}
	void GL_Device::EndFrame() {

		glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeWindow()));

	}


	///////////////////////////////////////////////////
	/// Private helpers
	///////////////////////////////////////////////////

	void GL_Device::CacheUniformLocations( ) {




	}

	GLbitfield	GL_Device::TranslateTextureMinFilter(const rhi::TextureMinFilter& filter) {

		switch (filter) {
		case rhi::TextureMinFilter::Linear: return GL_LINEAR;
		case rhi::TextureMinFilter::Nearest: return GL_NEAREST;
		case rhi::TextureMinFilter::Linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
		case rhi::TextureMinFilter::Linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
		case rhi::TextureMinFilter::Nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
		case rhi::TextureMinFilter::Nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
		}

		return GL_NEAREST;

	}
	GLbitfield	GL_Device::TranslateTextureMagFilter(const rhi::TextureMagFilter& filter) {

		switch(filter) {
		case rhi::TextureMagFilter::Linear: return GL_LINEAR;
		case rhi::TextureMagFilter::Nearest: return GL_NEAREST;
		}

		return GL_NEAREST;

	}
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
	GLbitfield	GL_Device::TranslateMappingFlags	( MapFlag flags ) {
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