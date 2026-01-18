#pragma once
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "core/utils/sparse_set.hpp"
namespace lum::gl {

	class GL_Device : public rhi::RHI_Device {

		using BufferHandle		= rhi::BufferHandle;
		using BufferDescriptor	= rhi::BufferDescriptor;
		using MapFlag			= rhi::mapflag_t;

		using VertexLayoutHandle		= rhi::VertexLayoutHandle;
		using VertexLayoutDescriptor	= rhi::VertexLayoutDescriptor;

		using ShaderHandle		= rhi::ShaderHandle;
		using ShaderDescriptor	= rhi::ShaderDescriptor;

		using Texture			= rhi::Texture;
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
		void			UpdateBuffer		( const BufferHandle& buff, cvptr_t data, size_t offset = 0, size_t size = 0 )	override;
		void			DeleteBuffer		( BufferHandle& buff )															override;
		vptr_t			MapBuffer			( const BufferHandle& buff, MapFlag flags, size_t offset = 0, size_t size = 0 )	override;
		void			UnmapBuffer			( const BufferHandle& buff )													override;
		void			AttachElementBufferToLayout	( const BufferHandle&, const VertexLayoutHandle& )						override;
		void			SetUniformBufferBinding		( const BufferHandle& ubo, int binding )								override;

		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		VertexLayoutHandle	CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo)	override;
		void				DeleteVertexLayout( VertexLayoutHandle& layout )									override;

		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		ShaderHandle	CreateShader( const ShaderDescriptor& desc )									override;
		void			BindShader	( const ShaderHandle& shader )										override;
		void			DeleteShader( ShaderHandle& shader )											override;
		void			SetMat4		( const ShaderHandle& shader, cstr_t location, const glm::mat4& mat )	override;
		void			Setf		( const ShaderHandle& shader, cstr_t location, float value )			override;
		void			Seti		( const ShaderHandle& shader, cstr_t location, int value )			override;
		void			SetVec3		( const ShaderHandle& shader, cstr_t location, const glm::vec4& vec )	override;
		void			SetVec3		( const ShaderHandle& shader, cstr_t location, const glm::vec3& vec )	override;
		void			SetVec3		( const ShaderHandle& shader, cstr_t location, const glm::vec2& vec )	override;

		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		TextureHandle	CreateTexture2D	( const TextureDescriptor& desc )	override;
		TextureHandle	CreateTexture3D	( const TextureDescriptor& desc )	override;
		void			DeleteTexture	( TextureHandle& texture )			override;
		void			BindTexture		( const TextureHandle& texture )	override;


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		void Draw			( const VertexLayoutHandle& vao, uint32_t vertex_count )	override;
		void DrawElements	( const VertexLayoutHandle& vao, uint32_t indices_count )	override;
		void BeginFrame		( )													override;
		void EndFrame		( )													override;

	protected:

		LUM_CONST_VAR_QUALIFIER 
		static unsigned int MAX_UNIFORMS = 1000;

		cstd::sparse_set<GLuint, uint32_t> m_unifomrs{ MAX_UNIFORMS };

		Window* window = nullptr;

		///////////////////////////////////////////////////
		/// Private helpers
		///////////////////////////////////////////////////

		void		CacheUniformLocations();
		GLbitfield	TranslateTextureMinFilter(const rhi::TextureMinFilter&);
		GLbitfield	TranslateTextureMagFilter(const rhi::TextureMagFilter&);
		bool		IsValidBufferDescriptor	( const BufferDescriptor& desc );
		GLbitfield	TranslateMappingFlags	( MapFlag flags );

	};

}