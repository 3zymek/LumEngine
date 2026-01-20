#include "rhi/backend/opengl/gl_device.hpp"
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "window_context/window.hpp"
#include "rhi/rhi_common.hpp"
#if LUM_ENABLE_IMGUI == 1
	#include "imgui.h"
	#include "imgui_impl_glfw.h"
	#include "imgui_impl_opengl3.h"
#endif
namespace lum::gl {

	///////////////////////////////////////////////////
	/// Buffers
	///////////////////////////////////////////////////

	rhi::BufferHandle GL_Device::CreateVertexBuffer( const BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (m_buffers.DenseSize() >= MAX_BUFFERS) {
			LUM_LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
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

		LUM_LOG_INFO(std::format("Created vertex buffer {}", buffer.handle.gl_handle));
		return m_buffers.CreateHandle(std::move(buffer));
	}
	rhi::BufferHandle GL_Device::CreateElementBuffer( const BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (m_buffers.DenseSize() >= MAX_BUFFERS) {
			LUM_LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
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

		LUM_LOG_INFO(std::format("Created element buffer {}", buffer.handle.gl_handle));
		return m_buffers.CreateHandle(std::move(buffer));
	}
	rhi::BufferHandle GL_Device::CreateUniformBuffer( const BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (m_buffers.DenseSize() >= MAX_BUFFERS) {
			LUM_LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
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

		LUM_LOG_INFO(std::format("Created uniform buffer {}", buffer.handle.gl_handle));
		return m_buffers.CreateHandle(std::move(buffer));
	}
	void GL_Device::UpdateBuffer( const BufferHandle& vbo, cvptr data, usize offset, usize size ) {

		LUM_HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Buffer does not exist");

		rhi::Buffer& buffer = m_buffers[vbo];

		LUM_HOTPATH_ASSERT_VOID(offset + size > buffer.size, "Invalid offset or size");
		if (size == 0) size = buffer.size;
		LUM_HOTPATH_ASSERT_VOID(
			buffer.usage == rhi::BufferUsage::Static, 
			std::format("Buffer {} is static, cannot be updated", buffer.handle.gl_handle)
		);
		LUM_HOTPATH_ASSERT_VOID(
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

		LUM_HOTPATH_ASSERT_VOID(!ptr, "Error during mapping");

		std::memcpy(ptr, data, buffer.size);
		glUnmapNamedBuffer(buffer.handle.gl_handle);

		LUM_LOG_DEBUG(std::format("Updated buffer {}", buffer.handle.gl_handle));
	}
	void GL_Device::DeleteBuffer( BufferHandle& vbo ) {

		LUM_HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Buffer doesn't exist");

		auto& buffer = m_buffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(buffer.mapped, "Unable to delete buffer - still mapped");
		glDeleteBuffers(1, &buffer.handle.gl_handle);
		m_buffers.DeleteHandle(vbo);

		LUM_LOG_INFO(std::format("Deleted buffer {}", buffer.handle.gl_handle));
	
	}
	vptr GL_Device::MapBuffer( const BufferHandle& vbo, mapflag_t flags, usize offset, usize size ) {

		LUM_HOTPATH_ASSERT_NULLPTR(!m_buffers.Exists(vbo), "Handle doesn't exist");

		rhi::Buffer& buffer = m_buffers[vbo];

		LUM_HOTPATH_ASSERT_NULLPTR(offset + size > buffer.size || size >  buffer.size, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		vptr ptr = glMapNamedBufferRange(buffer.handle.gl_handle, offset, size, TranslateMappingFlags(flags));
		
		LUM_HOTPATH_ASSERT_NULLPTR(!ptr, "Error during mapping");

		LUM_LOG_DEBUG(std::format("Mapped buffer {}", buffer.handle.gl_handle));
		return ptr;
	}
	void GL_Device::UnmapBuffer( const BufferHandle& vbo ) {

		LUM_HOTPATH_ASSERT_VOID(!m_buffers.Exists(vbo), "Handle does not exist");

		rhi::Buffer& buffer = m_buffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(!buffer.mapped, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.gl_handle);

		LUM_LOG_DEBUG(std::format("Unmapped buffer {}", buffer.handle.gl_handle));
	}
	void GL_Device::AttachElementBufferToLayout( const BufferHandle& ebo, const VertexLayoutHandle& vao ) {
		LUM_HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Layout doesn't exists");
		LUM_HOTPATH_ASSERT_VOID(!m_buffers.Exists(ebo), "Buffer doesn't exists");

		glVertexArrayElementBuffer(m_layouts[vao].vao, m_buffers[ebo].handle.gl_handle);

	}
	void GL_Device::SetUniformBufferBinding(const BufferHandle& ubo, int32 binding) {
		LUM_HOTPATH_ASSERT_VOID(!m_buffers.Exists(ubo), "Uniform buffer doesn't exists");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_buffers[ubo].handle.gl_handle);

	}


	///////////////////////////////////////////////////
	/// Layouts
	///////////////////////////////////////////////////

	rhi::VertexLayoutHandle GL_Device::CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo ) {

		LUM_HOTPATH_ASSERT_CUSTOM(!m_buffers.Exists(vbo), "Buffer doesn't exists", rhi::VertexLayoutHandle{});
		LUM_HOTPATH_ASSERT_CUSTOM(desc.attributes.size() <= 0, "VertexLayout has no attributes", rhi::VertexLayoutHandle{});

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
				TranslateDataFormat(desc.attributes[i].format),
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
	void GL_Device::DeleteVertexLayout( VertexLayoutHandle& layout ) {


	}


	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	rhi::ShaderHandle GL_Device::CreateShader( const ShaderDescriptor& desc ) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			m_shaders.DenseSize() >= MAX_SHADERS,
			"Max shaders reached",
			rhi::ShaderHandle{}
		);

		std::string&& vfile = AssetService::LoadInternalShader(desc.vertex_source);
		ccharptr vcstr = vfile.c_str();

		std::string&& ffile = AssetService::LoadInternalShader(desc.fragment_source);
		ccharptr fcstr = ffile.c_str();

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
	void GL_Device::BindShader( const ShaderHandle& shader ) {
		LUM_HOTPATH_ASSERT_VOID(!m_shaders.Exists(shader), "Shader doesn't exists");

		glUseProgram(m_shaders[shader].handle);

	}
	void GL_Device::DeleteShader( ShaderHandle& shader ) {

	}
	void GL_Device::SetMat4( const ShaderHandle& shader, ccharptr location, const glm::mat4& mat ) {

		GLuint loc = glGetUniformLocation(m_shaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

	}
	void GL_Device::Setf( const ShaderHandle& shader, ccharptr location, float32 value ) {

		GLuint loc = glGetUniformLocation(m_shaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1f(loc, value);

	}
	void GL_Device::Seti( const ShaderHandle& shader, ccharptr location, int32 value ) {

		GLuint loc = glGetUniformLocation(m_shaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1i(loc, value);

	}
	void GL_Device::SetVec4( const ShaderHandle& shader, ccharptr location, const glm::vec4& vec ) {

		GLuint loc = glGetUniformLocation(m_shaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform4fv(loc, 1, glm::value_ptr(vec));

	}
	void GL_Device::SetVec3( const ShaderHandle& shader, ccharptr location, const glm::vec3& vec ) {

		GLuint loc = glGetUniformLocation(m_shaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform3fv(loc, 1, glm::value_ptr(vec));

	}
	void GL_Device::SetVec2( const ShaderHandle& shader, ccharptr location, const glm::vec2& vec ) {

		GLuint loc = glGetUniformLocation(m_shaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform2fv(loc, 1, glm::value_ptr(vec));

	}


	///////////////////////////////////////////////////
	/// Textures
	///////////////////////////////////////////////////

	rhi::TextureHandle GL_Device::CreateTexture2D( const TextureDescriptor& desc ) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			m_textures.DenseSize() >= MAX_TEXTURES,
			"Max textures reached",
			rhi::TextureHandle{}
		);

		rhi::Texture texture;
		TextureData data = AssetService::LoadTexture(desc.filename);
		texture.height = data.height;
		texture.width = data.width;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.gl_handle);

		int mipmaps = rhi::mipmap_lvls(texture.width, texture.height);

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

		return m_textures.CreateHandle(std::move(texture));
	}
	// TO IMPLEMENT:
	rhi::TextureHandle GL_Device::CreateTexture3D	( const TextureDescriptor& desc ) {

		rhi::Texture texture;
		return m_textures.CreateHandle(std::move(texture));
	}
	void GL_Device::DeleteTexture( TextureHandle& texture ) {
		LUM_HOTPATH_ASSERT_VOID(!m_textures.Exists(texture), std::format("Texture {} doesn't exist", texture.id));

		glDeleteTextures(1, &m_textures[texture].handle.gl_handle);

		m_textures.DeleteHandle(texture);

	}
	void GL_Device::SetTextureBinding( const TextureHandle& texture, uint16 binding ) {
		LUM_HOTPATH_ASSERT_VOID(!m_textures.Exists(texture), std::format("Texture {} doesn't exists", texture.id));

		m_textures[texture].binding = binding;

	}
	void GL_Device::BindTexture( const TextureHandle& texture, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!m_textures.Exists(texture), "Texture doesn't exists");
		LUM_HOTPATH_ASSERT_VOID(
			m_textures[texture].binding == LUM_NULL_BINDING &&
			binding == LUM_NULL_BINDING,
			std::format("Binding has not been given to texture {}", texture.id)
		);

		uint16 bind = (binding == LUM_NULL_BINDING) ? m_textures[texture].binding : binding;

		glBindTextureUnit(bind, m_textures[texture].handle.gl_handle);

	}
	rhi::SamplerHandle GL_Device::CreateSampler( const SamplerDescriptor& desc ) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			m_samplers.DenseSize() >= MAX_SAMPLERS, 
			"Max samplers reached", 
			rhi::SamplerHandle{}
		);

		Sampler sampler;
		
		glCreateSamplers(1, &sampler.handle);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MAG_FILTER, TranslateTextureMagFilter(desc.mag_filter));
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MIN_FILTER, TranslateTextureMinFilter(desc.min_filter));

		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_S, TranslateTextureWrap(desc.wrap_s));
		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_T, TranslateTextureWrap(desc.wrap_t));

