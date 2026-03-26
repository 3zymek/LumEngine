//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core of Render Hardware Interface.
//          Provides a backend-agnostic GPU abstraction for resource
//          creation, pipeline state management, and draw call submission.
//
//=============================================================================//
#pragma once

#include "core/core_common.hpp"
#include "core/core_defines.hpp"
#include "core/utils/asset_loader.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/utils/flags.hpp"
#include "core/setup.hpp"

#include "render/shaders_define.h"

#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "rhi/core/rhi_shader.hpp"
#include "rhi/core/rhi_texture.hpp"
#include "rhi/core/rhi_sampler.hpp"
#include "rhi/core/rhi_framebuffer.hpp"
#include "rhi/core/rhi_pipeline.hpp"


#if LUM_ENABLE_RENDER_PROFILER == 1
#	include "rhi/rhi_profiler.hpp"
#endif

namespace lum { class Window; }
namespace lum::rhi {



	/* @brief Backend-agnostic GPU interface for resource creation and rendering.
	*
	* Abstracts over the underlying graphics API (OpenGL, Vulkan, etc.) and exposes
	* a unified interface for creating and managing GPU resources such as buffers,
	* textures, shaders, framebuffers, and pipelines.
	*
	* Tracks and caches current pipeline state (blend, cull, depth, stencil,
	* viewport, scissor) to avoid redundant API calls.
	*
	* All resource handles are generational — using a deleted handle is safe
	* and detected by validity checks.
	*
	* @note Use CreateDevice() to obtain an instance tied to a window.
	*/
	class RenderDevice {
	public:

		/* @brief Initializes the rendering device and binds it to the given window.
		*  Sets up the graphics context, surface and any backend-specific resources.
		*  Must be called before any rendering operations.
		*  @param window Pointer to the engine window to render into.
		*/
		virtual void Initialize( Window* window ) = 0;

		/* @brief Destroys all active RHI resources and shuts down the rendering device.
		* Must be called before the window is destroyed.
		*/
		void Finalize( );

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		/* @brief Creates a GPU buffer from the given descriptor.
		* @param desc Buffer descriptor specifying type, usage, and size.
		* @return Handle to the created buffer.
		*/
		LUM_NODISCARD
		virtual RBufferHandle CreateBuffer( const FBufferDescriptor& desc ) = 0;

		/* @brief Uploads data from CPU memory into a GPU buffer.
		* @param buff   Handle of the buffer to update.
		* @param data   Pointer to source data in CPU memory.
		* @param offset Byte offset from the start of the buffer (default 0).
		* @param size   Byte count to copy; 0 means whole buffer (default 0).
		*/
		virtual void UpdateBuffer( RBufferHandle buff, cvptr data, usize offset = 0, usize size = 0 ) = 0;

		/* @brief Frees the GPU memory associated with the given buffer.
		* @param buff Handle to delete. Becomes invalid after this call.
		*/
		virtual void DeleteBuffer( RBufferHandle& buff ) = 0;

		/* @brief Maps a GPU buffer into CPU address space for direct access.
		* @param buff   Handle of the buffer to map.
		* @param flags  Access flags (e.g. EMapFlag::Write, EMapFlag::Persistent).
		* @param offset Byte offset into the buffer to start mapping (default 0).
		* @param size   Byte count to map; 0 means whole buffer (default 0).
		* @return Pointer to the mapped CPU-accessible memory.
		* @note Must be followed by UnmapBuffer() when done.
		*/
		LUM_NODISCARD
		virtual vptr MapBuffer( RBufferHandle buff, Flags<MapFlag> flags, usize offset = 0, usize size = 0 ) = 0;

		/* @brief Unmaps a previously mapped buffer and synchronizes with the GPU.
		* @param buff Handle of the buffer to unmap.
		*/
		virtual void UnmapBuffer( RBufferHandle buff ) = 0;

		/* @brief Binds a shader storage buffer (SSBO) to a shader binding point.
		* @param ssbo    Handle of the SSBO.
		* @param binding Binding index matching the shader layout.
		*/
		virtual void SetShaderStorageBinding( RBufferHandle ssbo, uint32 binding ) = 0;

