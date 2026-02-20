//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL implementation of Render Hardware Interface
//
//=============================================================================//
#pragma once

#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"

#include "core/core_defines.hpp"
#include "core/utils/asset_loader.hpp"
#include "core/utils/sparse_set.hpp"

namespace lum::rhi::gl {

	class GLDevice : public RDevice {
	public:

		GLDevice(Window* win) : window(win) {}

		//=================================================
		// Buffers
		//=================================================

		RBufferHandle	CreateBuffer				( const RBufferDescriptor& desc )						override;
		void			UpdateBuffer				( const RBufferHandle&, cvptr, usize, usize )			override;
		void			DeleteBuffer				( RBufferHandle& )										override;
		vptr			MapBuffer					( const RBufferHandle&, Flags<RMapFlag>, usize, usize )	override;
		void			UnmapBuffer					( const RBufferHandle& )								override;
		void			SetShaderStorageBinding		( const RBufferHandle& ssbo, uint32 binding )			override;
		void			AttachElementBufferToLayout	( const RBufferHandle&, const RVertexLayoutHandle& )	override;
		void			SetUniformBufferBinding		( const RBufferHandle&, int32 )							override;



		//=================================================
		// Framebuffers
		//=================================================

		RFramebufferHandle	CreateFramebuffer			( )															override;
		RTextureHandle		CreateFramebufferTexture	( const RFramebufferTextureDescriptor& )					override;
		void				SetFramebufferColorTexture	( const RFramebufferHandle&, const RTextureHandle&, int8 )	override;
		void				SetFramebufferDepthTexture	( const RFramebufferHandle&, const RTextureHandle&)			override;
		void				SetFramebufferStencilTexture( const RFramebufferHandle&, const RTextureHandle&)			override;
		void				ClearFramebuffer			( RFramebufferHandle, ChannelRGBA, float32)					override;
		void				DeleteFramebuffer			( RFramebufferHandle& )										override;
		void				BindFramebuffer				( const RFramebufferHandle& )								override;
		void				UnbindFramebuffer			( )															override;



		//=================================================
		// Layouts
		//=================================================

		RVertexLayoutHandle	CreateVertexLayout ( const RVertexLayoutDescriptor&, const RBufferHandle&)	override;
		void				DeleteVertexLayout ( RVertexLayoutHandle& )									override;



		//=================================================
		// TODO Shaders ( !!! ADD CACHE LOCATIONS !!! )
		//=================================================

		RShaderHandle	CreateShader ( const RShaderDescriptor& )							override;
		void			BindShader	 ( const RShaderHandle& )								override;
		void			DeleteShader ( RShaderHandle& )										override;



		//=================================================
		// Textures
		//=================================================

		RTextureHandle	CreateTexture		 ( const RTextureDescriptor& desc ) override;
		void			UnbindTexture		 ( RTextureType )						override;
		void			UpdateTexture		 ( const RTextureHandle&, const RTextureUpdateDescriptor& ) override;
		void			DeleteTexture		 ( RTextureHandle& )							override;
		void			BindTexture			 ( const RTextureHandle&, uint16 )			override;



		//=================================================
		// Samplers
		//=================================================

		RSamplerHandle	CreateSampler	( const RSamplerDescriptor& )		override;
		void			BindSampler		( const RSamplerHandle&, uint16)		override;
		void			DeleteSampler	( RSamplerHandle )					override;



		//=================================================
		// Pipelines
		//=================================================

		RPipelineHandle		CreatePipeline	( const RPipelineDescriptor& )	override;
		void				DeletePipeline	( RPipelineHandle& )				override;
		void				BindPipeline	( const RPipelineHandle& )		override;



		//=================================================
		// Other
		//=================================================

		void SetViewport		( int32 x, int32 y, int32 width, int32 height )	override;
		void SetViewportX		( int32 x )			override;
		void SetViewportY		( int32 y )			override;
		void SetViewportWidth	( int32 width )		override;
		void SetViewportHeight	( int32 height )	override;

		// Scissors setters
		void ToggleScissors			( bool )										override;
		bool IsScissorEnabled		( )												const noexcept override;
		void SetScissors			( int32 x, int32 y, int32 width, int32 height )	override;
		void SetScissorX			( int32 x )			override;
		void SetScissorY			( int32 y )			override;
		void SetScissorWidth		( int32 width )		override;
		void SetScissorHeight		( int32 height )	override;


		// Cull setters
		void ToggleCull		( bool )			override;
		bool IsCullEnabled	( )					const noexcept override;
		void SetCullFace	( RFace face )		override;
		void SetFrontFace	( RWindingOrder )	override;


