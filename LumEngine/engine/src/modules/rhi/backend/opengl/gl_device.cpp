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

		if (mBuffers.DenseSize() >= sMaxBuffers) {
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
			(skBufferUsageLookup[static_cast<byte>(desc.buffer_usage)])
			| TranslateMappingFlags(desc.map_flags);

		glCreateBuffers(1, &buffer.handle.gl_handle);
		glNamedBufferStorage(
			buffer.handle.gl_handle,
			desc.size,
			desc.data,
			init_flags
		);

		LUM_LOG_INFO(std::format("Created vertex buffer {}", buffer.handle.gl_handle));
		return mBuffers.CreateHandle(std::move(buffer));
	}
	rhi::BufferHandle GL_Device::CreateElementBuffer( const BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (mBuffers.DenseSize() >= sMaxBuffers) {
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
		return mBuffers.CreateHandle(std::move(buffer));
	}
	rhi::BufferHandle GL_Device::CreateUniformBuffer( const BufferDescriptor& desc ) {
		if (!IsValidBufferDescriptor(desc))
			return rhi::BufferHandle{};

		if (mBuffers.DenseSize() >= sMaxBuffers) {
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
		return mBuffers.CreateHandle(std::move(buffer));
	}
	void GL_Device::UpdateBuffer( const BufferHandle& vbo, cvptr data, usize offset, usize size ) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.Exists(vbo), "Buffer does not exist");

		rhi::Buffer& buffer = mBuffers[vbo];

		LUM_HOTPATH_ASSERT_VOID(offset + size > buffer.size, "Invalid offset or size");
		if (size == 0) size = buffer.size;
		LUM_HOTPATH_ASSERT_VOID(
			buffer.usage == rhi::BufferUsage::Static, 
			std::format("Buffer {} is static, cannot be updated", buffer.handle.gl_handle)
		);
		LUM_HOTPATH_ASSERT_VOID(
			!(buffer.flags & rhi::Mapflag::Write),
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

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.Exists(vbo), "Buffer doesn't exist");

		auto& buffer = mBuffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(buffer.mapped, "Unable to delete buffer - still mapped");
		glDeleteBuffers(1, &buffer.handle.gl_handle);
		mBuffers.DeleteHandle(vbo);

		LUM_LOG_INFO(std::format("Deleted buffer {}", buffer.handle.gl_handle));
	
	}
	vptr GL_Device::MapBuffer( const BufferHandle& vbo, Mapflag flags, usize offset, usize size ) {

		LUM_HOTPATH_ASSERT_NULLPTR(!mBuffers.Exists(vbo), "Handle doesn't exist");

		rhi::Buffer& buffer = mBuffers[vbo];

		LUM_HOTPATH_ASSERT_NULLPTR(offset + size > buffer.size || size >  buffer.size, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		vptr ptr = glMapNamedBufferRange(buffer.handle.gl_handle, offset, size, TranslateMappingFlags(flags));
		
		LUM_HOTPATH_ASSERT_NULLPTR(!ptr, "Error during mapping");

		LUM_LOG_DEBUG(std::format("Mapped buffer {}", buffer.handle.gl_handle));
		return ptr;
	}
	void GL_Device::UnmapBuffer( const BufferHandle& vbo ) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.Exists(vbo), "Handle does not exist");

		rhi::Buffer& buffer = mBuffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(!buffer.mapped, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.gl_handle);

		LUM_LOG_DEBUG(std::format("Unmapped buffer {}", buffer.handle.gl_handle));
	}
	void GL_Device::AttachElementBufferToLayout( const BufferHandle& ebo, const VertexLayoutHandle& vao ) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.Exists(vao), "Layout doesn't exists");
		LUM_HOTPATH_ASSERT_VOID(!mBuffers.Exists(ebo), "Buffer doesn't exists");

		glVertexArrayElementBuffer(mLayouts[vao].vao, mBuffers[ebo].handle.gl_handle);

	}
	void GL_Device::SetUniformBufferBinding(const BufferHandle& ubo, int32 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mBuffers.Exists(ubo), "Uniform buffer doesn't exists");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mBuffers[ubo].handle.gl_handle);

	}



	///////////////////////////////////////////////////
	/// Framebuffers
	///////////////////////////////////////////////////

	rhi::FramebufferHandle GL_Device::CreateFramebuffer() {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mFramebuffers.DenseSize() >= sMaxFramebuffers,
			"Max framebuffers reached",
			FramebufferHandle{}
		);

		Framebuffer fbo;

		glCreateFramebuffers(1, &fbo.handle);

		return mFramebuffers.CreateHandle(std::move(fbo));

	}
	rhi::TextureHandle GL_Device::CreateFramebufferTexture(const FramebufferTextureDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mTextures.DenseSize() >= sMaxTextures || desc.height <= 0 || desc.width <= 0,
			"Max textures reached",
			TextureHandle{}
		);

		Texture tex;

		glCreateTextures(GL_TEXTURE_2D, 1, &tex.handle.gl_handle);

		glTextureStorage2D(tex.handle.gl_handle, 1, GL_RGB8, desc.width, desc.height);

		return mTextures.CreateHandle(std::move(tex));
	}
	void GL_Device::SetFramebufferColorTexture(const FramebufferHandle& fbo, const TextureHandle& tex, uint8 index) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.Exists(fbo) || !mTextures.Exists(tex), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextures[tex].handle.gl_handle, 0);

	}
	void GL_Device::SetFramebufferDepthTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.Exists(fbo), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextures[tex].handle.gl_handle, 0);

	}
	void GL_Device::SetFramebufferStencilTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {

	}
	void GL_Device::DeleteFramebuffer(FramebufferHandle& buff) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.Exists(buff), "Framebuffer doesn't exists");

		Framebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.handle);

		mFramebuffers.DeleteHandle(buff);
	}
	void GL_Device::BindFramebuffer(const FramebufferHandle& buff) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.Exists(buff), "Framebuffer doesn't exists");

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffers[buff].handle);

	}
	void GL_Device::UnbindFramebuffer() {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}




	///////////////////////////////////////////////////
	/// Layouts
	///////////////////////////////////////////////////

	rhi::VertexLayoutHandle GL_Device::CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo ) {

		LUM_HOTPATH_ASSERT_CUSTOM(!mBuffers.Exists(vbo), "Buffer doesn't exists", rhi::VertexLayoutHandle{});
		LUM_HOTPATH_ASSERT_CUSTOM(desc.attributes.size() <= 0, "VertexLayout has no attributes", rhi::VertexLayoutHandle{});

		rhi::VertexLayout layout;
		rhi::Buffer& buffer = mBuffers[vbo];

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
				rhi::detail::skDataFormatLookup[static_cast<byte>(desc.attributes[i].format)],
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

		return mLayouts.CreateHandle(std::move(layout));

	}
	void GL_Device::DeleteVertexLayout( VertexLayoutHandle& layout ) {


	}


	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	rhi::ShaderHandle GL_Device::CreateShader( const ShaderDescriptor& desc ) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mShaders.DenseSize() >= sMaxShaders,
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

		return mShaders.CreateHandle(std::move(shader));
	}
	void GL_Device::BindShader( const ShaderHandle& shader ) {
		LUM_HOTPATH_ASSERT_VOID(!mShaders.Exists(shader), "Shader doesn't exists");

		glUseProgram(mShaders[shader].handle);

	}
	void GL_Device::DeleteShader( ShaderHandle& shader ) {

	}
	void GL_Device::SetMat4( const ShaderHandle& shader, ccharptr location, const glm::mat4& mat ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

	}
	void GL_Device::Setf( const ShaderHandle& shader, ccharptr location, float32 value ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1f(loc, value);

	}
	void GL_Device::Seti( const ShaderHandle& shader, ccharptr location, int32 value ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1i(loc, value);

	}
	void GL_Device::SetVec4( const ShaderHandle& shader, ccharptr location, const glm::vec4& vec ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform4fv(loc, 1, glm::value_ptr(vec));

	}
	void GL_Device::SetVec3( const ShaderHandle& shader, ccharptr location, const glm::vec3& vec ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform3fv(loc, 1, glm::value_ptr(vec));

	}
	void GL_Device::SetVec2( const ShaderHandle& shader, ccharptr location, const glm::vec2& vec ) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform2fv(loc, 1, glm::value_ptr(vec));

	}


	///////////////////////////////////////////////////
	/// Textures
	///////////////////////////////////////////////////

	rhi::TextureHandle GL_Device::CreateTexture2D( const TextureDescriptor& desc ) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mTextures.DenseSize() >= sMaxTextures,
			"Max textures reached",
			rhi::TextureHandle{}
		);

		Texture texture;
		TextureData data = AssetService::LoadTexture(desc.filename);

		uint32 width = (desc.width == 0) ? data.width : desc.width;
		uint32 height = (desc.height == 0) ? data.height : desc.height;

		texture.height = height;
		texture.width = width;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.gl_handle);

		int mipmaps = rhi::mipmap_lvls(width, height);

		glTextureStorage2D(
			texture.handle.gl_handle,
			mipmaps,
			GL_RGBA8,
			width,
			height
		);

		glTextureSubImage2D(
			texture.handle.gl_handle, 
			0, 
			0, 
			0, 
			width, 
			height, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			data.pixels.data()
		);

		if (mipmaps > 1)
			glGenerateTextureMipmap(texture.handle.gl_handle);

		return mTextures.CreateHandle(std::move(texture));
	}
	// TO IMPLEMENT:
	rhi::TextureHandle GL_Device::CreateTexture3D	( const TextureDescriptor& desc ) {

		rhi::Texture texture;
		return mTextures.CreateHandle(std::move(texture));
	}
	void GL_Device::DeleteTexture( TextureHandle& texture ) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.Exists(texture), std::format("Texture {} doesn't exist", texture.id));

		glDeleteTextures(1, &mTextures[texture].handle.gl_handle);

		mTextures.DeleteHandle(texture);

	}
	void GL_Device::SetTextureBinding( const TextureHandle& texture, uint16 binding ) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.Exists(texture), std::format("Texture {} doesn't exists", texture.id));

		mTextures[texture].binding = binding;

	}
	void GL_Device::BindTexture( const TextureHandle& texture, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.Exists(texture), "Texture doesn't exists");
		LUM_HOTPATH_ASSERT_VOID(
			mTextures[texture].binding == LUM_NULL_BINDING &&
			binding == LUM_NULL_BINDING,
			std::format("Binding has not been given to texture {}", texture.id)
		);

		uint16 bind = (binding == LUM_NULL_BINDING) ? mTextures[texture].binding : binding;

		glBindTextureUnit(bind, mTextures[texture].handle.gl_handle);

	}



	///////////////////////////////////////////////////
	/// Samplers
	///////////////////////////////////////////////////

	rhi::SamplerHandle GL_Device::CreateSampler( const SamplerDescriptor& desc ) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mSamplers.DenseSize() >= sMaxSamplers, 
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

		return mSamplers.CreateHandle(std::move(sampler));
	}
	// TO IMPLEMENT:
	void GL_Device::SetSamplerBinding(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.Exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		mSamplers[sampler].binding = binding;

	}
	// TO CHANGE: add binding slot
	void GL_Device::BindSampler( const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.Exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));
		LUM_HOTPATH_ASSERT_VOID(
			mSamplers[sampler].binding == LUM_NULL_BINDING && 
			binding == LUM_NULL_BINDING, 
			std::format("Binding has not been given to sampler {}", sampler.id)
		);

		uint16_t bind = (binding == LUM_NULL_BINDING) ? mSamplers[sampler].binding : binding;
		
		glBindSampler(bind, mSamplers[sampler].handle);
		

	}
	void GL_Device::DeleteSampler( SamplerHandle sampler ) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.Exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		Sampler& samp = mSamplers[sampler];
		glDeleteSamplers(1, &samp.handle);

		mSamplers.DeleteHandle(sampler);
	}




	///////////////////////////////////////////////////
	/// Pipelines
	///////////////////////////////////////////////////

	rhi::PipelineHandle GL_Device::CreatePipeline(const PipelineDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mPipelines.DenseSize() >= sMaxPipelines,
			"Max pipelines reached",
			PipelineHandle{}
		);

		Pipeline pipeline;
		pipeline.polygon_mode = desc.polygon_mode;
		pipeline.polygon_mode_faces = desc.polygon_mode_faces;


		return mPipelines.CreateHandle(std::move(pipeline));
	}
	void GL_Device::DeletePipeline(PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.Exists(pipeline), "Pipeline doesn't exist");
		
		mPipelines.DeleteHandle(pipeline);
	}
	void GL_Device::BindPipeline(const PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.Exists(pipeline), "Pipeline doesn't exist");

		Pipeline& pip = mPipelines[pipeline];

		glPolygonMode(
			skFacesLookup[static_cast<byte>(pip.polygon_mode_faces)], 
			skPolygonModeLookup[static_cast<byte>(pip.polygon_mode)]
		);
	
	}


	///////////////////////////////////////////////////
	/// Other
	///////////////////////////////////////////////////
	
	void GL_Device::EnableState(State states) {
		
		switch (states) {
		case State::depth_test:		{ glEnable(GL_DEPTH_TEST);		break; }
		case State::stencil_test:	{ glEnable(GL_STENCIL_TEST);	break; }
		case State::scissor:		{ glEnable(GL_SCISSOR_TEST);	break; }
		case State::blend:			{ glEnable(GL_BLEND);			break; }
		case State::cull_face:		{ glEnable(GL_CULL_FACE);		break; }
		}

	}
	void GL_Device::DisableState(State states) {

		switch (states) {
		case State::depth_test:		{ glDisable(GL_DEPTH_TEST);		break; }
		case State::stencil_test:	{ glDisable(GL_STENCIL_TEST);	break; }
		case State::scissor:		{ glDisable(GL_SCISSOR_TEST);	break; }
		case State::blend:			{ glDisable(GL_BLEND);			break; }
		case State::cull_face:		{ glDisable(GL_CULL_FACE);		break; }
		}

	}
	void GL_Device::Draw(const VertexLayoutHandle& vao, uint32 vertex_count) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	}
	void GL_Device::DrawElements( const VertexLayoutHandle& vao, uint32 indices_count) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.Exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);

	}
	void GL_Device::BeginFrame( ) {

		glViewport(0, 0, pWindow->GetWidth(), pWindow->GetHeight());
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

		glfwSwapBuffers(static_cast<GLFWwindow*>(pWindow->GetNativeWindow()));

	}


	///////////////////////////////////////////////////
	/// Private helpers
	///////////////////////////////////////////////////

	// TO IMPLEMENT:
	void GL_Device::CacheUniformLocations( ) {




	}
	GLbitfield	GL_Device::TranslateDataFormat( const rhi::DataFormat format ) noexcept {
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
	GLbitfield	GL_Device::TranslateTextureWrap( const rhi::SamplerWrap wrap ) noexcept {
		switch (wrap) {
			case rhi::SamplerWrap::Clamp_border:	return GL_CLAMP_TO_BORDER;
			case rhi::SamplerWrap::Clamp_edge:		return GL_CLAMP_TO_EDGE;
			case rhi::SamplerWrap::Repeat:			return GL_REPEAT;
			case rhi::SamplerWrap::Repeat_mirrored: return GL_MIRRORED_REPEAT;
		}
		return GL_CLAMP_TO_EDGE;
	}
	bool GL_Device::IsValidBufferDescriptor( const rhi::BufferDescriptor& desc ) noexcept {

		if (desc.buffer_usage == rhi::BufferUsage::Static) {

			if ((desc.map_flags & ~(rhi::Mapflag::Coherent | rhi::Mapflag::Read)) != 0) {
				LUM_LOG_ERROR("Invalid buffer descriptor");
				return false;
			}

		}
		else if ((desc.map_flags & rhi::Mapflag::Coherent) && !(desc.map_flags & rhi::Mapflag::Persistent)) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}
	GLbitfield	GL_Device::TranslateMappingFlags( Mapflag flags ) noexcept {
		GLbitfield flag = 0;

		if (flags & rhi::Mapflag::Persistent)			flag |= GL_MAP_PERSISTENT_BIT;
		if (flags & rhi::Mapflag::Write)				flag |= GL_MAP_WRITE_BIT;
		if (flags & rhi::Mapflag::Read)					flag |= GL_MAP_READ_BIT;
		if (flags & rhi::Mapflag::Coherent)				flag |= GL_MAP_COHERENT_BIT;
		if (flags & rhi::Mapflag::Invalidate_Buffer)	flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags & rhi::Mapflag::Invalidate_Range)		flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags & rhi::Mapflag::Unsynchronized)		flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}
	GLbitfield GL_Device::TranslateStateFlags( State flags ) noexcept {

		switch (flags) {
		case State::depth_test: return GL_DEPTH_TEST;
		case State::stencil_test: return GL_STENCIL_TEST;
		case State::scissor: return GL_SCISSOR_TEST;
		case State::blend: return GL_BLEND;
		case State::cull_face: return GL_CULL_FACE;
		}

	}

}