		/* @brief Attaches an index buffer (EBO) to a vertex layout (VAO).
		* Required before issuing indexed draw calls.
		* @param ebo Handle of the element buffer.
		* @param vao Handle of the vertex layout to attach it to.
		*/
		virtual void AttachElementBufferToLayout( RBufferHandle ebo, RVertexLayoutHandle vao ) = 0;

		/* @brief Binds a uniform buffer (UBO) to a shader binding point.
		* @param ubo     Handle of the uniform buffer.
		* @param binding Binding index matching the shader uniform block.
		*/
		virtual void SetUniformBufferBinding( RBufferHandle ubo, int32 binding ) = 0;


		///////////////////////////////////////////////////
		/// Framebuffers
		///////////////////////////////////////////////////

		/* @brief Creates an empty framebuffer object.
		* @return Handle to the created framebuffer.
		*/
		LUM_NODISCARD
		virtual RFramebufferHandle CreateFramebuffer( const FFramebufferDescriptor& desc ) = 0;

		virtual void AttachCubemapFace( RFramebufferHandle fbo, RTextureHandle tex, int32 face ) = 0;

		/* @brief Clears color and depth buffers of the given framebuffer.
		* @param fbo   Handle of the framebuffer to clear.
		* @param color RGBA clear color.
		* @param depth Depth clear value (clamped to [0.0, 1.0]).
		*/
		virtual void ClearFramebuffer( RFramebufferHandle fbo, ChannelRGBA color, float32 depth ) = 0;

		/* @brief Copies a region of one framebuffer to another.
		*  @param desc Descriptor specifying source, destination, regions, buffer mask and filter.
		*/
		virtual void BlitFramebuffer(const FFramebufferBlitDescriptor& desc) = 0;

		/* @brief Destroys the framebuffer and releases its GPU resources.
		* @param fbo Handle to delete. Becomes invalid after this call.
		*/
		virtual void DeleteFramebuffer( RFramebufferHandle& fbo ) = 0;

		/* @brief Sets the active framebuffer for all subsequent draw operations.
		* @param fbo Handle of the framebuffer to bind.
		*/
		virtual void BindFramebuffer( RFramebufferHandle fbo ) = 0;


		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		/* @brief Creates a vertex layout (VAO) and connects it to a vertex buffer.
		* @param desc Descriptor specifying vertex attribute layout.
		* @param vbo  Vertex buffer to associate with this layout.
		* @return Handle to the created vertex layout.
		*/
		LUM_NODISCARD
		virtual RVertexLayoutHandle CreateVertexLayout( const FVertexLayoutDescriptor& desc, RBufferHandle vbo ) = 0;

		/* @brief Destroys a vertex layout and releases its GPU resources.
		* @param layout Handle to delete. Becomes invalid after this call.
		*/
		virtual void DeleteVertexLayout( RVertexLayoutHandle& layout ) = 0;


		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		/* @brief Compiles and creates a shader from the given descriptor.
		* @param desc Descriptor containing vertex and fragment source file paths.
		* @return Handle to the created shader.
		*/
		LUM_NODISCARD
		virtual RShaderHandle CreateShader( const FShaderDescriptor& desc ) = 0;

		/* @brief Binds a shader for use in subsequent draw or dispatch calls.
		* @param shader Handle of the shader to bind.
		*/
		virtual void BindShader( RShaderHandle shader ) = 0;

		/* @brief Destroys a shader and releases its GPU resources.
		* @param shader Handle to delete. Becomes invalid after this call.
		*/
		virtual void DeleteShader( RShaderHandle& shader ) = 0;


		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		/* @brief Creates a GPU texture from the given descriptor.
		* @param desc Texture descriptor specifying format, size, and filtering.
		* @return Handle to the created texture.
		*/
		LUM_NODISCARD
		virtual RTextureHandle CreateTexture( const FTextureDescriptor& desc ) = 0;

		/* @brief Unbinds any texture of the given type from the pipeline.
		* @param type Texture type to unbind (e.g. Texture2D, Cubemap).
		*/
		virtual void UnbindTexture( TextureType type ) = 0;

		/* @brief Updates the contents of an existing texture.
		* @param tex  Handle of the texture to update.
		* @param desc Update descriptor specifying region, format, and data.
		*/
		virtual void UpdateTexture( RTextureHandle tex, const FTextureUpdateDescriptor& desc ) = 0;

