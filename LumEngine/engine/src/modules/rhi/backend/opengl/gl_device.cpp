#include "rhi/backend/opengl/gl_device.hpp"
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "window_context/window.hpp"
#include "rhi/rhi_common.hpp"
#include "core/core_pch.hpp"
#if LUM_ENABLE_IMGUI == 1
	#include "imgui.h"
	#include "imgui_impl_glfw.h"
	#include "imgui_impl_opengl3.h"
#endif
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



	///////////////////////////////////////////////////
	/// Framebuffers
	///////////////////////////////////////////////////

	FramebufferHandle GLDevice::CreateFramebuffer() {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mFramebuffers.dense_size() >= skMaxFramebuffers,
			"Max framebuffers reached",
			FramebufferHandle{}
		);

		Framebuffer fbo;

		glCreateFramebuffers(1, &fbo.handle);

		return mFramebuffers.create_handle(std::move(fbo));

	}
	TextureHandle GLDevice::CreateFramebufferTexture(const FramebufferTextureDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mTextures.dense_size() >= skMaxTextures || desc.height <= 0 || desc.width <= 0,
			"Max textures reached",
			TextureHandle{}
		);

		Texture tex;

		glCreateTextures(GL_TEXTURE_2D, 1, &tex.handle.glHandle);

		GLenum format = (desc.attachment == FramebufferAttachment::DepthAttach) ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
		glTextureStorage2D(tex.handle.glHandle, 1, format, desc.width, desc.height);

		return mTextures.create_handle(std::move(tex));
	}
	void GLDevice::SetFramebufferColorTexture(const FramebufferHandle& fbo, const TextureHandle& tex, uint8 index) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(fbo) || !mTextures.exists(tex), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextures[tex].handle.glHandle, 0);

	}
	void GLDevice::SetFramebufferDepthTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(fbo), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextures[tex].handle.glHandle, 0);

	}
	void GLDevice::SetFramebufferStencilTexture(const FramebufferHandle& fbo, const TextureHandle& tex) {

	}
	void GLDevice::ClearFramebuffer(FramebufferHandle fbo, glm::vec4 color, float32 depth) {
		BindFramebuffer(fbo);
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(std::clamp(depth, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void GLDevice::DeleteFramebuffer(FramebufferHandle& buff) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(buff), "Framebuffer doesn't exists");

		Framebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.handle);

		mFramebuffers.delete_handle(buff);
	}
	void GLDevice::BindFramebuffer(const FramebufferHandle& buff) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(buff), "Framebuffer doesn't exists");

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffers[buff].handle);

	}
	void GLDevice::UnbindFramebuffer() {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}




	///////////////////////////////////////////////////
	/// Layouts
	///////////////////////////////////////////////////

	VertexLayoutHandle GLDevice::CreateVertexLayout(const VertexLayoutDescriptor& desc, const BufferHandle& vbo) {

		LUM_HOTPATH_ASSERT_CUSTOM(!mBuffers.exists(vbo), "Buffer doesn't exists", VertexLayoutHandle{});
		LUM_HOTPATH_ASSERT_CUSTOM(desc.attributes.size() <= 0, "VertexLayout has no attributes", VertexLayoutHandle{});

		VertexLayout layout;
		Buffer& buffer = mBuffers[vbo];

		glCreateVertexArrays(1, &layout.vao);
		glVertexArrayVertexBuffer(
			layout.vao,
			desc.binding,
			buffer.handle.glHandle,
			desc.offset,
			desc.stride
		);

		for (int i = 0; i < desc.attributes.size(); i++) {

			glVertexArrayAttribFormat(
				layout.vao,
				desc.attributes[i].shaderLocation,
				detail::skDataFormatLookup[lookup_cast(desc.attributes[i].format)],
				GL_FLOAT,
				GL_FALSE,
				desc.attributes[i].relativeOffset
			);

			glVertexArrayAttribBinding(
				layout.vao,
				desc.attributes[i].shaderLocation,
				desc.binding
			);

			glEnableVertexArrayAttrib(layout.vao, desc.attributes[i].shaderLocation);

		}

		return mLayouts.create_handle(std::move(layout));

	}
	void GLDevice::DeleteVertexLayout(VertexLayoutHandle& layout) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.exists(layout), "Layout doesn't exist");

		glDeleteVertexArrays(1, &mLayouts[layout].vao);
		mLayouts.delete_handle(layout);

	}



	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	ShaderHandle GLDevice::CreateShader(const ShaderDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mShaders.dense_size() >= skMaxShaders,
			"Max shaders reached",
			ShaderHandle{}
		);

		std::string&& vfile = AssetService::LoadInternalShader(desc.vertex_source);
		ccharptr vcstr = vfile.c_str();

		std::string&& ffile = AssetService::LoadInternalShader(desc.fragment_source);
		ccharptr fcstr = ffile.c_str();

		Shader shader;

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

		return mShaders.create_handle(std::move(shader));
	}
	void GLDevice::BindShader(const ShaderHandle& shader) {
		LUM_HOTPATH_ASSERT_VOID(!mShaders.exists(shader), "Shader doesn't exists");

		glUseProgram(mShaders[shader].handle);

	}
	void GLDevice::DeleteShader(ShaderHandle& shader) {

	}
	void GLDevice::SetMat4(const ShaderHandle& shader, ccharptr location, const glm::mat4& mat) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

	}
	void GLDevice::Setf(const ShaderHandle& shader, ccharptr location, float32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1f(loc, value);

	}
	void GLDevice::Seti(const ShaderHandle& shader, ccharptr location, int32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1i(loc, value);

	}
	void GLDevice::SetVec4(const ShaderHandle& shader, ccharptr location, const glm::vec4& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform4fv(loc, 1, glm::value_ptr(vec));

	}
	void GLDevice::SetVec3(const ShaderHandle& shader, ccharptr location, const glm::vec3& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform3fv(loc, 1, glm::value_ptr(vec));

	}
	void GLDevice::SetVec2(const ShaderHandle& shader, ccharptr location, const glm::vec2& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform2fv(loc, 1, glm::value_ptr(vec));

	}



	///////////////////////////////////////////////////
	/// Textures
	///////////////////////////////////////////////////

	TextureHandle GLDevice::CreateTexture2D(const TextureDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mTextures.dense_size() >= skMaxTextures,
			"Max textures reached",
			TextureHandle{}
		);

		Texture texture;
		TextureData data = AssetService::load_texture(desc.filename);

		uint32 width = (desc.width == 0) ? data.width : desc.width;
		uint32 height = (desc.height == 0) ? data.height : desc.height;

		texture.height = height;
		texture.width = width;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.glHandle);

		int mipmaps = mipmap_lvls(width, height);

		glTextureStorage2D(
			texture.handle.glHandle,
			mipmaps,
			GL_RGBA8,
			width,
			height
		);

		glTextureSubImage2D(
			texture.handle.glHandle,
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
			glGenerateTextureMipmap(texture.handle.glHandle);

		return mTextures.create_handle(std::move(texture));
	}
	// TO IMPLEMENT:
	TextureHandle GLDevice::CreateTexture3D(const TextureDescriptor& desc) {

		Texture texture;
		return mTextures.create_handle(std::move(texture));
	}
	void GLDevice::DeleteTexture(TextureHandle& texture) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.exists(texture), std::format("Texture {} doesn't exist", texture.id));

		glDeleteTextures(1, &mTextures[texture].handle.glHandle);

		mTextures.delete_handle(texture);

	}
	void GLDevice::SetTextureBinding(const TextureHandle& texture, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.exists(texture), std::format("Texture {} doesn't exists", texture.id));

		mTextures[texture].binding = binding;

	}
	void GLDevice::BindTexture(const TextureHandle& texture, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.exists(texture), "Texture doesn't exists");
		LUM_HOTPATH_ASSERT_VOID(
			mTextures[texture].binding == LUM_NULL_BINDING &&
			binding == LUM_NULL_BINDING,
			std::format("Binding has not been given to texture {}", texture.id)
		);

		uint16 bind = (binding == LUM_NULL_BINDING) ? mTextures[texture].binding : binding;

		glBindTextureUnit(bind, mTextures[texture].handle.glHandle);

	}



	///////////////////////////////////////////////////
	/// Samplers
	///////////////////////////////////////////////////

	SamplerHandle GLDevice::CreateSampler(const SamplerDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mSamplers.dense_size() >= skMaxSamplers,
			"Max samplers reached",
			SamplerHandle{}
		);

		Sampler sampler;

		glCreateSamplers(1, &sampler.handle);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MAG_FILTER, (desc.magFilter == SamplerMagFilter::Nearest) ? GL_NEAREST : GL_LINEAR);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MIN_FILTER, skTextureMinFilterLookup[lookup_cast(desc.minFilter)]);

		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_S, skSamplerWrapLookup[lookup_cast(desc.wrapS)]);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_T, skSamplerWrapLookup[lookup_cast(desc.wrapT)]);

		GLfloat max_anisotropy = 1.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
		GLfloat final_anisotropy = std::clamp((float32)desc.anisotropy, 1.0f, (float32)max_anisotropy);
		glSamplerParameterf(sampler.handle, GL_TEXTURE_MAX_ANISOTROPY, final_anisotropy);

		return mSamplers.create_handle(std::move(sampler));
	}
	// TO IMPLEMENT:
	void GLDevice::SetSamplerBinding(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		mSamplers[sampler].binding = binding;

	}
	void GLDevice::BindSampler(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));
		LUM_HOTPATH_ASSERT_VOID(
			mSamplers[sampler].binding == LUM_NULL_BINDING &&
			binding == LUM_NULL_BINDING,
			std::format("Binding has not been given to sampler {}", sampler.id)
		);

		uint16_t bind = (binding == LUM_NULL_BINDING) ? mSamplers[sampler].binding : binding;

		glBindSampler(bind, mSamplers[sampler].handle);


	}
	void GLDevice::DeleteSampler(SamplerHandle sampler) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		glDeleteSamplers(1, &mSamplers[sampler].handle);

		mSamplers.delete_handle(sampler);
	}




	///////////////////////////////////////////////////
	/// Pipelines
	///////////////////////////////////////////////////

	PipelineHandle GLDevice::CreatePipeline(const PipelineDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mPipelines.dense_size() >= skMaxPipelines,
			"Max pipelines reached",
			PipelineHandle{}
		);

		Pipeline pipeline;
		std::memcpy(&pipeline, &desc, sizeof(desc));

		return mPipelines.create_handle(std::move(pipeline));
	}
	void GLDevice::DeletePipeline(PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.exists(pipeline), "Pipeline doesn't exist");

		mPipelines.delete_handle(pipeline);
	}
	void GLDevice::BindPipeline(const PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.exists(pipeline), "Pipeline doesn't exist");

		if (pipeline.id != mCurrentPipeline.id && pipeline.generation != mCurrentPipeline.generation) return;
		mCurrentPipeline = pipeline;

		Pipeline& pip = mPipelines[pipeline];

		// Topology
		_BindCheckRasterizer(pip);

		// Depth & Stencil
		_BindCheckDepthStencil(pip);

		// Scissors
		_BindCheckScissors(pip);

		// Cull
		_BindCheckCull(pip);

		// Blend
		_BindCheckBlend(pip);

	}




	///////////////////////////////////////////////////
	/// Other
	///////////////////////////////////////////////////

	void GLDevice::Draw(const VertexLayoutHandle& vao, uint32 vertex_count) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	}
	void GLDevice::DrawElements(const VertexLayoutHandle& vao, uint32 indices_count) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, nullptr);

	}
	void GLDevice::BeginFrame() {

		glViewport(0, 0, window->GetWidth(), window->GetHeight());
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#		if LUM_ENABLE_IMGUI == 1
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#		endif

		glfwPollEvents();

	}
	void GLDevice::EndFrame() {

#		if LUM_ENABLE_IMGUI == 1
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


#		endif

		glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeWindow()));

	}

	// FINISHED
	void GLDevice::EnableScissors(bool enable) {

		if (enable == mEnabledStates.has(State::Scissor))
			return;

		if (enable) {

			glEnable(GL_SCISSOR_TEST);
			mEnabledStates.enable(State::Scissor);

		}
		else {

			glDisable(GL_SCISSOR_TEST);
			mEnabledStates.disable(State::Scissor);

		}


	}

	// FINISHED
	void GLDevice::SetScissor(int32 x, int32 y, int32 width, int32 height) {
		LUM_HOTPATH_ASSERT_VOID(!mEnabledStates.has(State::Scissor), "Scissor state not enabled");

		if (mScissorState.x == x && mScissorState.y == y && mScissorState.width == width && mScissorState.height == height) 
			return;

		mScissorState.x = x;
		mScissorState.y = y;
		mScissorState.width = width;
		mScissorState.height = height;

		glScissor(x, y, width, height);

	}

	// FINISHED
	void GLDevice::SetViewport(int32 x, int32 y, int32 width, int32 height) {

		glViewport(x, y, width, height);

	}


	// Cull setters (FINISHED)

	// FINISHED
	void GLDevice::EnableCull(bool enable) {

		if (enable == mEnabledStates.has(State::Cull))
			return;

		if (enable) {
			glEnable(GL_CULL_FACE);
			mEnabledStates.enable(State::Cull);
		}
		else {
			glDisable(GL_CULL_FACE);
			mEnabledStates.disable(State::Cull);
		}


	}

	// FINISHED
	void GLDevice::SetCullFace(Face face) {
		LUM_HOTPATH_ASSERT_VOID(!mEnabledStates.has(State::Cull), "Cull not enabled");

		if (face == mCullState.face)
			return;

		glCullFace(skFacesLookup[lookup_cast(face)]);
		mEnabledStates.enable(State::Cull);

		mCullState.face = face;

	}

	// FINISHED
	void GLDevice::SetCullWindingOrder(WindingOrder order) {
		LUM_HOTPATH_ASSERT_VOID(!mEnabledStates.has(State::Cull), "Cull not enabled");

		if (order == mCullState.windingOrder)
			return;

		glFrontFace((order == WindingOrder::Clockwise) ? GL_CW : GL_CCW);

		mCullState.windingOrder = order;

	}

	
	// Blend setters

	// FINISHED
	void GLDevice::EnableBlend(bool enable) {

		if (enable == mEnabledStates.has(State::Blend))
			return;

		if (enable) {
			glEnable(GL_BLEND);
			mEnabledStates.enable(State::Blend);
		}
		else {
			glDisable(GL_BLEND);
			mEnabledStates.disable(State::Blend);
		}

	}

	//virtual void SetBlendConstantColor(glm::vec4 rgba) = 0; IMPLEMENT
	
	// FINISHED
	void GLDevice::SetBlendFactors(BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha) {
		LUM_HOTPATH_ASSERT_VOID(!mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.srcColorFactor == srcColor && mBlendState.dstColorFactor == dstColor && 
			mBlendState.srcAlphaFactor == srcAlpha && mBlendState.dstAlphaFactor == dstAlpha)
			return;

		glBlendFuncSeparate(
			skBlendFactorLookup[lookup_cast(srcColor)],
			skBlendFactorLookup[lookup_cast(dstColor)],
			skBlendFactorLookup[lookup_cast(srcAlpha)],
			skBlendFactorLookup[lookup_cast(dstAlpha)]
		);

		mBlendState.srcColorFactor = srcColor;
		mBlendState.dstColorFactor = dstColor;
		mBlendState.srcAlphaFactor = srcAlpha;
		mBlendState.dstAlphaFactor = dstAlpha;

	}

	// FINISHED
	void GLDevice::SetBlendOp(BlendOp colorOp, BlendOp alphaOp) {
		LUM_HOTPATH_ASSERT_VOID(!mEnabledStates.has(State::Blend), "Blend not enabled");

		if (mBlendState.alphaOp == alphaOp && mBlendState.colorOp == colorOp)
			return;

		glBlendEquationSeparate(
			skBlendOpLookup[lookup_cast(colorOp)],
			skBlendOpLookup[lookup_cast(alphaOp)]
		);

	}


	// Depth setters

	// FINISHED
	void GLDevice::EnableDepthWrite(bool enable) {

		if (enable == mEnabledStates.has(State::DepthWrite))
			return;

		if (enable) {

			glDepthMask((GLboolean)enable);
			mEnabledStates.enable(State::DepthWrite);

		}
		else {

			glDepthMask((GLboolean)enable);
			mEnabledStates.disable(State::DepthWrite);

		}

	}

	// FINISHED
	void GLDevice::EnableDepthTest(bool enable) {

		if (enable == mEnabledStates.has(State::DepthTest))
			return;

		if (enable) {

			glEnable(GL_DEPTH_TEST);
			mEnabledStates.enable(State::DepthTest);

		}
		else {

			glDisable(GL_DEPTH_TEST);
			mEnabledStates.disable(State::DepthTest);

		}

	}

	// FINISHED
	void GLDevice::SetDepthFunc(CompareFlag func) {
		LUM_HOTPATH_ASSERT_VOID(!mEnabledStates.has(State::DepthTest), "Depth not enabled");

		if (func == mDepthStencilState.depth.compareFlag)
			return;

		glDepthFunc(skCompareFlagLookup[lookup_cast(func)]);

	}

	// FINISHED
	void GLDevice::EnableStencilTest(bool enable) {

		if (enable == mEnabledStates.has(State::StencilTest))
			return;

		if (enable) {

			glEnable(GL_STENCIL_TEST);
			mEnabledStates.enable(State::StencilTest);

		}
		else {

			glDisable(GL_STENCIL_TEST);
			mEnabledStates.disable(State::StencilTest);

		}


	}


	void GLDevice::SetStencilReference(int32 ref, Face face = Face::FrontBack) {


	}


	// Rasterizer setters

	// FINISHED
	void GLDevice::EnableDepthBias(bool enable) {

		if (enable == mEnabledStates.has(State::DepthBias))
			return;

		if (enable) {

			glEnable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.enable(State::DepthBias);

		}
		else {

			glDisable(GL_POLYGON_OFFSET_FILL);
			mEnabledStates.disable(State::DepthBias);

		}

	}

	// FINISHED
	void GLDevice::SetDepthBias(float32 slopFactor, float32 constantBias) {
		LUM_HOTPATH_ASSERT(!mEnabledStates.has(State::DepthBias), "Depth bias not enabled");

		if (slopFactor == mRasterizerState.depthBias.slopeFactor && constantBias == mRasterizerState.depthBias.constantBias)
			return;

		if (slopFactor == max_val<float32>())
			slopFactor = mRasterizerState.depthBias.slopeFactor;

		if (constantBias == max_val<float32>())
			constantBias = mRasterizerState.depthBias.constantBias;

		glPolygonOffset(slopFactor, constantBias);
		mRasterizerState.depthBias.slopeFactor = slopFactor;
		mRasterizerState.depthBias.constantBias = constantBias;

	}


	///////////////////////////////////////////////////
	/// Private helpers
	///////////////////////////////////////////////////

	void GLDevice::_BindCheckRasterizer(const Pipeline& pip) noexcept {
		const auto& rast = pip.rasterizer;

		if (rast.topologyMode != mRasterizerState.topologyMode || rast.topologyModeFaces != mRasterizerState.topologyModeFaces) {
			glPolygonMode(
				skFacesLookup[lookup_cast(rast.topologyModeFaces)],
				skPolygonModeLookup[lookup_cast(rast.topologyMode)]
			);
			mRasterizerState = rast;
		}

		EnableDepthBias(rast.depthBias.bEnable);

		if (rast.depthBias.bEnable) {

			SetDepthBias(rast.depthBias.slopeFactor, rast.depthBias.constantBias);

		}

	}
	void GLDevice::_BindCheckDepthStencil(const Pipeline& pip) noexcept {

		const auto& depth = pip.depthStencil.depth;
		const auto& stencil = pip.depthStencil.stencil;

		EnableDepthTest(depth.bEnabled);
		
		if (depth.bEnabled) {

			SetDepthFunc(depth.compareFlag);

		}
		

		if (stencil.bEnabled != mDepthStencilState.stencil.bEnabled) {

			if (stencil.bEnabled) {

				glEnable(GL_STENCIL_TEST);
				mEnabledStates.enable(State::StencilTest);

			}
			else {

				glDisable(GL_STENCIL_TEST);
				mEnabledStates.disable(State::StencilTest);

			}


		}

	}
	void GLDevice::_BindCheckScissors(const Pipeline& pip) noexcept {

		const auto& scissors = pip.scissor;

		if (scissors.bEnabled != mEnabledStates.has(State::Scissor)) {

			if (scissors.bEnabled) {

				glEnable(GL_SCISSOR_TEST);
				mEnabledStates.enable(State::Scissor);

				SetScissor(scissors.x, scissors.y, scissors.width, scissors.height); // Default options

			}
			else {

				glDisable(GL_SCISSOR_TEST);
				mEnabledStates.disable(State::Scissor);

			}
		}

	}
	void GLDevice::_BindCheckBlend(const Pipeline& pip) noexcept {

		const auto& blend = pip.blend;

		EnableBlend(blend.bEnabled);

		if (blend.bEnabled) {

			SetBlendFactors(blend.srcColorFactor, blend.dstColorFactor, blend.srcAlphaFactor, blend.dstAlphaFactor);
			SetBlendOp(blend.colorOp, blend.alphaOp);

		}

	}
	void GLDevice::_BindCheckCull(const Pipeline& pip) noexcept {

		const auto& cull = pip.cull;

		EnableCull(cull.bEnabled);

		if (cull.bEnabled) {
	
			SetCullFace(cull.face);
			SetCullWindingOrder(cull.windingOrder);

		}

	}

	// TO IMPLEMENT:
	void GLDevice::_CacheUniformsLocations() {




	}
	bool GLDevice::_IsValidBufferDescriptor(const BufferDescriptor& desc) noexcept {

		if (desc.bufferUsage == BufferUsage::Static) {

			if ((desc.mapFlags & ~(Mapflag::Coherent | Mapflag::Read)) != 0) {
				LUM_LOG_ERROR("Invalid buffer descriptor");
				return false;
			}

		}
		else if ((desc.mapFlags &Mapflag::Coherent) && !(desc.mapFlags & Mapflag::Persistent)) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}
	GLbitfield GLDevice::_TranslateMappingFlags(Mapflag flags) noexcept {
		GLbitfield flag = 0;

		if (flags & Mapflag::Persistent)			flag |= GL_MAP_PERSISTENT_BIT;
		if (flags & Mapflag::Write)					flag |= GL_MAP_WRITE_BIT;
		if (flags & Mapflag::Read)					flag |= GL_MAP_READ_BIT;
		if (flags & Mapflag::Coherent)				flag |= GL_MAP_COHERENT_BIT;
		if (flags & Mapflag::Invalidate_Buffer)		flag |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if (flags & Mapflag::Invalidate_Range)		flag |= GL_MAP_INVALIDATE_RANGE_BIT;
		if (flags & Mapflag::Unsynchronized)		flag |= GL_MAP_UNSYNCHRONIZED_BIT;

		return flag;
	}

}