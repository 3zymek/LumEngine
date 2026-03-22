//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: OpenGL implementation of Render Hardware Interface.
//
//=============================================================================//
#pragma once

#include "rhi/core/rhi_device.hpp"
#include "core/core_defines.hpp"

namespace lum::rhi::gl {

	class GLDevice : public RenderDevice {
	public:

		void Initialize( Window* window ) override;

		//=================================================
		// Buffers
		//=================================================

		RBufferHandle CreateBuffer( const FBufferDescriptor& desc ) override;

		void UpdateBuffer( RBufferHandle, cvptr, usize, usize ) override;

		void DeleteBuffer( RBufferHandle& ) override;

		vptr MapBuffer( RBufferHandle, Flags<MapFlag>, usize, usize ) override;

		void UnmapBuffer( RBufferHandle ) override;

		void SetShaderStorageBinding( RBufferHandle ssbo, uint32 binding ) override;

		void AttachElementBufferToLayout( RBufferHandle, RVertexLayoutHandle ) override;

		void SetUniformBufferBinding( RBufferHandle, int32 ) override;



		//=================================================
		// Framebuffers
		//=================================================

		RFramebufferHandle CreateFramebuffer( const FFramebufferDescriptor& ) override;

		void BlitFramebuffer( const FFramebufferBlitDescriptor& ) override;

		void ClearFramebuffer( RFramebufferHandle, ChannelRGBA, float32 ) override;

		void DeleteFramebuffer( RFramebufferHandle& ) override;

		void BindFramebuffer( RFramebufferHandle ) override;



		//=================================================
		// Layouts
		//=================================================

		RVertexLayoutHandle	CreateVertexLayout( const FVertexLayoutDescriptor&, RBufferHandle ) override;

		void DeleteVertexLayout( RVertexLayoutHandle& ) override;



		//=================================================
		// Shaders
		//=================================================

		RShaderHandle CreateShader( const FShaderDescriptor& ) override;

		void BindShader( RShaderHandle ) override;

		void DeleteShader( RShaderHandle& ) override;



		//=================================================
		// Textures
		//=================================================

		RTextureHandle CreateTexture( const FTextureDescriptor& desc ) override;

		void UnbindTexture( TextureType ) override;

		void UpdateTexture( RTextureHandle, const FTextureUpdateDescriptor& ) override;

		void DeleteTexture( RTextureHandle& ) override;

		void BindTexture( RTextureHandle, uint16 ) override;



		//=================================================
		// Samplers
		//=================================================

		RSamplerHandle CreateSampler( const FSamplerDescriptor& ) override;

		void BindSampler( RSamplerHandle, uint16 )	override;

		void DeleteSampler( RSamplerHandle ) override;



		//=================================================
		// Pipelines
		//=================================================

		RPipelineHandle CreatePipeline( const FPipelineDescriptor& ) override;

		void DeletePipeline( RPipelineHandle& ) override;

		void BindPipeline( RPipelineHandle ) override;




		//=================================================
		// Other
		//=================================================

		// Viewport setters
		void SetViewport( int32 x, int32 y, int32 width, int32 height ) override;

		void SetViewportX( int32 x ) override;

		void SetViewportY( int32 y ) override;

		void SetViewportWidth( int32 width ) override;

		void SetViewportHeight( int32 height ) override;


		// Scissors setters
		void ToggleScissors( bool ) override;

		void SetScissors( int32 x, int32 y, int32 width, int32 height ) override;

		void SetScissorX( int32 x ) override;

		void SetScissorY( int32 y ) override;

		void SetScissorWidth( int32 width ) override;

		void SetScissorHeight( int32 height )	override;



		// Cull setters
		void ToggleCull( bool ) override;

		void SetCullFace( Face face ) override;

		void SetFrontFace( WindingOrder ) override;


		// Blend setters
		void ToggleBlend( bool ) override;

		void SetBlendConstantColor( ChannelRGBA rgba ) override;

		void SetBlendFactors( BlendFactor, BlendFactor, BlendFactor, BlendFactor ) override;

		void SetBlendColorFactors( BlendFactor, BlendFactor ) override;

		void SetBlendAlphaFactors( BlendFactor, BlendFactor ) override;