		/* @brief Destroys a texture and frees its GPU memory.
		* @param texture Handle to delete. Becomes invalid after this call.
		*/
		virtual void DeleteTexture( RTextureHandle& texture ) = 0;

		/* @brief Binds a texture to a shader slot.
		* Skips the bind if the texture is already bound to the given slot (cached).
		* @param texture Handle of the texture to bind.
		* @param binding Shader binding slot index.
		*/
		virtual void BindTexture( RTextureHandle texture, uint16 binding ) = 0;


		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////

		/* @brief Creates a texture sampler from the given descriptor.
		* @param desc Sampler descriptor specifying filtering and wrap modes.
		* @return Handle to the created sampler.
		*/
		LUM_NODISCARD
		virtual RSamplerHandle CreateSampler( const FSamplerDescriptor& desc ) = 0;

		/* @brief Binds a sampler to a shader slot.
		* @param sampler Handle of the sampler to bind.
		* @param binding Shader binding slot index.
		*/
		virtual void BindSampler( RSamplerHandle sampler, uint16 binding ) = 0;

		/* @brief Destroys a sampler and releases its GPU resources.
		* @param sampler Handle to delete. Becomes invalid after this call.
		*/
		virtual void DeleteSampler( RSamplerHandle sampler ) = 0;


		///////////////////////////////////////////////////
		/// Pipelines
		///////////////////////////////////////////////////

		/* @brief Creates a pipeline state object from the given descriptor.
		* @param desc Descriptor specifying shaders, blend, depth, and rasterizer state.
		* @return Handle to the created pipeline.
		*/
		LUM_NODISCARD
		virtual RPipelineHandle CreatePipeline( const FPipelineDescriptor& desc ) = 0;

		/* @brief Destroys a pipeline and releases its GPU resources.
		* @param pipeline Handle to delete. Becomes invalid after this call.
		*/
		virtual void DeletePipeline( RPipelineHandle& pipeline ) = 0;

		/* @brief Binds a pipeline for use in subsequent draw calls.
		* @param pipeline Handle of the pipeline to bind.
		*/
		virtual void BindPipeline( RPipelineHandle pipeline ) = 0;


		///////////////////////////////////////////////////
		/// Viewport
		///////////////////////////////////////////////////

		/* @brief Sets the full viewport rectangle.
		* @param x      Left coordinate in pixels.
		* @param y      Bottom coordinate in pixels.
		* @param width  Width in pixels.
		* @param height Height in pixels.
		*/
		virtual void SetViewport( int32 x, int32 y, int32 width, int32 height ) = 0;

		/* @brief Sets only the X coordinate of the viewport. */
		virtual void SetViewportX( int32 x ) = 0;

		/* @brief Sets only the Y coordinate of the viewport. */
		virtual void SetViewportY( int32 y ) = 0;

		/* @brief Sets only the width of the viewport. */
		virtual void SetViewportWidth( int32 width ) = 0;

		/* @brief Sets only the height of the viewport. */
		virtual void SetViewportHeight( int32 height ) = 0;


		///////////////////////////////////////////////////
		/// Scissor
		///////////////////////////////////////////////////

		/* @brief Enables or disables the scissor test.
		* When enabled, fragments outside the scissor rectangle are discarded.
		* Commonly used for UI clipping, split-screen, and render region masking.
		* @param toggle True to enable, false to disable.
		*/
		virtual void ToggleScissors( bool toggle ) = 0;

		/* @brief Returns whether the scissor test is currently enabled. */
		bool IsScissorEnabled( ) const noexcept { return mEnabledStates.Has(State::Scissor); }

		/* @brief Sets the full scissor rectangle.
		* @param x      Left coordinate in pixels.
		* @param y      Bottom coordinate in pixels.
		* @param width  Width in pixels.
		* @param height Height in pixels.
		*/
		virtual void SetScissors( int32 x, int32 y, int32 width, int32 height ) = 0;

		/* @brief Sets only the X coordinate of the scissor rectangle. */
		virtual void SetScissorX( int32 x ) = 0;

		/* @brief Sets only the Y coordinate of the scissor rectangle. */
		virtual void SetScissorY( int32 y ) = 0;

		/* @brief Sets only the width of the scissor rectangle. */
		virtual void SetScissorWidth( int32 width ) = 0;

