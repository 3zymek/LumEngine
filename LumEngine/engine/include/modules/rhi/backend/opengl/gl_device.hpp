#pragma once
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
namespace lum::gl {

	class GL_Device : public rhi::RHI_Device {

		using BufferHandle				= rhi::BufferHandle;
		using BufferDescriptor			= rhi::BufferDescriptor;
		using MapFlag					= rhi::MapFlag;

		using LayoutHandle		= rhi::VertexLayoutHandle;
		using LayoutDescriptor	= rhi::VertexLayoutDescriptor;

		using ShaderHandle		= rhi::ShaderHandle;
		using ShaderDescriptor	= rhi::ShaderDescriptor;

	public:

		GL_Device(Window* win) : window(win) {}

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		BufferHandle	CreateVertexBuffer	( const BufferDescriptor& desc )									override;
		BufferHandle	CreateElementBuffer	( const BufferDescriptor& desc )									override;
		BufferHandle	CreateUniformBuffer	( const BufferDescriptor& desc )									override;
		void			UpdateBuffer( BufferHandle buff, const void* data, size_t offset = 0, size_t size = 0 )	override;
		void			DeleteBuffer( BufferHandle& buff )														override;
		void*			MapBuffer	( BufferHandle buff, MapFlag flags, size_t offset = 0, size_t size = 0 )	override;
		void			UnmapBuffer	( BufferHandle buff )														override;
		void			AttachElementBufferToLayout( BufferHandle, LayoutHandle )								override;
		
		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		LayoutHandle	CreateVertexLayout( const LayoutDescriptor& desc, BufferHandle vbo) override;
		void			DeleteVertexLayout( )												override;

		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		ShaderHandle	CreateShader( const ShaderDescriptor& desc )	override;
		void			BindShader	( ShaderHandle shader )				override;
		void			DeleteShader( ShaderHandle shader )				override;


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		void Draw			(LayoutHandle vao, uint32_t vertex_count )		override;
		void DrawElements	(LayoutHandle vao, uint32_t indices_count )		override;
		void BeginFrame		( )												override;
		void EndFrame		( )												override;

	protected:

		Window* window = nullptr;

		///////////////////////////////////////////////////
		/// Private helpers
		///////////////////////////////////////////////////

		bool		IsValidBufferDescriptor	( const BufferDescriptor& desc );
		GLbitfield	TranslateMappingFlags	( MapFlag flags );

	};

}