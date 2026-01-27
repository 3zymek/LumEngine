#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "rhi/core/rhi_shader.hpp"
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_texture.hpp"
#include "rhi/core/rhi_sampler.hpp"
#include "core/shaders_define.h"
#include "rhi/core/rhi_framebuffer.hpp"
#include "rhi/core/rhi_pipeline.hpp"
#include "core/flags.hpp"
namespace lum { class Window; }
namespace lum::rhi {

	/*!
	*  @brief Low-level GPU interface for resource creation and rendering.
	*
	*  @brief RHI_Device provides functions to create, bind, and manage GPU resources
	*  such as textures, samplers, shaders, and buffers. It also handles
	*  binding resources to the pipeline for drawing or compute operations.
	*
	*  @brief All operations are GPU-aware and should be used according to the
	*  current command buffer / pipeline state.
	*
	*  @brief Typical usage:
	*  @brief  - Create resources (textures, shaders, buffers)
	*  @brief  - Bind resources (shaders, textures, samplers)
	*  @brief  - Update uniforms or descriptors
	*  @brief  - Issue draw/dispatch commands
	*/
	class RHI_Device {
	public:

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		/*! @brief Creates Vertex buffer ( VBO ).
		* 
		*  @param desc BufferDescriptor of VBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle create_vertex_buffer( const BufferDescriptor& desc ) = 0;
		
		/*! @brief Creates Element buffer ( EBO ).
		*
		*  @param desc BufferDescriptor of EBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle create_element_buffer( const BufferDescriptor& desc ) = 0;
		
		/*! @brief Creates Uniform buffer ( UBO ).
		*
		*  @param desc BufferDescriptor of UBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle create_uniform_buffer( const BufferDescriptor& desc ) = 0;

		/*! @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer ( 0 for whole buffer ).
		*  @param size Byte size of data to write ( 0 for whole buffer ).
		*
		*/
		virtual void update_buffer( const BufferHandle& buff, cvptr data, usize offset = 0, usize size = 0 ) = 0;
		
		/*! @brief Deletes buffer.
		*
		*  @param buff Buffer handle to delete.
		*
		*/
		virtual void delete_buffer( BufferHandle& buff ) = 0;
		
		/*! @brief Maps data of given buffer.
		*
		*  @param buff Buffer handle to map.
		*  @param flags Mapping flags ( example: rhi::map_flags::Write ).
		*  @param offset Byte offset from the beginning of the buffer to map ( 0 for whole buffer ).
		*  @param size Byte size of data to map ( 0 for whole buffer ).
		* 
		*  @return GPU data pointer to manage.
		*	
		*/
		LUM_NODISCARD 
		virtual vptr map_buffer( const BufferHandle& buff, Mapflag flags, usize offset = 0, usize size = 0 ) = 0;
		
		/*! @brief Unmaps buffer.
		*
		*  @param buff Buffer handle to unmap.
		*
		*/
		virtual void unmap_buffer( const BufferHandle& buff ) = 0;
		
		/*! @brief Connects element buffer ( EBO ) to vertex layout ( VAO ).
		*
		*  @param ebo Element buffer to connect.
		*  @param vao Vertex layout to connect.
		* 
		*/
		virtual void attach_element_buffer_to_layout( const BufferHandle& ebo, const VertexLayoutHandle& vao ) = 0;
		
		/*! @brief Connects uniform buffer ( UBO ) to binding in shaders.
		*
		*  @param ubo Uniform buffer handle
		*  @param binding Binding in the shader ( example: LUM_UBO_CAMERA )
		*
		*/
		virtual void set_uniform_buffer_binding( const BufferHandle& ubo, int32 binding ) = 0;





		///////////////////////////////////////////////////
		/// Framebuffers
		///////////////////////////////////////////////////

		/*! @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer ( 0 for whole buffer ).
		*  @param size Byte size of data to write ( 0 for whole buffer ).
		*
		*/
		LUM_NODISCARD
		virtual FramebufferHandle create_framebuffer() = 0;