		/* @brief Sets only the height of the scissor rectangle. */
		virtual void SetScissorHeight( int32 height ) = 0;


		///////////////////////////////////////////////////
		/// Culling
		///////////////////////////////////////////////////

		/* @brief Enables or disables face culling.
		* When enabled, back-facing primitives are discarded before rasterization.
		* Reduces overdraw on opaque geometry.
		* @param toggle True to enable, false to disable.
		*/
		virtual void ToggleCull( bool toggle ) = 0;

		/* @brief Returns whether face culling is currently enabled. */
		bool IsCullEnabled( ) const noexcept { return mEnabledStates.Has(State::Cull); }

		/* @brief Selects which polygon faces are culled.
		* @param face Cull face selection (Front, Back, or FrontBack).
		*/
		virtual void SetCullFace( Face face ) = 0;

		/* @brief Sets the front-face winding order.
		* @param order Winding order (Clockwise or CounterClockwise).
		*/
		virtual void SetFrontFace( WindingOrder order ) = 0;


		///////////////////////////////////////////////////
		/// Blending
		///////////////////////////////////////////////////

		/* @brief Enables or disables color blending.
		* When enabled, fragment output is blended with the current render target.
		* @param toggle True to enable, false to disable.
		*/
		virtual void ToggleBlend( bool toggle ) = 0;

		/* @brief Returns whether color blending is currently enabled. */
		bool IsBlendEnabled( ) const noexcept { return mEnabledStates.Has(State::Blend); }

		/* @brief Sets the constant RGBA color used in constant-factor blend modes. */
		virtual void SetBlendConstantColor( ChannelRGBA rgba ) = 0;

		/* @brief Sets blend factors for both color and alpha channels.
		* @param srcColor Source factor for RGB.
		* @param dstColor Destination factor for RGB.
		* @param srcAlpha Source factor for alpha.
		* @param dstAlpha Destination factor for alpha.
		*/
		virtual void SetBlendFactors( BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha ) = 0;

		/* @brief Sets blend factors for RGB channels only. Alpha factors remain unchanged.
		* @param srcColor Source factor for RGB.
		* @param dstColor Destination factor for RGB.
		*/
		virtual void SetBlendColorFactors( BlendFactor srcColor, BlendFactor dstColor ) = 0;

		/* @brief Sets blend factors for alpha channel only. RGB factors remain unchanged.
		* @param srcAlpha Source factor for alpha.
		* @param dstAlpha Destination factor for alpha.
		*/
		virtual void SetBlendAlphaFactors( BlendFactor srcAlpha, BlendFactor dstAlpha ) = 0;

		/* @brief Sets the source blend factor for RGB channels. */
		virtual void SetBlendSrcColorFactor( BlendFactor factor ) = 0;

		/* @brief Sets the destination blend factor for RGB channels. */
		virtual void SetBlendDstColorFactor( BlendFactor factor ) = 0;

		/* @brief Sets the source blend factor for alpha channel. */
		virtual void SetBlendSrcAlphaFactor( BlendFactor factor ) = 0;

		/* @brief Sets the destination blend factor for alpha channel. */
		virtual void SetBlendDstAlphaFactor( BlendFactor factor ) = 0;

		/* @brief Sets the blend operation for both color and alpha channels.
		* @param colorOp Blend operation for RGB (e.g. Add, Subtract, Min, Max).
		* @param alphaOp Blend operation for alpha.
		*/
		virtual void SetBlendOp( BlendOp colorOp, BlendOp alphaOp ) = 0;

		/* @brief Sets the blend operation for RGB channels only. */
		virtual void SetBlendColorOp( BlendOp op ) = 0;

		/* @brief Sets the blend operation for alpha channel only. */
		virtual void SetBlendAlphaOp( BlendOp op ) = 0;

		/* @brief Sets blend factors for a specific render target index. */
		virtual void SetBlendFactorsForTarget( uint8 target ) = 0;

		/* @brief Enables or disables blending for a specific render target index. */
		virtual void ToggleBlendForTarget( uint8 target, bool enable ) = 0;


		///////////////////////////////////////////////////
		/// Depth
		///////////////////////////////////////////////////

