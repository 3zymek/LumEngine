#pragma once
#include "rhi/core/rhi_device.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/rhi_pch.hpp"
#include "core/core_defines.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "core/asset_service.hpp"
#include "core/utils/sparse_set.hpp"
namespace lum::rhi::gl {

	class GLDevice : public RenderDevice {
	public:

		GLDevice(Window* win) : window(win) {}

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		BufferHandle	CreateVertexBuffer			( const BufferDescriptor& )								override;
		BufferHandle	CreateElementBuffer			( const BufferDescriptor& )								override;
		BufferHandle	CreateUniformBuffer			( const BufferDescriptor& )								override;
		BufferHandle	CreateShaderStorageBuffer	( const BufferDescriptor& )								override;
		void			UpdateBuffer				( const BufferHandle&, cvptr, usize, usize )			override;
		void			DeleteBuffer				( BufferHandle& )										override;
		vptr			MapBuffer					( const BufferHandle&, Flags<Mapflag>, usize, usize )	override;
		void			UnmapBuffer					( const BufferHandle& )									override;
		void			AttachElementBufferToLayout	( const BufferHandle&, const VertexLayoutHandle& )		override;
		void			SetUniformBufferBinding		( const BufferHandle&, int32 )							override;



		///////////////////////////////////////////////////
		/// Framebuffers
		///////////////////////////////////////////////////

		FramebufferHandle	CreateFramebuffer			( )															override;
		TextureHandle		CreateFramebufferTexture	( const FramebufferTextureDescriptor& )						override;
		void				SetFramebufferColorTexture	( const FramebufferHandle&, const TextureHandle&, uint8 )	override;
		void				SetFramebufferDepthTexture	( const FramebufferHandle&, const TextureHandle&)			override;
		void				SetFramebufferStencilTexture( const FramebufferHandle&, const TextureHandle&)			override;
		void				ClearFramebuffer			( FramebufferHandle, ChannelRGBA, float32)					override;
		void				DeleteFramebuffer			( FramebufferHandle& )										override;
		void				BindFramebuffer				( const FramebufferHandle& )								override;
		void				UnbindFramebuffer			( )															override;



		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		VertexLayoutHandle	CreateVertexLayout ( const VertexLayoutDescriptor&, const BufferHandle&)	override;
		void				DeleteVertexLayout ( VertexLayoutHandle& )									override;



		///////////////////////////////////////////////////
		/// Shaders ( !!! ADD CACHE LOCATIONS !!! )
		///////////////////////////////////////////////////

		ShaderHandle	CreateShader ( const ShaderDescriptor& )							override;
		void			BindShader	 ( const ShaderHandle& )								override;
		void			DeleteShader ( ShaderHandle& )										override;
		void			SetMat4		 ( const ShaderHandle&, ccharptr, const math::Mat4& )	override;
		void			Setf		 ( const ShaderHandle&, ccharptr, float32 )				override;
		void			Seti		 ( const ShaderHandle&, ccharptr, int32 )				override;
		void			SetVec4		 ( const ShaderHandle&, ccharptr, const math::Vec4& )	override;
		void			SetVec3		 ( const ShaderHandle&, ccharptr, const math::Vec3& )	override;
		void			SetVec2		 ( const ShaderHandle&, ccharptr, const math::Vec2& )	override;



		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		TextureHandle	CreateTexture2D		 ( const TextureDescriptor& )				override;
		TextureHandle	CreateTexture3D		 ( const TextureDescriptor& )				override;
		TextureHandle	CreateCubemapTexture ( const TextureCubemapDescriptor& desc )	override;
		// DEFINETLY TODO
		void			UpdateTexture		 ( const TextureHandle& tex, const TextureDescriptor& desc ) override;
		void			DeleteTexture		 ( TextureHandle& )							override;
		void			BindTexture			 ( const TextureHandle&, uint16 )			override;



		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////

		SamplerHandle	CreateSampler	( const SamplerDescriptor& )		override;
		void			BindSampler		( const SamplerHandle&, uint16)		override;
		void			DeleteSampler	( SamplerHandle )					override;



		///////////////////////////////////////////////////
		/// Pipelines
		///////////////////////////////////////////////////

		PipelineHandle		CreatePipeline	( const PipelineDescriptor& )	override;
		void				DeletePipeline	( PipelineHandle& )				override;
		void				BindPipeline	( const PipelineHandle& )		override;



		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

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
		void SetCullFace	( Face face )		override;
		void SetFrontFace	( WindingOrder )	override;


		// Blend setters
		void ToggleBlend				( bool enabled )		override;
		bool IsBlendEnabled				( )						const noexcept override;
		void SetBlendConstantColor		( ChannelRGBA rgba )	override;
		void SetBlendFactors			( BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha ) override;
		void SetBlendColorFactors		( BlendFactor srcColor, BlendFactor dstColor )	override;
		void SetBlendAlphaFactors		( BlendFactor srcAlpha, BlendFactor dstAlpha )	override;
		void SetBlendSrcColorFactor		( BlendFactor factor )							override;
		void SetBlendDstColorFactor		( BlendFactor factor )							override;
		void SetBlendSrcAlphaFactor		( BlendFactor factor )							override;
		void SetBlendDstAlphaFactor		( BlendFactor factor )							override;
		void SetBlendOp					( BlendOp colorOp, BlendOp alphaOp )			override;
		void SetBlendColorOp			( BlendOp op )									override;
		void SetBlendAlphaOp			( BlendOp op )									override;
		void SetBlendFactorsForTarget	( uint8 target )								override;
		void ToggleBlendForTarget		( uint8 target, bool enable )					override;

