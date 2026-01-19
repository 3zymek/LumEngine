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
		using mapflag_t			= rhi::mapflag;

		using VertexLayout				= rhi::VertexLayout;
		using VertexLayoutHandle		= rhi::VertexLayoutHandle;
		using VertexLayoutDescriptor	= rhi::VertexLayoutDescriptor;

		using ShaderHandle		= rhi::ShaderHandle;
		using ShaderDescriptor	= rhi::ShaderDescriptor;

		using Texture			= rhi::Texture;
		using TextureHandle		= rhi::TextureHandle;
		using TextureDescriptor = rhi::TextureDescriptor;

		using Sampler			= rhi::Sampler;
		using SamplerHandle		= rhi::SamplerHandle;
		using SamplerDescriptor = rhi::SamplerDescriptor;

	public:

		GL_Device(Window* win) : window(win) {}

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		BufferHandle	CreateVertexBuffer	( const BufferDescriptor& desc )									override;
		BufferHandle	CreateElementBuffer	( const BufferDescriptor& desc )									override;
		BufferHandle	CreateUniformBuffer	( const BufferDescriptor& desc )									override;
		void			UpdateBuffer		( const BufferHandle& buff, LUMcvptr data, LUMsize offset = 0, LUMsize size = 0 )	override;
		void			DeleteBuffer		( BufferHandle& buff )															override;
		LUMvptr			MapBuffer			( const BufferHandle& buff, mapflag_t flags, LUMsize offset = 0, LUMsize size = 0 )	override;
		void			UnmapBuffer			( const BufferHandle& buff )													override;
		void			AttachElementBufferToLayout	( const BufferHandle&, const VertexLayoutHandle& )						override;
		void			SetUniformBufferBinding		( const BufferHandle& ubo, LUMint binding )								override;

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
		void			SetMat4		( const ShaderHandle& shader, LUMcharptr location, const glm::mat4& mat )	override;
		void			Setf		( const ShaderHandle& shader, LUMcharptr location, LUMfloat value )			override;
		void			Seti		( const ShaderHandle& shader, LUMcharptr location, LUMint value )			override;
		void			SetVec3		( const ShaderHandle& shader, LUMcharptr location, const glm::vec4& vec )	override;
		void			SetVec3		( const ShaderHandle& shader, LUMcharptr location, const glm::vec3& vec )	override;
		void			SetVec3		( const ShaderHandle& shader, LUMcharptr location, const glm::vec2& vec )	override;

		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		TextureHandle	CreateTexture2D	( const TextureDescriptor& desc )	override;
		TextureHandle	CreateTexture3D	( const TextureDescriptor& desc )	override;
		void			DeleteTexture	( TextureHandle& texture )			override;
		void			BindTexture		( const TextureHandle& texture )	override;


		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////

		SamplerHandle	CreateSampler	( const SamplerDescriptor& desc )	override;
		void			SetSamplerBinding(const SamplerHandle& sampler, LUMint binding) override;
		void			BindSampler		( const SamplerHandle& sampler )	override;
		void			DeleteSampler	( SamplerHandle sampler )			override;


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		void Draw			( const VertexLayoutHandle& vao, LUMuint vertex_count )	override;
		void DrawElements	( const VertexLayoutHandle& vao, LUMuint indices_count )	override;
		void BeginFrame		( )													override;
		void EndFrame		( )													override;

	protected:

		LUM_CONST_VAR_QUALIFIER 
		static unsigned int MAX_UNIFORMS = 1000;

		cstd::sparse_set<GLuint, LUMuint> m_unifomrs{ MAX_UNIFORMS };

		Window* window = nullptr;

		///////////////////////////////////////////////////
		/// Private helpers
		///////////////////////////////////////////////////

		void		CacheUniformLocations		( );
		GLbitfield	TranslateDataFormat			( const rhi::DataFormat ) noexcept;
		GLbitfield	TranslateTextureMinFilter	( const rhi::SamplerMinFilter ) noexcept;
		GLbitfield	TranslateTextureMagFilter	( const rhi::SamplerMagFilter ) noexcept;
		GLbitfield	TranslateTextureWrap		( const rhi::SamplerWrap wrap ) noexcept;
		bool		IsValidBufferDescriptor		( const BufferDescriptor& desc ) noexcept;
		GLbitfield	TranslateMappingFlags		( mapflag_t flags ) noexcept;

	};

}