		/* @brief Enables or disables depth buffer writes.
		* Depth testing may still occur when writes are disabled.
		* @param toggle True to enable, false to disable.
		*/
		virtual void ToggleDepthWrite( bool toggle ) = 0;

		/* @brief Returns whether depth buffer writes are currently enabled. */
		bool IsDepthWriteEnabled( ) const noexcept { return mEnabledStates.Has(State::DepthWrite); }

		/* @brief Enables or disables depth testing.
		* Failing fragments are discarded before blending.
		* @param enable True to enable, false to disable.
		*/
		virtual void ToggleDepthTest( bool enable ) = 0;

		/* @brief Returns whether depth testing is currently enabled. */
		bool IsDepthTestEnabled( ) const noexcept { return mEnabledStates.Has(State::DepthTest); }

		/* @brief Sets the depth comparison function.
		* @param func Comparison function (e.g. Less, LessOrEqual, Always).
		*/
		virtual void SetDepthFunc( CompareFlag func ) = 0;


		///////////////////////////////////////////////////
		/// Stencil
		///////////////////////////////////////////////////

		/* @brief Enables or disables stencil testing.
		* Used for masking, outlining, shadow volumes, and portal rendering.
		* @param toggle True to enable, false to disable.
		*/
		virtual void ToggleStencilTest( bool toggle ) = 0;

		/* @brief Returns whether stencil testing is currently enabled. */
		bool IsStencilTestEnabled( ) const noexcept { return mEnabledStates.Has(State::StencilTest); }

		/* @brief Sets the reference value used in stencil comparisons.
		* @param ref  Reference value (typically 0-255).
		* @param face Polygon faces this applies to (Front, Back, or FrontBack).
		*/
		virtual void SetStencilReference( int32 ref, Face face = Face::FrontBack ) = 0;

		/* @brief Sets stencil buffer operations for all test outcomes.
		* @param stencilFailOp Operation when stencil test fails.
		* @param depthFailOp   Operation when stencil passes but depth fails.
		* @param passOp        Operation when both tests pass.
		* @param face          Polygon faces this applies to.
		*/
		virtual void SetStencilOp( StencilOp stencilFailOp, StencilOp depthFailOp, StencilOp passOp, Face face = Face::FrontBack ) = 0;

		/* @brief Sets the stencil operation applied when the stencil test fails.
		* @param op   Operation to apply.
		* @param face Polygon faces this applies to.
		*/
		virtual void SetStencilOpOnStencilFail( StencilOp op, Face face = Face::FrontBack ) = 0;

		/* @brief Sets the stencil operation applied when the depth test fails.
		* @param op   Operation to apply.
		* @param face Polygon faces this applies to.
		*/
		virtual void SetStencilOpOnDepthFail( StencilOp op, Face face = Face::FrontBack ) = 0;
		
		/* @brief Sets the stencil operation applied when both tests pass.
		* @param op   Operation to apply.
		* @param face Polygon faces this applies to.
		*/
		virtual void SetStencilOpOnDepthPass( StencilOp op, Face face = Face::FrontBack ) = 0;


		///////////////////////////////////////////////////
		/// Rasterizer
		///////////////////////////////////////////////////

		/* @brief Enables or disables depth bias (polygon offset).
		* Used to prevent z-fighting in shadow mapping and decals.
		* @param toggle True to enable, false to disable.
		*/
		virtual void ToggleDepthBias( bool toggle ) = 0;

		/* @brief Returns whether depth bias is currently enabled. */
		bool IsDepthBiasEnabled( ) const noexcept { return mEnabledStates.Has(State::DepthBias); }

		/* @brief Sets depth bias scale factors. Offset = slope * dZ + constant.
		* @param slope    Scale factor applied to the polygon's depth slope.
		* @param constant Constant offset scaled by an implementation-specific value.
		*/
		virtual void SetDepthBiasFactors( float32 slope, float32 constant ) = 0;

		/* @brief Sets the maximum depth bias clamp.
		* Positive clamps from above, negative from below. Zero = no clamp.
		* @param clamp Maximum depth offset limit.
		*/
		virtual void SetDepthBiasClamp( float32 clamp ) = 0;

		/* @brief Sets only the slope scale factor for depth bias. */
		virtual void SetDepthBiasSlope( float32 slopeFactor ) = 0;

