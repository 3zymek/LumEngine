//========= Copyright (C) 2025-present 3zymek, MIT License ============//
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

		BufferHandle CreateBuffer( const BufferCreateInfo& desc ) override;
		void UpdateBuffer( BufferHandle buff, cvptr data, usize offset, usize size ) override;
		void Delete( BufferHandle& buff ) override;
		vptr MapBuffer( BufferHandle buff, Flags<MapFlag> flags, usize offset, usize size ) override;
		void UnmapBuffer( BufferHandle buff ) override;
		void SetShaderStorageBinding( BufferHandle ssbo, uint32 binding ) override;
		void AttachElementBufferToLayout( BufferHandle ebo, VertexLayoutHandle vao ) override;
		void SetUniformBufferBinding( BufferHandle ubo, int32 binding ) override;



		//=================================================
		// Framebuffers
		//=================================================

		FramebufferHandle CreateFramebuffer( const FramebufferCreateInfo& desc ) override;
		void AttachTextureLayer( rhi::FramebufferHandle fbo, rhi::TextureHandle tex, const TextureLayerAttachment& desc ) override;
		void BlitFramebuffer( const FramebufferBlitDescription& desc ) override;
		void ClearFramebuffer( FramebufferHandle fbo, Vector4 color, float32 depth ) override;
		void Delete( FramebufferHandle& fbo ) override;
		void BindFramebuffer( FramebufferHandle fbo ) override;



		//=================================================
		// Layouts
		//=================================================

		VertexLayoutHandle CreateVertexLayout( const VertexLayoutCreateInfo& desc, BufferHandle vbo ) override;
		void Delete( VertexLayoutHandle& layout ) override;



		//=================================================
		// Shaders
		//=================================================

		ShaderHandle CreateShader( const ShaderCreateInfo& shader ) override;
		void BindShader( ShaderHandle shader ) override;
		void Delete( ShaderHandle& shader ) override;
		void SetUniform( ShaderHandle shader, uint32 location, float32 value ) override;
		void SetUniform( ShaderHandle shader, uint32 location, uint32 value ) override;
		void SetUniform( ShaderHandle shader, uint32 location, bool value ) override;
		void SetUniform( ShaderHandle shader, uint32 location, const Vector2& value ) override;
		void SetUniform( ShaderHandle shader, uint32 location, const Vector3& value ) override;
		void SetUniform( ShaderHandle shader, uint32 location, const Vector4& value ) override;
		void SetUniform( ShaderHandle shader, uint32 location, const Matrix3& value ) override;
		void SetUniform( ShaderHandle shader, uint32 location, const Matrix4& value ) override;



		//=================================================
		// Textures
		//=================================================

		TextureHandle CreateTexture( const TextureCreateInfo& desc ) override;
		void Copy( TextureHandle src, TextureHandle dst ) override;
		void UnbindTexture( uint16 unit ) override;
		void UpdateTexture( TextureHandle tex, const TextureUpdateDescription& desc ) override;
		void Delete( TextureHandle& tex ) override;
		void BindTexture( TextureHandle tex, uint16 binding ) override;



		//=================================================
		// Samplers
		//=================================================

		SamplerHandle CreateSampler( const SamplerCreateInfo& desc ) override;
		void BindSampler( SamplerHandle sampler, uint16 binding )	override;
		void Delete( SamplerHandle sampler ) override;



		//=================================================
		// Pipelines
		//=================================================

		PipelineHandle CreatePipeline( const PipelineCreateInfo& desc ) override;
		void Delete( PipelineHandle& pipeline ) override;
		void BindPipeline( PipelineHandle pipeline ) override;



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
		void ToggleScissors( bool toggle ) override;
		void SetScissors( int32 x, int32 y, int32 width, int32 height ) override;
		void SetScissorX( int32 x ) override;
		void SetScissorY( int32 y ) override;
		void SetScissorWidth( int32 width ) override;
		void SetScissorHeight( int32 height )	override;
		

		// Cull setters
		void ToggleCull( bool toggle ) override;
		void SetCullFace( Face face ) override;
		void SetFrontFace( WindingOrder order ) override;


		// Blend setters
		void ToggleBlend( bool toggle ) override;
		void SetBlendConstantColor( Vector4 rgba ) override;
		void SetBlendFactors( BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha ) override;
		void SetBlendColorFactors( BlendFactor srcColor, BlendFactor dstColor ) override;
		void SetBlendAlphaFactors( BlendFactor srcAlpha, BlendFactor dstAlpha ) override;
		void SetBlendSrcColorFactor( BlendFactor factor ) override;
		void SetBlendDstColorFactor( BlendFactor factor ) override;
		void SetBlendSrcAlphaFactor( BlendFactor factor ) override;
		void SetBlendDstAlphaFactor( BlendFactor factor ) override;
		void SetBlendOp( BlendOp colorOp, BlendOp alphaOp ) override;
		void SetBlendColorOp( BlendOp op ) override;
		void SetBlendAlphaOp( BlendOp op ) override;
		void SetBlendFactorsForTarget( uint8 target ) override;
		void ToggleBlendForTarget( uint8 target, bool toggle ) override;


		// Depth setters
		void ToggleDepthWrite( bool toggle ) override;
		void ToggleDepthTest( bool toggle ) override;
		void SetDepthFunc( CompareFlag func ) override;
		

		// Stencil setters
		void ToggleStencilTest( bool toggle ) override;
		void SetStencilReference( int32 ref, Face face ) override;
		void SetStencilOp( StencilOp stencilFailOp, StencilOp depthFailOp, StencilOp passOp, Face face ) override;
		void SetStencilOpOnStencilFail( StencilOp op, Face face ) override;
		void SetStencilOpOnDepthFail( StencilOp op, Face face ) override;
		void SetStencilOpOnDepthPass( StencilOp op, Face face ) override;


		// Rasterizer setters
		void ToggleDepthBias( bool toggle ) override;
		void SetDepthBiasFactors( float32 slope, float32 constant ) override;
		void SetDepthBiasClamp( float32 clamp ) override;
		void SetDepthBiasSlope( float32 slope ) override;
		void SetDepthBiasConstant( float32 constant ) override;
		void SetTopology( TopologyMode mode, Face face ) override;
		void SetPointSize( float32 size ) override;
		void SetLineWidth( float32 width ) override;
		void ToggleMultisample( bool toggle ) override;
		void ToggleSampleCoverage( bool toggle ) override;
		void ToggleSampleAlphaToCoverage( bool togggle ) override;
		void ToggleSampleAlphaToOne( bool toggle ) override;
		void SetSampleCoverage( float32 value, bool invert ) override;


		// Framebuffer operations
		void SetColorMask( bool r, bool g, bool b, bool a ) override;
		void SetColorMask( ColorMask rgba ) override;
		void SetClearColor( Vector4 color ) override;
		void ClearColor( ) override;
		void ClearColor( Vector4 color ) override;
		void ClearDepth( ) override;
		void ClearStencil( ) override;
		void Clear( Flags<BufferBit> flags ) override;


		void Draw( VertexLayoutHandle layout, uint32 numVertices ) override;
		void DrawInstanced( VertexLayoutHandle layout, uint32 numVertices, uint32 numInstances ) override;
		void DrawInstancedBase( VertexLayoutHandle layout, uint32 numVertices, uint32 numInstances, uint32 baseIndex ) override;
		void DrawElements( VertexLayoutHandle layout, uint32 numIndices ) override;
		void DrawElementsInstanced( VertexLayoutHandle layout, uint32 numIndices, uint32 numInstances ) override;
		void DrawElementsInstancedBase( VertexLayoutHandle layout, uint32 numIndices, uint32 numInstances, uint32 baseIndex ) override;

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
		inline constexpr static GLenum skBufferBitLookup[ ] =
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
			void bind_check_shader( const Pipeline& pipeline ) noexcept;

		/* @brief Checks and applies rasterizer state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_rasterizer( const Pipeline& pipeline ) noexcept;

		/* @brief Checks and applies depth/stencil state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_depth_stencil( const Pipeline& pipeline ) noexcept;

		/* @brief Checks and applies scissor state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_scissors( const Pipeline& pipeline ) noexcept;

		/* @brief Checks and applies blend state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_blend( const Pipeline& pipeline ) noexcept;

		/* @brief Checks and applies cull state from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_cull( const Pipeline& pipeline ) noexcept;

		/* @brief Checks and applies color mask from the pipeline if it differs from current state. */
		LUM_FORCEINLINE
			void bind_check_color_mask( const Pipeline& pipeline ) noexcept;

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
		TextureHandle create_texture_2d( const TextureCreateInfo& desc );

		TextureHandle create_texture_2d_array( const TextureCreateInfo& desc );

		/* @brief Creates a 3D texture from the given descriptor. */
		TextureHandle create_texture_3d( const TextureCreateInfo& desc );

		/* @brief Creates a cubemap texture from the given descriptor. */
		TextureHandle create_texture_cubemap( const TextureCreateInfo& desc );

		/* @brief Translates engine MapFlag bits to the corresponding GL map access flags. */
		GLbitfield translate_mapping_flags( Flags<MapFlag> flags ) noexcept;

	};

}