#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "rhi/core/rhi_shader.hpp"
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_texture.hpp"
namespace lum { class Window; }
namespace lum::rhi {

	class RHI_Device {
	public:

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		/*! @brief Creates Vertex buffer (VBO).
		* 
		*  @param desc BufferDescriptor of VBO (containing size, flags, usage and data).
		*  @return Handle to the buffer.
		* 
		*/
		virtual BufferHandle CreateVertexBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Creates Element buffer (EBO).
		*
		*  @param desc BufferDescriptor of EBO (containing size, flags, usage and data).
		*  @return Handle to the buffer.
		* 
		*/
		virtual BufferHandle CreateElementBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Creates Uniform buffer (UBO).
		*
		*  @param desc BufferDescriptor of UBO (containing size, flags, usage and data).
		*  @return Handle to the buffer.
		* 
		*/
		virtual BufferHandle CreateUniformBuffer( const BufferDescriptor& desc ) = 0;
		/*! @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.  
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer (0 for whole buffer).
		*  @param size Byte size of data to write (0 for whole buffer).
		* 
		*/
		virtual void UpdateBuffer( BufferHandle buff, const void* data, size_t offset = 0, size_t size = 0 ) = 0;
		/*! @brief Deletes buffer.
		*
		*  @param buff Buffer handle to delete.
		*
		*/
		virtual void DeleteBuffer( BufferHandle& buff ) = 0;
		/*! @brief Maps data of given buffer.
		*
		*  @param buff Buffer handle to map.
		*  @param flags Mapping flags (example: rhi::map_flags::Write).
		*  @param offset Byte offset from the beginning of the buffer to map (0 for whole buffer).
		*  @param size Byte size of data to map (0 for whole buffer).
		* 
		*  @return GPU data pointer to manage.
		*	
		*/
		virtual void*			MapBuffer	( BufferHandle buff, MapFlag flags, size_t offset = 0, size_t size = 0 ) = 0;
		/*! @brief Unmaps buffer.
		*
		*  @param buff Buffer handle to unmap.
		*
		*/
		virtual void			UnmapBuffer	( BufferHandle buff ) = 0;
		/*! @brief Connects element buffer (EBO) to vertex layout (VAO).
		*
		*  @param ebo Element buffer to connect.
		*  @param vao Vertex layout to connect.
		* 
		*/
		virtual void AttachElementBufferToLayout( BufferHandle ebo, VertexLayoutHandle vao ) = 0;



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
		virtual VertexLayoutHandle	CreateVertexLayout( const VertexLayoutDescriptor& desc, BufferHandle vbo )	= 0;
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
		virtual void			BindShader	( ShaderHandle shader ) = 0;
		/*! @brief Deletes shader.
		*
		*  @param shader Shader to delete.
		*
		*/
		virtual void			DeleteShader( ShaderHandle& shader ) = 0;
		virtual void SetMat4(glm::mat4)



		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		virtual TextureHandle	CreateTexture2D	( const TextureDescriptor& desc ) = 0;
		virtual TextureHandle	CreateTexture3D	( const TextureDescriptor& desc ) = 0;
		virtual void			DeleteTexture	( TextureHandle& texture ) = 0;
		virtual void			BindTexture		( TextureHandle texture ) = 0;



		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		virtual void Draw			( VertexLayoutHandle vao, uint32_t vertex_count )	= 0;
		virtual void DrawElements	( VertexLayoutHandle, uint32_t indices_count )		= 0;
		virtual void BeginFrame		( )													= 0;
		virtual void EndFrame		( )													= 0;

	protected:

		static constexpr unsigned int MAX_SHADERS = 8;
		static constexpr unsigned int MAX_BUFFERS = 10000;
		static constexpr unsigned int MAX_LAYOUTS = 10000;
		static constexpr unsigned int MAX_TEXTURES = 1000;

		cstd::handle_pool<Shader, ShaderHandle>				m_shaders	{ MAX_SHADERS };
		cstd::handle_pool<Buffer, BufferHandle>				m_buffers	{ MAX_BUFFERS };
		cstd::handle_pool<VertexLayout, VertexLayoutHandle> m_layouts	{ MAX_LAYOUTS };
		cstd::handle_pool<Texture, TextureHandle>			m_textures	{ MAX_TEXTURES };

	};

	RHI_Device* CreateDevice(Window*);

}