		/* @brief Sets only the constant factor for depth bias. */
		virtual void SetDepthBiasConstant( float32 constantBias ) = 0;

		/* @brief Sets the polygon rasterization mode.
		* @param mode Rasterization mode (Point, Line, or Fill).
		* @param face Polygon faces this applies to (default FrontBack).
		*/
		virtual void SetTopology( TopologyMode mode, Face face = Face::FrontBack ) = 0;

		/* @brief Sets the size of rendered points in pixels. */
		virtual void SetPointSize( float32 size ) = 0;

		/* @brief Sets the width of rendered lines in pixels. */
		virtual void SetLineWidth( float32 width ) = 0;

		/* @brief Enables or disables multisampling. */
		virtual void ToggleMultisample( bool toggle ) = 0;

		/* @brief Enables or disables sample coverage. */
		virtual void ToggleSampleCoverage( bool toggle ) = 0;

		/* @brief Enables or disables alpha-to-coverage sampling. */
		virtual void ToggleSampleAlphaToCoverage( bool toggle ) = 0;

		/* @brief Enables or disables alpha-to-one sampling. */
		virtual void ToggleSampleAlphaToOne( bool toggle ) = 0;

		/* @brief Sets the sample coverage value and inversion flag. */
		virtual void SetSampleCoverage( float32 value, bool invert ) = 0;


		///////////////////////////////////////////////////
		/// State Queries
		///////////////////////////////////////////////////

		/* @brief Returns the current blend state. */
		const FBlendState& GetBlendState( ) const noexcept { return mBlendState; }

		/* @brief Returns the current face culling state. */
		const FCullState& GetCullState( ) const noexcept { return mCullState; }

		/* @brief Returns the current scissor state. */
		const FScissorState& GetScissorState( ) const noexcept { return mScissorState; }

		/* @brief Returns the current depth and stencil state. */
		const FDepthStencilState& GetDepthStencilState( ) const noexcept { return mDepthStencilState; }

		/* @brief Returns the current rasterizer state. */
		const FRasterizerState& GetRasterizerState( ) const noexcept { return mRasterizerState; }

		/* @brief Returns the current viewport state. */
		const FViewportState& GetViewport( ) const noexcept { return mViewportState; }


		///////////////////////////////////////////////////
		/// Handle Validation
		///////////////////////////////////////////////////

		/* @brief Returns true if the buffer handle refers to a live resource. */
		bool IsValid( RBufferHandle handle ) const noexcept { return mBuffers.Contains(handle); }

		/* @brief Returns true if the texture handle refers to a live resource. */
		bool IsValid( RTextureHandle handle ) const noexcept { return mTextures.Contains(handle); }

		/* @brief Returns true if the shader handle refers to a live resource. */
		bool IsValid( RShaderHandle handle ) const noexcept { return mShaders.Contains(handle); }

		/* @brief Returns true if the framebuffer handle refers to a live resource. */
		bool IsValid( RFramebufferHandle handle ) const noexcept { return mFramebuffers.Contains(handle); }

		/* @brief Returns true if the vertex layout handle refers to a live resource. */
		bool IsValid( RVertexLayoutHandle handle ) const noexcept { return mLayouts.Contains(handle); }

		/* @brief Returns true if the pipeline handle refers to a live resource. */
		bool IsValid( RPipelineHandle handle ) const noexcept { return mPipelines.Contains(handle); }

		/* @brief Returns true if the sampler handle refers to a live resource. */
		bool IsValid( RSamplerHandle handle ) const noexcept { return mSamplers.Contains(handle); }


		uint32 GetNativeHandle( RBufferHandle handle ) noexcept {
			if (mBuffers.Contains( handle ))
				return mBuffers[ handle ].mHandle;
			return {};
		}
		uint32 GetNativeHandle( RTextureHandle handle ) noexcept {
			if (mTextures.Contains( handle ))
				return mTextures[ handle ].mHandle;
			return {};
		}
		uint32 GetNativeHandle( RShaderHandle handle ) noexcept {
			if (mShaders.Contains( handle ))
				return mShaders[ handle ].mHandle;
			return {};
		}
		uint32 GetNativeHandle( RFramebufferHandle handle ) noexcept {
			if (mFramebuffers.Contains( handle ))
				return mFramebuffers[ handle ].mHandle;
			return {};
		}
		uint32 GetNativeHandle( RVertexLayoutHandle handle ) noexcept {
			if (mLayouts.Contains( handle ))
				return mLayouts[ handle ].mHandle;
			return {};
		}
		uint32 GetNativeHandle( RSamplerHandle handle ) noexcept {
			if (mSamplers.Contains( handle ))
				return mSamplers[ handle ].mHandle;
			return {};
		}


