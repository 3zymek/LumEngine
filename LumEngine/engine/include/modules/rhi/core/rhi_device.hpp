#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "rhi/core/rhi_shader.hpp"
#include "rhi/rhi_common.hpp"
namespace lum { class Window; }
namespace lum::rhi {

	class RHI_Device {
	public:

		virtual void BeginFrame	( ) = 0;
		virtual void EndFrame	( ) = 0;


		virtual BufferHandle	CreateBuffer( const BufferDescriptor& desc ) = 0;
		virtual void		UpdateBuffer( 
			BufferHandle vbo, 
			const void* data, 
			size_t offset, 
			size_t size,
			MapFlag map_flags
		) = 0;
		virtual void		DeleteBuffer( BufferHandle& vbo ) = 0;
		virtual void*		MapBuffer	( BufferHandle vbo, size_t, size_t, MapFlag ) = 0;
		virtual void		UnmapBuffer	( BufferHandle vbo ) = 0;


		virtual VertexLayoutHandle CreateVertexLayout( const VertexLayoutDescriptor& desc, BufferHandle vbo ) = 0;
		virtual void DeleteVertexLayout( ) = 0;

		virtual ShaderHandle	CreateShader	( const ShaderDescriptor& desc) = 0;
		virtual void			BindShader		( ShaderHandle shader )				= 0;
		virtual void			DeleteShader	( ShaderHandle shader )				= 0;


		virtual void Draw( VertexLayoutHandle vao, uint32_t vertex_count ) = 0;

	protected:

		static constexpr unsigned int MAX_SHADERS = 8;
		static constexpr unsigned int MAX_BUFFERS = 10000;
		static constexpr unsigned int MAX_LAYOUTS = 10000;

		cstd::handle_pool<Shader, ShaderHandle> m_shaders{ MAX_SHADERS };
		cstd::handle_pool<Buffer, BufferHandle> m_buffers{ MAX_BUFFERS };
		cstd::handle_pool<VertexLayout, VertexLayoutHandle> m_layouts{ MAX_LAYOUTS };

	};

	RHI_Device* CreateDevice(Window*);


}