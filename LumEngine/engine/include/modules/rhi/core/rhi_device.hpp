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
namespace lum { class Window; }
namespace lum::rhi {

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
		virtual BufferHandle CreateVertexBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Creates Element buffer ( EBO ).
		*
		*  @param desc BufferDescriptor of EBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		virtual BufferHandle CreateElementBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Creates Uniform buffer ( UBO ).
		*
		*  @param desc BufferDescriptor of UBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		virtual BufferHandle CreateUniformBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.  
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer ( 0 for whole buffer ).
		*  @param size Byte size of data to write ( 0 for whole buffer ).
		* 
		*/
		virtual void UpdateBuffer( const BufferHandle& buff, cvptr_t data, size_t offset = 0, size_t size = 0 ) = 0;
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
		virtual vptr_t			MapBuffer	( const BufferHandle& buff, mapflag_t flags, size_t offset = 0, size_t size = 0 ) = 0;
		/*! @brief Unmaps buffer.
		*
		*  @param buff Buffer handle to unmap.
		*
		*/
		virtual void			UnmapBuffer	( const BufferHandle& buff ) = 0;
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
		virtual void SetUniformBufferBinding(const BufferHandle& ubo, int binding) = 0;


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
		virtual VertexLayoutHandle	CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo ) = 0;
		/*! @brief Deletes vertex layout.
		*
		*  @param layout Layout handle to delete.
		*
		*/
		virtual void DeleteVertexLayout( VertexLayoutHandle& layout ) = 0;



		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		/*! @brief Creates Shader.
		*
		*  @param desc Shader Descriptor (vertex source file name, fragment source file name).
		*
		*  @return Handle to shader.
		*
		*/
		virtual ShaderHandle	CreateShader( const ShaderDescriptor& desc)	= 0;
		/*! @brief Binds shader.
		*
		*  @param shader Shader to bind.
		*
		*/
		virtual void			BindShader	( const ShaderHandle& shader ) = 0;
		/*! @brief Deletes shader.
		*
		*  @param shader Shader to delete.
		*
		*/
		virtual void			DeleteShader( ShaderHandle& shader ) = 0;
		virtual void			SetMat4		( const ShaderHandle& shader, cstr_t location, const glm::mat4& mat ) = 0;
		virtual void			Setf		( const ShaderHandle& shader, cstr_t location, float value )			= 0;
		virtual void			Seti		( const ShaderHandle& shader, cstr_t location, int value )			= 0;
		virtual void			SetVec3		( const ShaderHandle& shader, cstr_t location, const glm::vec4& vec ) = 0;
		virtual void			SetVec3		( const ShaderHandle& shader, cstr_t location, const glm::vec3& vec ) = 0;
		virtual void			SetVec3		( const ShaderHandle& shader, cstr_t location, const glm::vec2& vec ) = 0;

		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		virtual TextureHandle	CreateTexture2D	( const TextureDescriptor& desc )	= 0;
		virtual TextureHandle	CreateTexture3D	( const TextureDescriptor& desc )	= 0;
		virtual void			DeleteTexture	( TextureHandle& texture )			= 0;
		virtual void			BindTexture		( const TextureHandle& texture )	= 0;
		virtual void			CreateSampler	( const SamplerDescriptor& desc )	= 0;



		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		virtual void Draw			( const VertexLayoutHandle& vao, uint32_t vertex_count )	= 0;
		virtual void DrawElements	( const VertexLayoutHandle&, uint32_t indices_count )		= 0;
		virtual void BeginFrame		( )															= 0;
		virtual void EndFrame		( )															= 0;

	protected:

		LUM_CONST_VAR_QUALIFIER
		static unsigned int MAX_SHADERS = 8;

		LUM_CONST_VAR_QUALIFIER
		static unsigned int MAX_SAMPLERS = 500;

		LUM_CONST_VAR_QUALIFIER
		static unsigned int MAX_BUFFERS = 10000;

		LUM_CONST_VAR_QUALIFIER
		static unsigned int MAX_LAYOUTS = 10000;

		LUM_CONST_VAR_QUALIFIER
		static unsigned int MAX_TEXTURES = 1000;

		//cstd::handle_pool<Sampler, 
		cstd::handle_pool<Shader, ShaderHandle>				m_shaders	{ MAX_SHADERS };
		cstd::handle_pool<Buffer, BufferHandle>				m_buffers	{ MAX_BUFFERS };
		cstd::handle_pool<VertexLayout, VertexLayoutHandle> m_layouts	{ MAX_LAYOUTS };
		cstd::handle_pool<Texture, TextureHandle>			m_textures	{ MAX_TEXTURES };

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