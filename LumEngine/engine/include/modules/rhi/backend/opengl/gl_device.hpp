#pragma once
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
namespace lum::gl {

	class GL_Device : public rhi::RHI_Device {

		using BufferHandle		= rhi::BufferHandle;
		using BufferDescriptor	= rhi::BufferDescriptor;
		using MapFlag			= rhi::MapFlag;

		using VertexLayoutHandle		= rhi::VertexLayoutHandle;
		using VertexLayoutDescriptor	= rhi::VertexLayoutDescriptor;

		using ShaderHandle		= rhi::ShaderHandle;
		using ShaderDescriptor	= rhi::ShaderDescriptor;

		using Texture = rhi::Texture;
		using TextureHandle		= rhi::TextureHandle;
		using TextureDescriptor = rhi::TextureDescriptor;

	public:

		GL_Device(Window* win) : window(win) {}

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		BufferHandle	CreateVertexBuffer	( const BufferDescriptor& desc )									override;
		BufferHandle	CreateElementBuffer	( const BufferDescriptor& desc )									override;
		BufferHandle	CreateUniformBuffer	( const BufferDescriptor& desc )									override;
		void			UpdateBuffer		( const BufferHandle& buff, cvptr data, size_t offset = 0, size_t size = 0 )	override;
		void			DeleteBuffer		( BufferHandle& buff )															override;
		vptr			MapBuffer			( const BufferHandle& buff, MapFlag flags, size_t offset = 0, size_t size = 0 )	override;
		void			UnmapBuffer			( const BufferHandle& buff )													override;
		void			AttachElementBufferToLayout( const BufferHandle&, const VertexLayoutHandle& )						override;
		
		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		VertexLayoutHandle	CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo)	override;
		void				DeleteVertexLayout( VertexLayoutHandle& layout )									override;

		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		ShaderHandle	CreateShader( const ShaderDescriptor& desc )						override;
		void			BindShader	( const ShaderHandle& shader )							override;
		void			DeleteShader( ShaderHandle& shader )								override;
		virtual void	SetMat4		( const ShaderHandle& shader, const glm::mat4& mat )	override;
		virtual void	Setf		( const ShaderHandle& shader, float value )				override;
		virtual void	Seti		( const ShaderHandle& shader, int value )				override;
		virtual void	SetVec3		( const ShaderHandle& shader, const glm::vec4& vec )	override;
		virtual void	SetVec3		( const ShaderHandle& shader, const glm::vec3& vec )	override;
		virtual void	SetVec3		( const ShaderHandle& shader, const glm::vec2& vec )	override;

		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		virtual TextureHandle	CreateTexture2D	( const TextureDescriptor& desc )	override;
		virtual TextureHandle	CreateTexture3D	( const TextureDescriptor& desc )	override;
		virtual void			DeleteTexture	( TextureHandle& texture )			override;
		virtual void			BindTexture		( const TextureHandle& texture )	override;


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		void Draw			( const VertexLayoutHandle& vao, uint32_t vertex_count )	override;
		void DrawElements	( const VertexLayoutHandle& vao, uint32_t indices_count )	override;
		void BeginFrame		( )													override;
		void EndFrame		( )													override;

	protected:

		Window* window = nullptr;

		///////////////////////////////////////////////////
		/// Private helpers
		///////////////////////////////////////////////////

		GLbitfield	TranslateTextureMinFilter(const rhi::TextureMinFilter&);
		GLbitfield	TranslateTextureMagFilter(const rhi::TextureMagFilter&);
		bool		IsValidBufferDescriptor	( const BufferDescriptor& desc );
		GLbitfield	TranslateMappingFlags	( MapFlag flags );

	};

}