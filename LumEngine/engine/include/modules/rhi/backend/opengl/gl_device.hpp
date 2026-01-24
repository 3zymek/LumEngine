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
		using Mapflag			= rhi::Mapflag;

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

		using Framebuffer					= rhi::Framebuffer;
		using FramebufferHandle				= rhi::FramebufferHandle;
		using FramebufferTextureDescriptor	= rhi::FramebufferTextureDescriptor;

		using Pipeline				= rhi::Pipeline;
		using PipelineHandle		= rhi::PipelineHandle;
		using PipelineDescriptor	= rhi::PipelineDescriptor;

		using State			= rhi::State;
		using PolygonMode	= rhi::PolygonMode;
		using Face			= rhi::Face;

	public:

		GL_Device(Window* win) : pWindow(win) {}

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		BufferHandle		CreateVertexBuffer			( const BufferDescriptor& )							override;
		BufferHandle		CreateElementBuffer			( const BufferDescriptor& )							override;
		BufferHandle		CreateUniformBuffer			( const BufferDescriptor& )							override;
		void				UpdateBuffer				( const BufferHandle&, cvptr, usize, usize )		override;
		void				DeleteBuffer				( BufferHandle& )									override;
		vptr				MapBuffer					( const BufferHandle&, Mapflag, usize, usize )		override;
		void				UnmapBuffer					( const BufferHandle& )								override;
		void				AttachElementBufferToLayout	( const BufferHandle&, const VertexLayoutHandle& )	override;
		void				SetUniformBufferBinding		( const BufferHandle&, int32 )						override;



		///////////////////////////////////////////////////
		/// Framebuffers
		///////////////////////////////////////////////////

		FramebufferHandle	CreateFramebuffer			( )															override;
		TextureHandle		CreateFramebufferTexture	( const FramebufferTextureDescriptor& )						override;
		void				SetFramebufferColorTexture	( const FramebufferHandle&, const TextureHandle&, uint8 )	override;
		void				SetFramebufferDepthTexture	( const FramebufferHandle&, const TextureHandle&)			override;
		void				SetFramebufferStencilTexture( const FramebufferHandle&, const TextureHandle&)			override;
		void				ClearFramebuffer			( FramebufferHandle, glm::vec4, float32)					override;
		void				DeleteFramebuffer			( FramebufferHandle& )										override;
		void				BindFramebuffer				( const FramebufferHandle& )								override;
		void				UnbindFramebuffer			( )															override;



		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		VertexLayoutHandle	CreateVertexLayout( const VertexLayoutDescriptor&, const BufferHandle&)	override;
		void				DeleteVertexLayout( VertexLayoutHandle& )								override;



		///////////////////////////////////////////////////
		/// Shaders ( !!! ADD CACHE LOCATIONS !!! )
		///////////////////////////////////////////////////

		ShaderHandle	CreateShader( const ShaderDescriptor& )							override;
		void			BindShader	( const ShaderHandle& )								override;
		void			DeleteShader( ShaderHandle& )									override;
		void			SetMat4		( const ShaderHandle&, ccharptr, const glm::mat4& )	override;
		void			Setf		( const ShaderHandle&, ccharptr, float32 )			override;
		void			Seti		( const ShaderHandle&, ccharptr, int32 )			override;
		void			SetVec4		( const ShaderHandle&, ccharptr, const glm::vec4& )	override;
		void			SetVec3		( const ShaderHandle&, ccharptr, const glm::vec3& )	override;
		void			SetVec2		( const ShaderHandle&, ccharptr, const glm::vec2& )	override;



		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		TextureHandle	CreateTexture2D		( const TextureDescriptor& )		override;
		TextureHandle	CreateTexture3D		( const TextureDescriptor& )		override;
		void			DeleteTexture		( TextureHandle& )					override;
		void			SetTextureBinding	( const TextureHandle&, uint16)		override;
		void			BindTexture			( const TextureHandle&, uint16 )	override;



		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////

		SamplerHandle	CreateSampler		( const SamplerDescriptor& )		override;
		void			SetSamplerBinding	( const SamplerHandle&, uint16 )	override;
		void			BindSampler			( const SamplerHandle&, uint16)		override;
		void			DeleteSampler		( SamplerHandle )					override;



		///////////////////////////////////////////////////
		/// Pipelines
		///////////////////////////////////////////////////

		rhi::PipelineHandle CreatePipeline(const PipelineDescriptor& )	override;
		void DeletePipeline(PipelineHandle&)							override;
		void BindPipeline(const PipelineHandle&)						override;



		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		void Draw			( const VertexLayoutHandle&, uint32 )	override;
		void DrawElements	( const VertexLayoutHandle&, uint32 )	override;
		void BeginFrame		( )										override;
		void EndFrame		( )										override;



	protected:

		LUM_CONST_VAR_QUALIFIER
		static GLenum skPolygonModeLookup[] = 
		{ 
			GL_POINT, 
			GL_LINE, 
			GL_FILL 
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skFacesLookup[] =
		{ 
			GL_FRONT, 
			GL_BACK, 
			GL_FRONT_AND_BACK 
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skTextureMinFilterLookup[] =
		{
			GL_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_NEAREST,
			GL_NEAREST_MIPMAP_NEAREST,
			GL_NEAREST_MIPMAP_LINEAR
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skSamplerWrapLookup[] =
		{
			GL_REPEAT,
			GL_MIRRORED_REPEAT,
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_BORDER
		};

		LUM_CONST_VAR_QUALIFIER
		static GLenum skCompareFlagLookup[] =
		{
			GL_LESS,
			GL_LEQUAL,
			GL_GREATER,
			GL_GEQUAL,
			GL_NOTEQUAL,
			GL_ALWAYS,
			GL_NEVER
		};

		Window* pWindow = nullptr;

		///////////////////////////////////////////////////
		/// Private helpers ( !!! REPAIR TYPE CASTING !!! )
		///////////////////////////////////////////////////

		void		CacheUniformLocations		( );
		bool		IsValidBufferDescriptor		( const BufferDescriptor&)		noexcept;
		GLbitfield	TranslateMappingFlags		( Mapflag )						noexcept;

	};

}