		// TO LOOK:
		virtual TextureHandle create_framebuffer_texture(const FramebufferTextureDescriptor& desc) = 0;
		// TO FIX:
		virtual void set_framebuffer_color_texture(const FramebufferHandle& fbo, const TextureHandle& tex, uint8 index) = 0;
		// TO IMPLEMENT:
		virtual void set_framebuffer_depth_texture(const FramebufferHandle& fbo, const TextureHandle& tex) = 0;
		virtual void set_framebuffer_stencil_texture(const FramebufferHandle& fbo, const TextureHandle& tex) = 0;

		virtual void clear_framebuffer(FramebufferHandle fbo, glm::vec4 color, float32 depth) = 0;
		virtual void delete_framebuffer(FramebufferHandle& fbo) = 0;

		virtual void bind_framebuffer(const FramebufferHandle& fbo) = 0;

		virtual void unbind_framebuffer() = 0;




		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		/*! @brief Creates Vertex layout (VAO).
		*
		*  @param desc Descriptor.
		*  @param vbo Vertex buffer to connect.
		* 
		*  @return Handle to vertex layout.
		*
		*/
		LUM_NODISCARD
		virtual VertexLayoutHandle create_vertex_layout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo ) = 0;
		
		/*! @brief Deletes vertex layout.
		*
		*  @param layout Layout handle to delete.
		*
		*/
		virtual void delete_vertex_layout( VertexLayoutHandle& layout ) = 0;





		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		/*! @brief Creates a shader from a descriptor.
		*
		*  Uses vertex/fragment sources from the descriptor.
		*  @param desc ShaderDescriptor with source file names.
		*  @return Handle to the created shader.
		* 
		*/
		LUM_NODISCARD
		virtual ShaderHandle create_shader( const ShaderDescriptor& desc)	= 0;
		
		/*! @brief Binds a shader for rendering.
		*
		*  The bound shader will be used on next draw/dispatch.
		*  @param shader Shader handle to bind.
		* 
		*/
		virtual void bind_shader( const ShaderHandle& shader ) = 0;
		
		/*! @brief Deletes a shader.
		*
		*  Frees GPU resources and invalidates the handle.
		*  @param shader Shader handle to delete.
		* 
		*/
		virtual void delete_shader( ShaderHandle& shader ) = 0;
		
		/*! @brief Sets a 4x4 matrix uniform in a shader.
		* 
		*  @param shader Shader handle.
		*  @param location Name of the uniform.
		*  @param mat Matrix to set.
		* 
		*/
		virtual void set_mat4( const ShaderHandle& shader, ccharptr location, const glm::mat4& mat ) = 0;
		
		/*!
		* @brief Sets a float uniform in a shader.
		*/
		virtual void set_f( const ShaderHandle& shader, ccharptr location, float32 value )	= 0;
		
		/*!
		* @brief Sets an int uniform in a shader.
		*/
		virtual void set_i( const ShaderHandle& shader, ccharptr location, int32 value )	= 0;
		
		/*!
		* @brief Sets a vec4 uniform in a shader.
		*/
		virtual void set_vec4( const ShaderHandle& shader, ccharptr location, const glm::vec4& vec ) = 0;
		
		/*!
		* @brief Sets a vec3 uniform in a shader.
		*/
		virtual void set_vec3( const ShaderHandle& shader, ccharptr location, const glm::vec3& vec ) = 0;
		
		/*!
		* @brief Sets a vec2 uniform in a shader.
		*/
		virtual void set_vec2( const ShaderHandle& shader, ccharptr location, const glm::vec2& vec ) = 0;




		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		/*! @brief Creates a 2D texture.
		*
		*  Uses the descriptor to initialize GPU texture.
		*  @param desc Texture properties and data.
		*  @return Handle to the created texture.
		* 
		*/
		LUM_NODISCARD 
		virtual TextureHandle create_texture_2d( const TextureDescriptor& desc ) = 0;

		/*! @brief Creates a 3D texture.
		*
		*  Uses the descriptor to initialize GPU 3D texture.
		*  @param desc Texture properties and data.
		*  @return Handle to the created texture.
		* 
		*/
		LUM_NODISCARD 
		virtual TextureHandle create_texture_3d( const TextureDescriptor& desc ) = 0;
		
