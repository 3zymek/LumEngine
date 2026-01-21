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
		virtual BufferHandle CreateVertexBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Creates Element buffer ( EBO ).
		*
		*  @param desc BufferDescriptor of EBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle CreateElementBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Creates Uniform buffer ( UBO ).
		*
		*  @param desc BufferDescriptor of UBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle CreateUniformBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.  
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer ( 0 for whole buffer ).
		*  @param size Byte size of data to write ( 0 for whole buffer ).
		* 
		*/
		LUM_NODISCARD
		virtual FramebufferHandle CreateFramebuffer(const FramebufferDescriptor& desc) = 0;

		virtual void DeleteFramebuffer(FramebufferHandle& buff) = 0;

		virtual void BindFramebuffer(const FramebufferHandle& buff) = 0;

		virtual void UnbindFramebuffer(const FramebufferHandle& buff) = 0;

		virtual void UpdateBuffer( const BufferHandle& buff, cvptr data, usize offset = 0, usize size = 0 ) = 0;
		/*! @brief Deletes buffer.
		*
		*  @param buff Buffer handle to delete.
		*
		*/
		virtual void DeleteBuffer( BufferHandle& buff ) = 0;
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
		virtual vptr MapBuffer( const BufferHandle& buff, mapflag flags, usize offset = 0, usize size = 0 ) = 0;
		/*! @brief Unmaps buffer.
		*
		*  @param buff Buffer handle to unmap.
		*
		*/
		virtual void UnmapBuffer( const BufferHandle& buff ) = 0;
		/*! @brief Connects element buffer ( EBO ) to vertex layout ( VAO ).
		*
		*  @param ebo Element buffer to connect.
		*  @param vao Vertex layout to connect.
		* 
		*/
		virtual void AttachElementBufferToLayout( const BufferHandle& ebo, const VertexLayoutHandle& vao ) = 0;
		/*! @brief Connects uniform buffer ( UBO ) to binding in shaders.
		*
		*  @param ubo Uniform buffer handle
		*  @param binding Binding in the shader ( example: LUM_UBO_CAMERA )
		*
		*/
		virtual void SetUniformBufferBinding( const BufferHandle& ubo, int32 binding ) = 0;


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
		virtual VertexLayoutHandle CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo ) = 0;
		/*! @brief Deletes vertex layout.
		*
		*  @param layout Layout handle to delete.
		*
		*/
		virtual void DeleteVertexLayout( VertexLayoutHandle& layout ) = 0;



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
		virtual ShaderHandle CreateShader( const ShaderDescriptor& desc)	= 0;
		/*! @brief Binds a shader for rendering.
		*
		*  The bound shader will be used on next draw/dispatch.
		*  @param shader Shader handle to bind.
		* 
		*/
		virtual void BindShader( const ShaderHandle& shader ) = 0;
		/*! @brief Deletes a shader.
		*
		*  Frees GPU resources and invalidates the handle.
		*  @param shader Shader handle to delete.
		* 
		*/
		virtual void DeleteShader( ShaderHandle& shader ) = 0;
		/*! @brief Sets a 4x4 matrix uniform in a shader.
		* 
		*  @param shader Shader handle.
		*  @param location Name of the uniform.
		*  @param mat Matrix to set.
		* 
		*/
		virtual void SetMat4( const ShaderHandle& shader, ccharptr location, const glm::mat4& mat ) = 0;
		/*!
		* @brief Sets a float uniform in a shader.
		*/
		virtual void Setf( const ShaderHandle& shader, ccharptr location, float32 value )	= 0;
		/*!
		* @brief Sets an int uniform in a shader.
		*/
		virtual void Seti( const ShaderHandle& shader, ccharptr location, int32 value )	= 0;
		/*!
		* @brief Sets a vec4 uniform in a shader.
		*/
		virtual void SetVec4( const ShaderHandle& shader, ccharptr location, const glm::vec4& vec ) = 0;
		/*!
		* @brief Sets a vec3 uniform in a shader.
		*/
		virtual void SetVec3( const ShaderHandle& shader, ccharptr location, const glm::vec3& vec ) = 0;
		/*!
		* @brief Sets a vec2 uniform in a shader.
		*/
		virtual void SetVec2( const ShaderHandle& shader, ccharptr location, const glm::vec2& vec ) = 0;

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
		virtual TextureHandle CreateTexture2D( const TextureDescriptor& desc ) = 0;

		/*! @brief Creates a 3D texture.
		*
		*  Uses the descriptor to initialize GPU 3D texture.
		*  @param desc Texture properties and data.
		*  @return Handle to the created texture.
		* 
		*/
		LUM_NODISCARD 
		virtual TextureHandle CreateTexture3D( const TextureDescriptor& desc ) = 0;
		/*! @brief Deletes a texture.
		*
		*  Frees GPU memory and invalidates the handle.
		*  @param texture Handle of the texture to delete.
		* 
		*/
		virtual void DeleteTexture( TextureHandle& texture ) = 0;
		/*! @brief Binds a texture to the active slot/unit.
		*
		*  The bound texture will be used by shaders on the next draw/dispatch.
		*  @param texture Texture to bind.
		* 
		*/
		virtual void SetTextureBinding(const TextureHandle& texture, uint16 binding) = 0;
		virtual void BindTexture( const TextureHandle& texture, uint16 binding = LUM_NULL_BINDING ) = 0;


		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////
		
		/*! @brief Creates Sampler.
		*
		*  @param desc SamplerDescriptor of sampler.
		*
		*/
		LUM_NODISCARD
		virtual SamplerHandle CreateSampler( const SamplerDescriptor& desc ) = 0;
		/*!
		*  @brief Binds a sampler to a GPU slot.
		*
		*  Keeps the binding until changed. Used by shaders on next draw/dispatch.
		*
		*  @param sampler  Sampler handle to bind.
		*  @param binding  GPU binding slot index.
		*/
		virtual void SetSamplerBinding( const SamplerHandle& sampler, uint16 binding ) = 0;
		virtual void BindSampler( const SamplerHandle& sampler, uint16 binding = LUM_NULL_BINDING )	= 0;
		virtual void DeleteSampler( SamplerHandle sampler ) = 0;

		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////

		/*
		virtual PipelineHandle CreatePipeline(const PipelineDescriptor& desc) = 0;
		virtual void DeletePipeline(PipelineHandle& pipeline) = 0;
		
		*/


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		/*
		virtual void EnableState(..);
		virtual void DisableState(...);

		*/

		virtual void Draw			( const VertexLayoutHandle& vao, uint32 vertex_count )	= 0;
		virtual void DrawElements	( const VertexLayoutHandle&, uint32 indices_count )		= 0;
		virtual void BeginFrame		( )															= 0;
		virtual void EndFrame		( )															= 0;

	protected:
		
		LUM_CONST_VAR_QUALIFIER
		static uint8 MAX_SHADERS = 8;

		LUM_CONST_VAR_QUALIFIER
		static uint32 MAX_SAMPLERS = 500;

		LUM_CONST_VAR_QUALIFIER
		static uint32 MAX_BUFFERS = 10000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 MAX_LAYOUTS = 10000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 MAX_TEXTURES = 1000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 MAX_FRAMEBUFFERS = 100;

		cstd::handle_pool<Sampler, SamplerHandle>			m_samplers		{ MAX_SAMPLERS };
		cstd::handle_pool<Shader, ShaderHandle>				m_shaders		{ MAX_SHADERS };
		cstd::handle_pool<Buffer, BufferHandle>				m_buffers		{ MAX_BUFFERS };
		cstd::handle_pool<VertexLayout, VertexLayoutHandle> m_layouts		{ MAX_LAYOUTS };
		cstd::handle_pool<Texture, TextureHandle>			m_textures		{ MAX_TEXTURES };
		cstd::handle_pool<Framebuffer, FramebufferHandle>	m_framebuffers	{ MAX_FRAMEBUFFERS };

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
	RHI_Device* CreateDevice(Window* window);

}