		GLfloat max_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
		int final_anisotropy = (desc.anisotropy > max_anisotropy) ? max_anisotropy : desc.anisotropy;
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MAX_ANISOTROPY, final_anisotropy);

		return m_samplers.CreateHandle(std::move(sampler));
	}
	// TO IMPLEMENT:
	void GL_Device::SetSamplerBinding(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!m_samplers.Exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		m_samplers[sampler].binding = binding;

	}
	// TO CHANGE: add binding slot
	void GL_Device::BindSampler( const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!m_samplers.Exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));
		LUM_HOTPATH_ASSERT_VOID(
			m_samplers[sampler].binding == LUM_NULL_BINDING && 
			binding == LUM_NULL_BINDING, 
			std::format("Binding has not been given to sampler {}", sampler.id)
		);

		uint16_t bind = (binding == LUM_NULL_BINDING) ? m_samplers[sampler].binding : binding;
		
		glBindSampler(bind, m_samplers[sampler].handle);

	}
	void GL_Device::DeleteSampler( SamplerHandle sampler ) {
		LUM_HOTPATH_ASSERT_VOID(!m_samplers.Exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		Sampler& samp = m_samplers[sampler];
		glDeleteSamplers(1, &samp.handle);

		m_samplers.DeleteHandle(sampler);
	}


	///////////////////////////////////////////////////
	/// Other
	///////////////////////////////////////////////////

	void GL_Device::Draw(const VertexLayoutHandle& vao, uint32 vertex_count) {
		LUM_HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(m_layouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	}
	void GL_Device::DrawElements( const VertexLayoutHandle& vao, uint32 indices_count) {
		LUM_HOTPATH_ASSERT_VOID(!m_layouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(m_layouts[vao].vao);
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);

	}
	void GL_Device::BeginFrame( ) {

		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

#		if LUM_ENABLE_IMGUI == 1
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
#		endif

		glfwPollEvents();

	}
	void GL_Device::EndFrame( ) {

#		if LUM_ENABLE_IMGUI == 1
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


#		endif

		glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeWindow()));

	}


	///////////////////////////////////////////////////
	/// Private helpers
	///////////////////////////////////////////////////

	// TO IMPLEMENT:
	void GL_Device::CacheUniformLocations( ) {




	}
	GLbitfield	GL_Device::TranslateDataFormat(const rhi::DataFormat format) noexcept {
		switch (format) {
		case rhi::DataFormat::Float: return 1;
		case rhi::DataFormat::Float2: return 2;
		case rhi::DataFormat::Float3: return 3;
		case rhi::DataFormat::Float4: return 4;
		case rhi::DataFormat::Mat3: return 9;
		case rhi::DataFormat::Mat4: return 16;
		}
		return 1;
	}
	GLbitfield	GL_Device::TranslateTextureMinFilter( const rhi::SamplerMinFilter filter ) noexcept  {

		switch (filter) {
		case rhi::SamplerMinFilter::Linear: return GL_LINEAR;
		case rhi::SamplerMinFilter::Nearest: return GL_NEAREST;
		case rhi::SamplerMinFilter::Linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
		case rhi::SamplerMinFilter::Linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
		case rhi::SamplerMinFilter::Nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
		case rhi::SamplerMinFilter::Nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
		}

		return GL_NEAREST;

	}
	GLbitfield	GL_Device::TranslateTextureMagFilter( const rhi::SamplerMagFilter filter ) noexcept {

		switch(filter) {
		case rhi::SamplerMagFilter::Linear: return GL_LINEAR;
		case rhi::SamplerMagFilter::Nearest: return GL_NEAREST;
		}

		return GL_NEAREST;

	}
	GLbitfield	GL_Device::TranslateTextureWrap		( const rhi::SamplerWrap wrap ) noexcept {
		switch (wrap) {
			case rhi::SamplerWrap::Clamp_border:	return GL_CLAMP_TO_BORDER;
			case rhi::SamplerWrap::Clamp_edge:		return GL_CLAMP_TO_EDGE;
			case rhi::SamplerWrap::Repeat:			return GL_REPEAT;
			case rhi::SamplerWrap::Repeat_mirrored: return GL_MIRRORED_REPEAT;
		}
		return GL_CLAMP_TO_EDGE;
	}
	bool GL_Device::IsValidBufferDescriptor	( const rhi::BufferDescriptor& desc ) noexcept {

		if (desc.buffer_usage == rhi::BufferUsage::Static) {

			if ((desc.map_flags & ~(rhi::map_flags::Coherent | rhi::map_flags::Read)) != 0) {
				LUM_LOG_ERROR("Invalid buffer descriptor");
				return false;
			}

		}
		else if ((desc.map_flags & rhi::map_flags::Coherent) && !(desc.map_flags & rhi::map_flags::Persistent)) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}
	GLbitfield	GL_Device::TranslateMappingFlags	( mapflag_t flags ) noexcept {
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