		///////////////////////////////////////////////////
		/// Color & Clear
		///////////////////////////////////////////////////

		/* @brief Sets which RGBA channels are written to the framebuffer.
		* @param r, g, b, a Per-channel write enable flags.
		*/
		virtual void SetColorMask( bool r, bool g, bool b, bool a ) = 0;

		/* @brief Sets which RGBA channels are written to the framebuffer.
		* @param rgba ColorMask structure containing all channel flags.
		*/
		virtual void SetColorMask( FColorMask rgba ) = 0;

		/* @brief Sets the RGBA color used for subsequent ClearColor() calls. */
		virtual void SetClearColor( ChannelRGBA color ) = 0;

		/* @brief Clears the color buffer using the current clear color. */
		virtual void ClearColor( ) = 0;

		/* @brief Sets the clear color and immediately clears the color buffer. */
		virtual void ClearColor( ChannelRGBA color ) = 0;

		/* @brief Clears the depth buffer to its default value (1.0). */
		virtual void ClearDepth( ) = 0;

		/* @brief Clears the stencil buffer to zero. */
		virtual void ClearStencil( ) = 0;

		/* @brief Clears the specified buffers in a single operation.
		* @param flags Combination of EClearFlag values (Color, Depth, Stencil).
		*/
		virtual void Clear( Flags<BufferBit> flags ) = 0;


		///////////////////////////////////////////////////
		/// Draw Calls
		///////////////////////////////////////////////////

		/* @brief Draws non-indexed geometry.
		* @param layout         Vertex layout to draw.
		* @param numVertices Number of vertices.
		*/
		virtual void Draw( RVertexLayoutHandle layout, uint32 numVertices) = 0;

		/* @brief Draws non-indexed geometry with instancing.
		* @param layout           Vertex layout to draw.
		* @param numVertices   Number of vertices per instance.
		* @param numInstances Number of instances.
		*/
		virtual void DrawInstanced( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances ) = 0;

		/* @brief Draws non-indexed geometry with instancing and a base instance offset.
		* @param layout        Vertex layout to draw.
		* @param numVertices   Number of vertices per instance.
		* @param numInstances  Number of instances.
		* @param baseIndex  Starting instance index.
		*/
		virtual void DrawInstancedBase( RVertexLayoutHandle layout, uint32 numVertices, uint32 numInstances, uint32 baseIndex ) = 0;

		/* @brief Draws indexed geometry.
		* @param layout       Vertex layout with attached EBO.
		* @param indicesCount Number of indices.
		*/
		virtual void DrawElements( RVertexLayoutHandle layout, uint32 numIndices ) = 0;

		/* @brief Draws indexed geometry with instancing.
		* @param layout        Vertex layout with attached EBO.
		* @param numIndices    Number of indices per instance.
		* @param numInstances  Number of instances.
		*/
		virtual void DrawElementsInstanced( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances) = 0;

		/* @brief Draws indexed geometry with instancing and a base instance offset.
		* @param layout        Vertex layout with attached EBO.
		* @param numIndices    Number of indices per instance.
		* @param numInstances  Number of instances.
		* @param baseIndex     Starting instance index.
		*/
		virtual void DrawElementsInstancedBase( RVertexLayoutHandle layout, uint32 numIndices, uint32 numInstances, uint32 baseIndex ) = 0;

		/* @brief Ends the current frame and presents the result. */
		virtual void SwapBuffers( ) = 0;


#		if LUM_ENABLE_RENDER_PROFILER == 1
			inline void GetProfilerInfo( ) {
				LUM_LOG_INFO("Cache hit rate: %f", mProfiler.GetCacheHitRate());
			}
#		endif

	protected:

		inline constexpr static uint32 skMaxTextureUnits = 32;
		inline constexpr static uint32 skMaxSamplerUnits = 32;