		// Blend setters
		void ToggleBlend				( bool enabled )		override;
		bool IsBlendEnabled				( )						const noexcept override;
		void SetBlendConstantColor		( ChannelRGBA rgba )	override;
		void SetBlendFactors			( RBlendFactor srcColor, RBlendFactor dstColor, RBlendFactor srcAlpha, RBlendFactor dstAlpha ) override;
		void SetBlendColorFactors		( RBlendFactor srcColor, RBlendFactor dstColor )	override;
		void SetBlendAlphaFactors		( RBlendFactor srcAlpha, RBlendFactor dstAlpha )	override;
		void SetBlendSrcColorFactor		( RBlendFactor factor )							override;
		void SetBlendDstColorFactor		( RBlendFactor factor )							override;
		void SetBlendSrcAlphaFactor		( RBlendFactor factor )							override;
		void SetBlendDstAlphaFactor		( RBlendFactor factor )							override;
		void SetBlendOp					( RBlendOp colorOp, RBlendOp alphaOp )			override;
		void SetBlendColorOp			( RBlendOp op )									override;
		void SetBlendAlphaOp			( RBlendOp op )									override;
		void SetBlendFactorsForTarget	( uint8 target )								override;
		void ToggleBlendForTarget		( uint8 target, bool enable )					override;

		// Depth setters
		void ToggleDepthWrite	 ( bool )		override;
		bool IsDepthWriteEnabled ( )			const noexcept override;

		void ToggleDepthTest	( bool )		override;
		bool IsDepthTestEnabled	( )				const noexcept override;
		void SetDepthFunc		( RCompareFlag ) override;

		// Stencil setters
		void ToggleStencilTest			( bool )			override;
		bool IsStencilTestEnabled		( )					const noexcept override;
		void SetStencilReference		( int32 ref, RFace )	override;
		void SetStencilOp				( RStencilOp sfail, RStencilOp dpfail, RStencilOp dppass, RFace face)	override;
		void SetStencilOpOnStencilFail	( RStencilOp op, RFace face )	override;
		void SetStencilOpOnDepthFail	( RStencilOp op, RFace face )	override;
		void SetStencilOpOnDepthPass	( RStencilOp op, RFace face )	override;


		// Rasterizer setters
		void ToggleDepthBias		( bool )							override;
		bool IsDepthBiasEnabled		( )									const noexcept override;
		void SetDepthBiasFactors	( float32 slope, float32 constant ) override;
		void SetDepthBiasClamp		( float32 clamp )					override;
		void SetDepthBiasSlope		( float32 )							override;
		void SetDepthBiasConstant	( float32 )							override;
		void SetTopology			( RTopologyMode mode, RFace face )	override;

		const RBlendState&			GetBlendState		 ( ) const noexcept override;
		const RCullState&			GetCullState		 ( ) const noexcept override;
		const RScissorState&			GetScissorState		 ( ) const noexcept override;
		const RDepthStencilState&	GetDepthStencilState ( ) const noexcept override;
		const RRasterizerState&		GetRasterizerState	 ( ) const noexcept override;
		const RViewportState&		GetViewport			 ( ) const noexcept override;

		bool IsValid ( RBufferHandle handle )		const override;
		bool IsValid ( RTextureHandle handle )		const override;
		bool IsValid ( RShaderHandle handle )		const override;
		bool IsValid ( RFramebufferHandle handle )	const override;
		bool IsValid ( RVertexLayoutHandle handle )  const override;
		bool IsValid ( RPipelineHandle handle )		const override;
		bool IsValid ( RSamplerHandle handle )		const override;

		void SetColorMask ( bool r, bool g, bool b, bool a )	override;
		void SetColorMask ( RColorMask rgba )					override;

		void SetClearColor	( ChannelRGBA color )	override;
		void ClearColor		( )						override;
		void ClearColor		( ChannelRGBA color )	override;
		void ClearDepth		( )						override;
		void ClearStencil	( )						override;
		void Clear			( Flags<RClearFlag> )	override;

		void SetPointSize ( float32 size )	override;
		void SetLineWidth ( float32 width )	override;

		void ToggleMultisample			 ( bool toggle ) override;
		void ToggleSampleCoverage		 ( bool toggle ) override;
		void ToggleSampleAlphaToCoverage ( bool toggle ) override;
		void ToggleSampleAlphaToOne		 ( bool toggle ) override;
		void SetSampleCoverage			 ( float32 value, bool invert ) override;

		void Draw						( const RVertexLayoutHandle&, uint32 )	override;
		void DrawInstanced				( const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount ) override;
		void DrawInstancedBase			( const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount, uint32 baseInstance ) override;
		void DrawElements				( const RVertexLayoutHandle&, uint32 )	override;
		void DrawElementsInstanced		( const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount ) override;
		void DrawElementsInstancedBase	( const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount, uint32 baseInstance ) override;

		void BeginFrame	( )	override;
		void EndFrame	( )	override;


	protected:

		LUM_COMPILE_VARIABLE
		static GLenum skTopologyModeLookup[] = 
		{ 
			GL_POINT, 
			GL_LINE, 
			GL_FILL 
		};

		LUM_COMPILE_VARIABLE
		static GLenum skFacesLookup[] =
		{ 
			GL_FRONT, 
			GL_BACK, 
			GL_FRONT_AND_BACK 
		};

		LUM_COMPILE_VARIABLE
		static GLenum skTextureMinFilterLookup[] =
		{
			GL_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,

			GL_NEAREST,
			GL_NEAREST_MIPMAP_NEAREST,
			GL_NEAREST_MIPMAP_LINEAR
		};