		/*! @brief Deletes a texture.
		*
		*  Frees GPU memory and invalidates the handle.
		*  @param texture Handle of the texture to delete.
		* 
		*/
		virtual void delete_texture( TextureHandle& texture ) = 0;
		
		/*! @brief Binds a texture to the active slot/unit.
		*
		*  The bound texture will be used by shaders on the next draw/dispatch.
		*  @param texture Texture to bind.
		* 
		*/
		virtual void set_texture_binding( const TextureHandle& texture, uint16 binding ) = 0;
		virtual void bind_texture( const TextureHandle& texture, uint16 binding = LUM_NULL_BINDING ) = 0;





		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////
		
		/*! @brief Creates Sampler.
		*
		*  @param desc SamplerDescriptor of sampler.
		*
		*/
		LUM_NODISCARD
		virtual SamplerHandle create_sampler( const SamplerDescriptor& desc ) = 0;
		
		/*!
		*  @brief Binds a sampler to a GPU slot.
		*
		*  Keeps the binding until changed. Used by shaders on next draw/dispatch.
		*
		*  @param sampler  Sampler handle to bind.
		*  @param binding  GPU binding slot index.
		*/
		virtual void set_sampler_binding( const SamplerHandle& sampler, uint16 binding ) = 0;
		virtual void bind_sampler( const SamplerHandle& sampler, uint16 binding = LUM_NULL_BINDING )	= 0;
		virtual void delete_sampler( SamplerHandle sampler ) = 0;




		///////////////////////////////////////////////////
		/// Pipelines
		///////////////////////////////////////////////////

		virtual PipelineHandle create_pipeline(const PipelineDescriptor& desc) = 0;
		virtual void delete_pipeline(PipelineHandle& pipeline) = 0;
		virtual void bind_pipeline(const PipelineHandle& pipeline) = 0;


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		virtual void draw				( const VertexLayoutHandle& vao, uint32 vertex_count )		= 0;
		virtual void draw_elements		( const VertexLayoutHandle&, uint32 indices_count )			= 0;
		virtual void begin_frame		( )															= 0;
		virtual void end_frame			( )															= 0;


		/*
		virtual void SetBlendConstantColor(glm::vec4 rgba);
		virtual void SetScissor(int32 x, int32 y, int32 width, int32 height);
		virtual void SetViewport(int32 x, int32 y, int32 width, int32 height);
		virutal void SetStencilReference(int32 ref);
		virtual void SetDepthBias(float32 factor, float32 units);
		*/

	protected:

		CullState			mCullState		{};
		ScissorState		mScissorState	{};
		DepthStencilState	mDepthStencilState		{};
		RasterizerState		mRasterizerState{};
		Flags<State>		mEnabledStates	{};
		
		LUM_CONST_VAR_QUALIFIER
		static uint8 skMaxShaders = 8;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxSamplers = 500;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxBuffers = 10000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxLayouts = 10000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxTextures = 1000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxFramebuffers = 100;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxPipelines = 100;

		cstd::handle_pool<Sampler, SamplerHandle>			mSamplers		{ skMaxSamplers };
		cstd::handle_pool<Shader, ShaderHandle>				mShaders		{ skMaxShaders };
		cstd::handle_pool<Buffer, BufferHandle>				mBuffers		{ skMaxBuffers };
		cstd::handle_pool<VertexLayout, VertexLayoutHandle> mLayouts		{ skMaxLayouts };
		cstd::handle_pool<Texture, TextureHandle>			mTextures		{ skMaxTextures };
		cstd::handle_pool<Framebuffer, FramebufferHandle>	mFramebuffers	{ skMaxFramebuffers };
		cstd::handle_pool<Pipeline, PipelineHandle>			mPipelines		{ skMaxPipelines };

	};

	/*! @brief Creates render device.
	*
	*  Initializes and creates render device including used backend (OpenGL for now).
	* 
	*  @param window Pointer to the window.
	*
	*  @return Pointer to the device.
	*
	*/
	RHI_Device* create_device(Window* window);

}