		// Depth setters
		void ToggleDepthWrite	 ( bool )		override;
		bool IsDepthWriteEnabled ( )			const noexcept override;

		void ToggleDepthTest	( bool )		override;
		bool IsDepthTestEnabled	( )				const noexcept override;
		void SetDepthFunc		( CompareFlag ) override;

		// Stencil setters
		void ToggleStencilTest			( bool )			override;
		bool IsStencilTestEnabled		( )					const noexcept override;
		void SetStencilReference		( int32 ref, Face )	override;
		void SetStencilOp				( StencilOp sfail, StencilOp dpfail, StencilOp dppass, Face face)	override;
		void SetStencilOpOnStencilFail	( StencilOp op, Face face )	override;
		void SetStencilOpOnDepthFail	( StencilOp op, Face face )	override;
		void SetStencilOpOnDepthPass	( StencilOp op, Face face )	override;


		// Rasterizer setters
		void ToggleDepthBias		( bool )							override;
		bool IsDepthBiasEnabled		( )									const noexcept override;
		void SetDepthBiasFactors	( float32 slope, float32 constant ) override;
		void SetDepthBiasClamp		( float32 clamp )					override;
		void SetDepthBiasSlope		( float32 )							override;
		void SetDepthBiasConstant	( float32 )							override;
		void SetTopology			( TopologyMode mode, Face face )	override;

		const BlendState&			GetBlendState		 ( ) const noexcept override;
		const CullState&			GetCullState		 ( ) const noexcept override;
		const ScissorState&			GetScissorState		 ( ) const noexcept override;
		const DepthStencilState&	GetDepthStencilState ( ) const noexcept override;
		const RasterizerState&		GetRasterizerState	 ( ) const noexcept override;
		const ViewportState&		GetViewport			 ( ) const noexcept override;

		bool IsValid ( BufferHandle handle )		const override;
		bool IsValid ( TextureHandle handle )		const override;
		bool IsValid ( ShaderHandle handle )		const override;
		bool IsValid ( FramebufferHandle handle )	const override;
		bool IsValid ( VertexLayoutHandle handle )  const override;
		bool IsValid ( PipelineHandle handle )		const override;
		bool IsValid ( SamplerHandle handle )		const override;

		void SetColorMask ( bool r, bool g, bool b, bool a )	override;
		void SetColorMask ( ColorMask rgba )					override;

		void SetClearColor	( ChannelRGBA color )	override;
		void ClearColor		( )						override;
		void ClearColor		( ChannelRGBA color )	override;
		void ClearDepth		( )						override;
		void ClearStencil	( )						override;
		void Clear			( Flags<ClearFlag> )	override;

		void SetPointSize ( float32 size )	override;
		void SetLineWidth ( float32 width )	override;

		void ToggleMultisample(bool toggle) override;
		void ToggleSampleCoverage(bool toggle) override;
		void ToggleSampleAlphaToCoverage(bool toggle) override;
		void ToggleSampleAlphaToOne(bool toggle) override;
		void SetSampleCoverage(float32 value, bool invert) override;

		void Draw ( const VertexLayoutHandle&, uint32 )	override;
		void DrawInstanced( const VertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount ) override;
		void DrawInstancedBase(const VertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount, uint32 baseInstance ) override;
		void DrawElements ( const VertexLayoutHandle&, uint32 )	override;
		void DrawElementsInstanced ( const VertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount ) override;
		void DrawElementsInstancedBase ( const VertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount, uint32 baseInstance ) override;

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
			GL_TEXTURE_1D,
			GL_TEXTURE_2D,
			GL_TEXTURE_3D,
			GL_TEXTURE_2D_ARRAY,
			GL_TEXTURE_2D_MULTISAMPLE,
			GL_TEXTURE_CUBE_MAP,
			GL_TEXTURE_CUBE_MAP_ARRAY
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

		///////////////////////////////////////////////////
		/// Private helpers ( !!! REPAIR TYPE CASTING !!! )
		///////////////////////////////////////////////////

		LUM_FORCEINLINE
		void bind_check_shader(const Pipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_rasterizer(const Pipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_depth_stencil(const Pipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_scissors(const Pipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_blend(const Pipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_cull(const Pipeline&) noexcept;

		LUM_FORCEINLINE
		void bind_check_color_mask(const Pipeline&) noexcept;



		LUM_FORCEINLINE
		bool compile_shader(GLuint shader);

		LUM_FORCEINLINE
		bool link_program(GLuint program);


		void		cache_uniforms_locations	( );
		bool		is_valid_buffer_descriptor	( const BufferDescriptor&)		noexcept;
		GLbitfield	translate_mapping_flags		( Flags<Mapflag> )				noexcept;

	};

}