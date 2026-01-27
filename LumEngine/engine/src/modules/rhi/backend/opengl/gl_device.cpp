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
namespace lum::gl {

	///////////////////////////////////////////////////
	/// Buffers
	///////////////////////////////////////////////////

	rhi::BufferHandle GL_Device::create_vertex_buffer(const BufferDescriptor& desc) {
		if (!_is_valid_buffer_descriptor(desc))
			return rhi::BufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return rhi::BufferHandle{};
		}

		rhi::Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.mapFlags;
		buffer.type = rhi::BufferType::Vertex;
		buffer.usage = desc.bufferUsage;

		GLbitfield init_flags =
			((desc.bufferUsage == rhi::BufferUsage::Dynamic) ? GL_DYNAMIC_STORAGE_BIT : 0)
			| _translate_mapping_flags(desc.mapFlags);

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
	rhi::BufferHandle GL_Device::create_element_buffer(const BufferDescriptor& desc) {
		if (!_is_valid_buffer_descriptor(desc))
			return rhi::BufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return rhi::BufferHandle{};
		}

		rhi::Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.mapFlags;
		buffer.type = rhi::BufferType::Element;
		buffer.usage = desc.bufferUsage;

		GLbitfield init_flags =
			((buffer.usage == rhi::BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| _translate_mapping_flags(desc.mapFlags);

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
	rhi::BufferHandle GL_Device::create_uniform_buffer(const BufferDescriptor& desc) {
		if (!_is_valid_buffer_descriptor(desc))
			return rhi::BufferHandle{};

		if (mBuffers.dense_size() >= skMaxBuffers) {
			LUM_LOG_ERROR("Max buffers reached");
			return rhi::BufferHandle{};
		}

		if (desc.size <= 0) {
			LUM_LOG_WARN("Invalid buffer size");
			return rhi::BufferHandle{};
		}

		rhi::Buffer buffer;
		buffer.size = desc.size;
		buffer.flags = desc.mapFlags;
		buffer.type = rhi::BufferType::Uniform;
		buffer.usage = desc.bufferUsage;

		GLbitfield init_flags =
			((buffer.usage == rhi::BufferUsage::Static) ? 0 : GL_DYNAMIC_STORAGE_BIT)
			| _translate_mapping_flags(desc.mapFlags);

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
	void GL_Device::update_buffer(const BufferHandle& vbo, cvptr data, usize offset, usize size) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(vbo), "Buffer does not exist");

		rhi::Buffer& buffer = mBuffers[vbo];

		LUM_HOTPATH_ASSERT_VOID(offset + size > buffer.size, "Invalid offset or size");
		if (size == 0) size = buffer.size;
		LUM_HOTPATH_ASSERT_VOID(
			buffer.usage == rhi::BufferUsage::Static,
			std::format("Buffer {} is static, cannot be updated", buffer.handle.glHandle)
		);
		LUM_HOTPATH_ASSERT_VOID(
			!(buffer.flags & rhi::Mapflag::Write),
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
	void GL_Device::delete_buffer(BufferHandle& vbo) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(vbo), "Buffer doesn't exist");

		auto& buffer = mBuffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(buffer.mapped, "Unable to delete buffer - still mapped");
		glDeleteBuffers(1, &buffer.handle.glHandle);
		mBuffers.delete_handle(vbo);

		LUM_LOG_INFO(std::format("Deleted buffer {}", buffer.handle.glHandle));

	}
	vptr GL_Device::map_buffer(const BufferHandle& vbo, Mapflag flags, usize offset, usize size) {

		LUM_HOTPATH_ASSERT_NULLPTR(!mBuffers.exists(vbo), "Handle doesn't exist");

		rhi::Buffer& buffer = mBuffers[vbo];

		LUM_HOTPATH_ASSERT_NULLPTR(offset + size > buffer.size || size > buffer.size, "Invalid offset or size");
		if (size <= 0) size = buffer.size;

		vptr ptr = glMapNamedBufferRange(buffer.handle.glHandle, offset, size, _translate_mapping_flags(flags));

		LUM_HOTPATH_ASSERT_NULLPTR(!ptr, "Error during mapping");

		LUM_LOG_DEBUG(std::format("Mapped buffer {}", buffer.handle.glHandle));
		return ptr;
	}
	void GL_Device::unmap_buffer(const BufferHandle& vbo) {

		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(vbo), "Handle does not exist");

		rhi::Buffer& buffer = mBuffers[vbo];
		LUM_HOTPATH_ASSERT_VOID(!buffer.mapped, "Buffer is already unmapped");
		glUnmapNamedBuffer(buffer.handle.glHandle);

		LUM_LOG_DEBUG(std::format("Unmapped buffer {}", buffer.handle.glHandle));
	}
	void GL_Device::attach_element_buffer_to_layout(const BufferHandle& ebo, const VertexLayoutHandle& vao) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.exists(vao), "Layout doesn't exists");
		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(ebo), "Buffer doesn't exists");

		glVertexArrayElementBuffer(mLayouts[vao].vao, mBuffers[ebo].handle.glHandle);

	}
	void GL_Device::set_uniform_buffer_binding(const BufferHandle& ubo, int32 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mBuffers.exists(ubo), "Uniform buffer doesn't exists");

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mBuffers[ubo].handle.glHandle);

	}



	///////////////////////////////////////////////////
	/// Framebuffers
	///////////////////////////////////////////////////

	rhi::FramebufferHandle GL_Device::create_framebuffer() {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mFramebuffers.dense_size() >= skMaxFramebuffers,
			"Max framebuffers reached",
			FramebufferHandle{}
		);

		Framebuffer fbo;

		glCreateFramebuffers(1, &fbo.handle);

		return mFramebuffers.create_handle(std::move(fbo));

	}
	rhi::TextureHandle GL_Device::create_framebuffer_texture(const FramebufferTextureDescriptor& desc) {
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
	void GL_Device::set_framebuffer_color_texture(const FramebufferHandle& fbo, const TextureHandle& tex, uint8 index) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(fbo) || !mTextures.exists(tex), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTextures[tex].handle.glHandle, 0);

	}
	void GL_Device::set_framebuffer_depth_texture(const FramebufferHandle& fbo, const TextureHandle& tex) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(fbo), "Framebuffer doesn't exists");

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextures[tex].handle.glHandle, 0);

	}
	void GL_Device::set_framebuffer_stencil_texture(const FramebufferHandle& fbo, const TextureHandle& tex) {

	}
	void GL_Device::clear_framebuffer(FramebufferHandle fbo, glm::vec4 color, float32 depth) {
		bind_framebuffer(fbo);
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(std::clamp(depth, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void GL_Device::delete_framebuffer(FramebufferHandle& buff) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(buff), "Framebuffer doesn't exists");

		Framebuffer& fbo = mFramebuffers[buff];
		glDeleteFramebuffers(1, &fbo.handle);

		mFramebuffers.delete_handle(buff);
	}
	void GL_Device::bind_framebuffer(const FramebufferHandle& buff) {
		LUM_HOTPATH_ASSERT_VOID(!mFramebuffers.exists(buff), "Framebuffer doesn't exists");

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffers[buff].handle);

	}
	void GL_Device::unbind_framebuffer() {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}




	///////////////////////////////////////////////////
	/// Layouts
	///////////////////////////////////////////////////

	rhi::VertexLayoutHandle GL_Device::create_vertex_layout(const VertexLayoutDescriptor& desc, const BufferHandle& vbo) {

		LUM_HOTPATH_ASSERT_CUSTOM(!mBuffers.exists(vbo), "Buffer doesn't exists", rhi::VertexLayoutHandle{});
		LUM_HOTPATH_ASSERT_CUSTOM(desc.attributes.size() <= 0, "VertexLayout has no attributes", rhi::VertexLayoutHandle{});

		rhi::VertexLayout layout;
		rhi::Buffer& buffer = mBuffers[vbo];

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
				rhi::detail::skDataFormatLookup[static_cast<byte>(desc.attributes[i].format)],
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
	void GL_Device::delete_vertex_layout(VertexLayoutHandle& layout) {


	}



	///////////////////////////////////////////////////
	/// Shaders
	///////////////////////////////////////////////////

	rhi::ShaderHandle GL_Device::create_shader(const ShaderDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mShaders.dense_size() >= skMaxShaders,
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

		return mShaders.create_handle(std::move(shader));
	}
	void GL_Device::bind_shader(const ShaderHandle& shader) {
		LUM_HOTPATH_ASSERT_VOID(!mShaders.exists(shader), "Shader doesn't exists");

		glUseProgram(mShaders[shader].handle);

	}
	void GL_Device::delete_shader(ShaderHandle& shader) {

	}
	void GL_Device::set_mat4(const ShaderHandle& shader, ccharptr location, const glm::mat4& mat) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

	}
	void GL_Device::set_f(const ShaderHandle& shader, ccharptr location, float32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1f(loc, value);

	}
	void GL_Device::set_i(const ShaderHandle& shader, ccharptr location, int32 value) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform1i(loc, value);

	}
	void GL_Device::set_vec4(const ShaderHandle& shader, ccharptr location, const glm::vec4& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform4fv(loc, 1, glm::value_ptr(vec));

	}
	void GL_Device::set_vec3(const ShaderHandle& shader, ccharptr location, const glm::vec3& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform3fv(loc, 1, glm::value_ptr(vec));

	}
	void GL_Device::set_vec2(const ShaderHandle& shader, ccharptr location, const glm::vec2& vec) {

		GLuint loc = glGetUniformLocation(mShaders[shader].handle, location);
		LUM_HOTPATH_ASSERT_VOID(loc == -1, std::format("Couldn't localize uniform named {}", location));
		glUniform2fv(loc, 1, glm::value_ptr(vec));

	}



	///////////////////////////////////////////////////
	/// Textures
	///////////////////////////////////////////////////

	rhi::TextureHandle GL_Device::create_texture_2d(const TextureDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mTextures.dense_size() >= skMaxTextures,
			"Max textures reached",
			rhi::TextureHandle{}
		);

		Texture texture;
		TextureData data = AssetService::load_texture(desc.filename);

		uint32 width = (desc.width == 0) ? data.width : desc.width;
		uint32 height = (desc.height == 0) ? data.height : desc.height;

		texture.height = height;
		texture.width = width;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture.handle.glHandle);

		int mipmaps = rhi::mipmap_lvls(width, height);

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
	rhi::TextureHandle GL_Device::create_texture_3d(const TextureDescriptor& desc) {

		rhi::Texture texture;
		return mTextures.create_handle(std::move(texture));
	}
	void GL_Device::delete_texture(TextureHandle& texture) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.exists(texture), std::format("Texture {} doesn't exist", texture.id));

		glDeleteTextures(1, &mTextures[texture].handle.glHandle);

		mTextures.delete_handle(texture);

	}
	void GL_Device::set_texture_binding(const TextureHandle& texture, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mTextures.exists(texture), std::format("Texture {} doesn't exists", texture.id));

		mTextures[texture].binding = binding;

	}
	void GL_Device::bind_texture(const TextureHandle& texture, uint16 binding) {
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

	rhi::SamplerHandle GL_Device::create_sampler(const SamplerDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mSamplers.dense_size() >= skMaxSamplers,
			"Max samplers reached",
			rhi::SamplerHandle{}
		);

		Sampler sampler;

		glCreateSamplers(1, &sampler.handle);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MAG_FILTER, (desc.magFilter == rhi::SamplerMagFilter::Nearest) ? GL_NEAREST : GL_LINEAR);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_MIN_FILTER, skTextureMinFilterLookup[static_cast<byte>(desc.minFilter)]);

		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_S, skSamplerWrapLookup[static_cast<byte>(desc.wrapS)]);
		glSamplerParameteri(sampler.handle, GL_TEXTURE_WRAP_T, skSamplerWrapLookup[static_cast<byte>(desc.wrapT)]);

		GLfloat max_anisotropy = 1.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
		GLfloat final_anisotropy = std::clamp((float32)desc.anisotropy, 1.0f, (float32)max_anisotropy);
		glSamplerParameterf(sampler.handle, GL_TEXTURE_MAX_ANISOTROPY, final_anisotropy);

		return mSamplers.create_handle(std::move(sampler));
	}
	// TO IMPLEMENT:
	void GL_Device::set_sampler_binding(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		mSamplers[sampler].binding = binding;

	}
	void GL_Device::bind_sampler(const SamplerHandle& sampler, uint16 binding) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));
		LUM_HOTPATH_ASSERT_VOID(
			mSamplers[sampler].binding == LUM_NULL_BINDING &&
			binding == LUM_NULL_BINDING,
			std::format("Binding has not been given to sampler {}", sampler.id)
		);

		uint16_t bind = (binding == LUM_NULL_BINDING) ? mSamplers[sampler].binding : binding;

		glBindSampler(bind, mSamplers[sampler].handle);


	}
	void GL_Device::delete_sampler(SamplerHandle sampler) {
		LUM_HOTPATH_ASSERT_VOID(!mSamplers.exists(sampler), std::format("Sampler {} doesn't exists", sampler.id));

		glDeleteSamplers(1, &mSamplers[sampler].handle);

		mSamplers.delete_handle(sampler);
	}




	///////////////////////////////////////////////////
	/// Pipelines
	///////////////////////////////////////////////////

	rhi::PipelineHandle GL_Device::create_pipeline(const PipelineDescriptor& desc) {
		LUM_HOTPATH_ASSERT_CUSTOM(
			mPipelines.dense_size() >= skMaxPipelines,
			"Max pipelines reached",
			PipelineHandle{}
		);

		Pipeline pipeline;
		std::memcpy(&pipeline, &desc, sizeof(desc));

		return mPipelines.create_handle(std::move(pipeline));
	}
	void GL_Device::delete_pipeline(PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.exists(pipeline), "Pipeline doesn't exist");

		mPipelines.delete_handle(pipeline);
	}
	void GL_Device::bind_pipeline(const PipelineHandle& pipeline) {
		LUM_HOTPATH_ASSERT_VOID(!mPipelines.exists(pipeline), "Pipeline doesn't exist");

		Pipeline& pip = mPipelines[pipeline];

		// !!!!!! CREATE OTHER FUNCTIONS AND SEPARATE USING Pipeline*
		
		// Topology

		const auto& rast = pip.rasterizer;

		if (rast.topologyMode != mRasterizerState.topologyMode || rast.topologyModeFaces != mRasterizerState.topologyModeFaces) {
			glPolygonMode(
				skFacesLookup[static_cast<byte>(rast.topologyModeFaces)],
				skPolygonModeLookup[static_cast<byte>(rast.topologyMode)]
			);
			mRasterizerState = rast;
		}
		if (rast.bEnableDepthBias) {



			mRasterizerState = rast;
		}


		// Depth & Stencil

		const auto& depth = pip.depthStencil.depth;
		const auto& stencil = pip.depthStencil.stencil;

		if (depth.bEnabled != mDepthStencilState.depth.bEnabled) {

			if (depth.bEnabled) {

				glEnable(GL_DEPTH_TEST);
				mEnabledStates.enable(State::DepthTest);

			}
			else {

				glDisable(GL_DEPTH_TEST);
				mEnabledStates.disable(State::DepthTest);

			}

		}

		if (depth.bEnabled) {

			if (depth.compareFlag != mDepthStencilState.depth.compareFlag) {

				glDepthFunc(skCompareFlagLookup[static_cast<bitfield>(depth.compareFlag)]);

			}

			if (depth.bWriteToZBuffer != mDepthStencilState.depth.bWriteToZBuffer) {

				glDepthMask((pip.depthStencil.depth.bWriteToZBuffer) ? GL_TRUE : GL_FALSE);

			}

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
		if (stencil.back != mDepthStencilState.stencil.back || stencil.front != mDepthStencilState.stencil.front) {
			
		}


		// Scissors

		const auto& scissors = pip.scissor;
		bool scissorsEnabled = mEnabledStates.has(State::Scissor);

		if (scissors.bEnabled != mScissorState.bEnabled) {

			if (scissors.bEnabled) {

				glEnable(GL_SCISSOR_TEST);
				mEnabledStates.enable(State::Scissor);

			}
			else {

				glDisable(GL_SCISSOR_TEST);
				mEnabledStates.disable(State::Scissor);

			}
		}
		if (scissors.bEnabled && (scissors != mScissorState)) {

			glScissor(scissors.x, scissors.y, scissors.width, scissors.height);
			mScissorState = scissors;

		}


		// Cull

		const auto& cull = pip.cull;
		bool cullEnabled = mEnabledStates.has(State::CullFace);

		if (cull.bEnabled != cullEnabled) {
			if (cull.bEnabled) {

				glEnable(GL_CULL_FACE);
				mEnabledStates.enable(State::CullFace);

			}
			else {

				glDisable(GL_CULL_FACE);
				mEnabledStates.disable(State::CullFace);

			}
		}
		if (cull.bEnabled) {

			if (cull.face != mCullState.face) {
				glCullFace(skFacesLookup[static_cast<byte>(cull.face)]);
			}

			if (cull.windingOrder != mCullState.windingOrder) {
				glFrontFace(cull.windingOrder == rhi::WindingOrder::CounterClockwise ? GL_CCW : GL_CW);
			}

			mCullState = cull;

		}

	}




	///////////////////////////////////////////////////
	/// Other
	///////////////////////////////////////////////////

	void GL_Device::draw(const VertexLayoutHandle& vao, uint32 vertex_count) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	}
	void GL_Device::draw_elements(const VertexLayoutHandle& vao, uint32 indices_count) {
		LUM_HOTPATH_ASSERT_VOID(!mLayouts.exists(vao), "Cannot draw, invalid vertex layout");

		glBindVertexArray(mLayouts[vao].vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, nullptr);

	}
	void GL_Device::begin_frame() {

		glViewport(0, 0, window->GetWidth(), window->GetHeight());
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#		if LUM_ENABLE_IMGUI == 1
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#		endif

		glfwPollEvents();

	}
	void GL_Device::end_frame() {

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
	void GL_Device::_cache_uniforms_locations() {




	}
	bool GL_Device::_is_valid_buffer_descriptor(const rhi::BufferDescriptor& desc) noexcept {

		if (desc.bufferUsage == rhi::BufferUsage::Static) {

			if ((desc.mapFlags & ~(rhi::Mapflag::Coherent | rhi::Mapflag::Read)) != 0) {
				LUM_LOG_ERROR("Invalid buffer descriptor");
				return false;
			}

		}
		else if ((desc.mapFlags & rhi::Mapflag::Coherent) && !(desc.mapFlags & rhi::Mapflag::Persistent)) {
			LUM_LOG_ERROR("Invalid buffer descriptor");
			return false;
		}

		return true;

	}
	GLbitfield	GL_Device::_translate_mapping_flags(Mapflag flags) noexcept {
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

}