		void SetBlendSrcColorFactor( BlendFactor ) override;

		void SetBlendDstColorFactor( BlendFactor ) override;

		void SetBlendSrcAlphaFactor( BlendFactor ) override;

		void SetBlendDstAlphaFactor( BlendFactor ) override;

		void SetBlendOp( BlendOp, BlendOp ) override;

		void SetBlendColorOp( BlendOp ) override;

		void SetBlendAlphaOp( BlendOp ) override;

		void SetBlendFactorsForTarget( uint8 target ) override;

		void ToggleBlendForTarget( uint8, bool ) override;


		// Depth setters
		void ToggleDepthWrite( bool ) override;

		void ToggleDepthTest( bool ) override;

		void SetDepthFunc( CompareFlag ) override;


		// Stencil setters
		void ToggleStencilTest( bool ) override;

		void SetStencilReference( int32, Face ) override;

		void SetStencilOp( StencilOp, StencilOp, StencilOp, Face ) override;

		void SetStencilOpOnStencilFail( StencilOp, Face ) override;

		void SetStencilOpOnDepthFail( StencilOp, Face ) override;

		void SetStencilOpOnDepthPass( StencilOp, Face ) override;


		// Rasterizer setters
		void ToggleDepthBias( bool ) override;

		void SetDepthBiasFactors( float32, float32 ) override;

		void SetDepthBiasClamp( float32 ) override;

		void SetDepthBiasSlope( float32 ) override;

		void SetDepthBiasConstant( float32 ) override;

		void SetTopology( TopologyMode, Face ) override;


		// Framebuffer operations
		void SetColorMask( bool, bool, bool, bool ) override;

		void SetColorMask( FColorMask ) override;

		void SetClearColor( ChannelRGBA ) override;

		void ClearColor( ) override;

		void ClearColor( ChannelRGBA ) override;

		void ClearDepth( ) override;

		void ClearStencil( ) override;

		void Clear( Flags<BufferBit> ) override;



		void SetPointSize( float32 ) override;

		void SetLineWidth( float32 ) override;

		void ToggleMultisample( bool ) override;

		void ToggleSampleCoverage( bool ) override;

		void ToggleSampleAlphaToCoverage( bool ) override;

		void ToggleSampleAlphaToOne( bool ) override;

		void SetSampleCoverage( float32, bool ) override;



		void Draw( RVertexLayoutHandle, uint32 ) override;

		void DrawInstanced( RVertexLayoutHandle, uint32, uint32 ) override;

		void DrawInstancedBase( RVertexLayoutHandle, uint32, uint32, uint32 ) override;

		void DrawElements( RVertexLayoutHandle, uint32 ) override;

		void DrawElementsInstanced( RVertexLayoutHandle, uint32, uint32 ) override;

		void DrawElementsInstancedBase( RVertexLayoutHandle, uint32, uint32, uint32 ) override;


		void SwapBuffers( ) override;


	protected:

		//=================================================
		// Lookup tables
		//=================================================

		/* @brief Maps TopologyMode to GL polygon mode (GL_POINT, GL_LINE, GL_FILL). */
		inline constexpr static GLenum skTopologyModeLookup[ ] =
		{
			GL_POINT,
			GL_LINE,
			GL_FILL
		};

		/* @brief Maps Face enum to GL face target (GL_FRONT, GL_BACK, GL_FRONT_AND_BACK). */
		inline constexpr static GLenum skFacesLookup[ ] =
		{
			GL_FRONT,
			GL_BACK,
			GL_FRONT_AND_BACK
		};

		/* @brief Maps texture minification filter modes to GL equivalents. */
		inline constexpr static GLenum skTextureMinFilterLookup[ ] =
		{
			GL_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_NEAREST,
			GL_NEAREST_MIPMAP_NEAREST,
			GL_NEAREST_MIPMAP_LINEAR
		};

		/* @brief Maps sampler wrap modes to GL wrap targets. */
		inline constexpr static GLenum skSamplerWrapLookup[ ] =
		{
			GL_REPEAT,
			GL_MIRRORED_REPEAT,
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_BORDER
		};

		/* @brief Maps CompareFlag to GL comparison functions. */
		inline constexpr static GLenum skCompareFlagLookup[ ] =
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