		std::array<RTextureHandle, skMaxTextureUnits> mCurrentTextures{};
		std::array<RSamplerHandle, skMaxSamplerUnits> mCurrentSamplers{};

		RShaderHandle		mCurrentShader{};
		RFramebufferHandle	mCurrentFramebuffer{};
		RPipelineHandle		mCurrentPipeline{};

		FBlendState			mBlendState{};
		FCullState			mCullState{};
		FScissorState		mScissorState{};
		FDepthStencilState	mDepthStencilState{};
		FRasterizerState	mRasterizerState{};
		FViewportState		mViewportState{};
		FColorMask			mColorMask{};

		Flags<State>		mEnabledStates{};
		ChannelRGBA			mClearColor{};

#		if LUM_ENABLE_RENDER_PROFILER == 1
		performance::Profiler mProfiler{};

#			define LUM_PROFILER_DRAW_CALL()		mProfiler.RegisterDrawCall()
#			define LUM_PROFILER_CACHE_MISS()	mProfiler.RegisterCacheMiss()
#			define LUM_PROFILER_CACHE_HIT()		mProfiler.RegisterCacheHit()

#		else

#			define LUM_PROFILER_BEGIN_FRAME()	((void)0)
#			define LUM_PROFILER_END_FRAME()		((void)0)
#			define LUM_PROFILER_DRAW_CALL()		((void)0)
#			define LUM_PROFILER_CACHE_MISS()	((void)0)
#			define LUM_PROFILER_CACHE_HIT()		((void)0)

#		endif

		inline constexpr static uint32 skMaxShaders			= 8;
		inline constexpr static uint32 skMaxSamplers		= 500;
		inline constexpr static uint32 skMaxBuffers			= 10000;
		inline constexpr static uint32 skMaxLayouts			= 10000;
		inline constexpr static uint32 skMaxTextures		= 1000;
		inline constexpr static uint32 skMaxFramebuffers	= 100;
		inline constexpr static uint32 skMaxPipelines		= 100;

		cstd::HandlePool<RSamplerHandle, FSampler, RSamplerID>				mSamplers{ skMaxSamplers };
		cstd::HandlePool<RShaderHandle, FShader, RShaderID>					mShaders{ skMaxShaders };
		cstd::HandlePool<RBufferHandle, FBuffer, RBufferID>					mBuffers{ skMaxBuffers };
		cstd::HandlePool<RVertexLayoutHandle, FVertexLayout, RLayoutID>		mLayouts{ skMaxLayouts };
		cstd::HandlePool<RTextureHandle, FTexture, RTextureID>				mTextures{ skMaxTextures };
		cstd::HandlePool<RFramebufferHandle, FFramebuffer, RFramebufferID>	mFramebuffers{ skMaxFramebuffers };
		cstd::HandlePool<RPipelineHandle, FPipeline, RPipelineID>			mPipelines{ skMaxPipelines };


		bool validate_framebuffer_handle( RFramebufferHandle buff ) const noexcept { return IsValid(buff) || (buff == gDefaultFramebuffer); }

		/* @brief Validates a texture descriptor before resource creation.
		*  @param desc Texture descriptor to validate.
		*  @return True if the descriptor is valid, false otherwise.
		*/
		bool validate_texture_descriptor( const FTextureDescriptor& ) const noexcept;

		/* @brief Validates a buffer descriptor before resource creation.
		*  @param desc Buffer descriptor to validate.
		*  @return True if the descriptor is valid, false otherwise.
		*/
		bool validate_buffer_descriptor( const FBufferDescriptor& ) const noexcept;

		/* @brief Returns true if the given image layout is a depth format. */
		bool is_depth_format( ImageLayout fmt ) const noexcept;

		/* @brief Returns true if the given image layout is a stencil format. */
		bool is_stencil_format( ImageLayout fmt ) const noexcept;

		/* @brief Returns true if the given image layout is a color format. */
		bool is_color_format( ImageLayout fmt ) const noexcept;

	};

	/* @brief Creates a render device tied to the given window.
	* Initializes the underlying graphics backend.
	* @param backend Backend used.
	* @return Pointer to the created device.
	*/
	RenderDevice* CreateDevice( RenderBackend backend );

} // namespace lum::rhi