		LUM_COMPILE_VARIABLE
		static GLenum skSamplerWrapLookup[] =
		{
			GL_REPEAT,
			GL_MIRRORED_REPEAT,

			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_BORDER
		};

		LUM_COMPILE_VARIABLE
		static GLenum skCompareFlagLookup[] =
		{
			GL_EQUAL,
			GL_NOTEQUAL,
			GL_LEQUAL,
			GL_GEQUAL,
			GL_LESS,
			GL_GREATER,
			GL_ALWAYS,
			GL_NEVER
		};

		LUM_COMPILE_VARIABLE
		static GLenum skBlendFactorLookup[] =
		{
			GL_ZERO,
			GL_ONE,

			GL_SRC_COLOR,
			GL_ONE_MINUS_SRC_COLOR,
			GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,

			GL_DST_COLOR,
			GL_ONE_MINUS_DST_COLOR,
			GL_DST_ALPHA,
			GL_ONE_MINUS_DST_ALPHA,

			GL_CONSTANT_COLOR,
			GL_ONE_MINUS_CONSTANT_COLOR,
			GL_CONSTANT_ALPHA,
			GL_ONE_MINUS_CONSTANT_ALPHA,

			GL_SRC_ALPHA_SATURATE,

			GL_SRC1_COLOR,
			GL_ONE_MINUS_SRC1_COLOR,
			GL_SRC1_ALPHA,
			GL_ONE_MINUS_SRC1_ALPHA,

		};

		LUM_COMPILE_VARIABLE
		static GLenum skBlendOpLookup[] =
		{
			GL_FUNC_ADD,
			GL_FUNC_SUBTRACT,
			GL_FUNC_REVERSE_SUBTRACT,
			GL_MIN,
			GL_MAX
		};

		LUM_COMPILE_VARIABLE
		static GLenum skStencilOpLookup[] =
		{
			GL_ZERO,
			GL_ONE,
			GL_KEEP,
			GL_REPLACE,
			GL_INCR,
			GL_INCR_WRAP,
			GL_DECR,
			GL_DECR_WRAP,
			GL_INVERT
		};

		LUM_COMPILE_VARIABLE
		static GLenum skClearOpLookup[] =
		{
			GL_COLOR_BUFFER_BIT,
			GL_DEPTH_BUFFER_BIT,
			GL_STENCIL_BUFFER_BIT
		};

		LUM_COMPILE_VARIABLE
		static GLenum skTextureTypeLookup[] =
		{
			GL_NONE,
			GL_TEXTURE_2D,
			GL_TEXTURE_3D,
			GL_TEXTURE_CUBE_MAP
		};

		LUM_COMPILE_VARIABLE
		static GLenum skInternalImageFormatLookup[] =
		{
			GL_RGBA8,
			GL_SRGB8_ALPHA8,
			GL_RGB8,
			GL_RG8,
			GL_R8,

			GL_RGBA16F,
			GL_RGB16F,
			GL_RG16F,
			GL_R16F,

			GL_RGBA32F,
			GL_RGB32F,
			GL_RG32F,
			GL_R32F,
			
			GL_DEPTH_COMPONENT24,
			GL_DEPTH_COMPONENT32F,
			GL_DEPTH24_STENCIL8,
			GL_DEPTH32F_STENCIL8,

		};

		LUM_COMPILE_VARIABLE
		static GLenum skLoadedImageFormatLookup[] =
		{
			GL_RGBA,
			GL_RGB,
			GL_RG,
			GL_RED,
			GL_BGRA,
			GL_DEPTH_COMPONENT,
			GL_DEPTH_STENCIL
		};

		LUM_COMPILE_VARIABLE
		static GLenum skTextureDataTypeLookup[] =
		{
			GL_UNSIGNED_BYTE,
			GL_BYTE,

			GL_UNSIGNED_SHORT,
			GL_SHORT,

			GL_UNSIGNED_INT,
			GL_INT,

			GL_HALF_FLOAT,
			GL_FLOAT,

			GL_UNSIGNED_INT_24_8,
			GL_FLOAT_32_UNSIGNED_INT_24_8_REV
		};

		Window* window = nullptr;

		//=================================================
		// Private helpers
		//=================================================

		LUM_FORCEINLINE
		void bind_check_shader(const RPipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_rasterizer(const RPipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_depth_stencil(const RPipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_scissors(const RPipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_blend(const RPipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_cull(const RPipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_color_mask(const RPipeline&) noexcept;



		LUM_FORCEINLINE
		bool compile_shader(GLuint shader);

		LUM_FORCEINLINE
		bool link_program(GLuint program);

		RTextureHandle create_texture_2d(const RTextureDescriptor&);
		RTextureHandle create_texture_3d(const RTextureDescriptor&);
		RTextureHandle create_texture_cubemap(const RTextureDescriptor&);
		
		bool		validate_texture_descriptor	( const RTextureDescriptor& )	noexcept;
		bool		validate_buffer_descriptor	( const RBufferDescriptor& )	noexcept;
		GLbitfield	translate_mapping_flags		( Flags<RMapFlag> )				noexcept;

	};

}