		/* @brief Maps BlendFactor to GL blend factor constants. */
		inline constexpr static GLenum skBlendFactorLookup[ ] =
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

		/* @brief Maps BlendOp to GL blend equation modes. */
		inline constexpr static GLenum skBlendOpLookup[ ] =
		{
			GL_FUNC_ADD,
			GL_FUNC_SUBTRACT,
			GL_FUNC_REVERSE_SUBTRACT,
			GL_MIN,
			GL_MAX
		};

		/* @brief Maps StencilOp to GL stencil operation constants. */
		inline constexpr static GLenum skStencilOpLookup[ ] =
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

		/* @brief Maps ClearFlag bits to GL clear buffer bits. */
		inline constexpr static GLenum skClearOpLookup[ ] =
		{
			GL_COLOR_BUFFER_BIT,
			GL_DEPTH_BUFFER_BIT,
			GL_STENCIL_BUFFER_BIT
		};

		/* @brief Maps TextureType to GL texture targets. */
		inline constexpr static GLenum skTextureTypeLookup[ ] =
		{
			GL_NONE,
			GL_TEXTURE_2D,
			GL_TEXTURE_3D,
			GL_TEXTURE_CUBE_MAP
		};

		/* @brief Maps ImageLayout to GL sized internal formats. */
		inline constexpr static GLenum skImageLayoutLookup[ ] =
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
			GL_DEPTH_COMPONENT16,
			GL_DEPTH_COMPONENT24,
			GL_DEPTH_COMPONENT32,
			GL_DEPTH_COMPONENT32F,
			GL_DEPTH24_STENCIL8,
			GL_DEPTH32F_STENCIL8,
			GL_STENCIL_INDEX8
		};

		/* @brief Maps image channel layout to GL base format (GL_RGBA, GL_DEPTH_COMPONENT, etc.). */
		inline constexpr static GLenum skImageFormatLookup[ ] =
		{
			GL_RGBA,
			GL_RGB,
			GL_RG,
			GL_RED,
			GL_BGRA,
			GL_DEPTH_COMPONENT,
			GL_DEPTH_STENCIL
		};

		/* @brief Maps texture data types to GL data type constants. */
		inline constexpr static GLenum skTextureDataTypeLookup[ ] =
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

		/* @brief Pointer to the engine window this device renders into. */
		Window* mWindow = nullptr;

		//=================================================
		// Private helpers
		//=================================================

		/* @brief Checks and applies shader state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_shader( const FPipeline& ) noexcept;

		/* @brief Checks and applies rasterizer state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_rasterizer( const FPipeline& ) noexcept;

		/* @brief Checks and applies depth/stencil state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_depth_stencil( const FPipeline& ) noexcept;

		/* @brief Checks and applies scissor state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_scissors( const FPipeline& ) noexcept;

		/* @brief Checks and applies blend state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_blend( const FPipeline& ) noexcept;

		/* @brief Checks and applies cull state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_cull( const FPipeline& ) noexcept;

		/* @brief Checks and applies color mask from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_color_mask( const FPipeline& ) noexcept;

		/* @brief Compiles a GLSL shader object and logs any errors.
		*  @param shader OpenGL shader handle to compile.
		*  @param name Shader name used in error messages.
		*  @return True if compilation succeeded.
		*/
		LUM_FORCEINLINE
			bool compile_shader( GLuint shader );

		/* @brief Links a GLSL program and logs any errors.
		*  @param program OpenGL program handle to link.
		*  @return True if linking succeeded.
		*/
		LUM_FORCEINLINE
			bool link_program( GLuint program );

		/* @brief Creates a 2D texture from the given descriptor. */
		RTextureHandle create_texture_2d( const FTextureDescriptor& );

		/* @brief Creates a 3D texture from the given descriptor. */
		RTextureHandle create_texture_3d( const FTextureDescriptor& );

		/* @brief Creates a cubemap texture from the given descriptor. */
		RTextureHandle create_texture_cubemap( const FTextureDescriptor& );

		/* @brief Translates engine MapFlag bits to the corresponding GL map access flags. */
		GLbitfield translate_mapping_flags( Flags<MapFlag> ) noexcept;

	};

}