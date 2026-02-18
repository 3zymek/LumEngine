//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core of Render Hardware Interface
// 
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/asset_loader.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/shaders_define.h"
#include "core/utils/flags.hpp"
#include "core/setup.hpp"

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

	/*====================================================================
	*  @brief Low-level GPU interface for resource creation and rendering.
	*
	*  @brief RHI_Device provides functions to create, bind, and manage GPU resources
	*  such as textures, samplers, shaders, and buffers. It also handles
	*  binding resources to the pipeline for drawing or compute operations.
	*
	*  @brief All operations are GPU-aware and should be used according to the
	*  current command buffer / pipeline state.
	*
	*/
	class RDevice {
	public:

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		/*====================================================================
		* @brief Creates a vertex buffer (VBO).
		*
		* Allocates a GPU buffer to store vertex data. The buffer is described
		* by a BufferDescriptor containing size, usage flags, and optional initial data.
		*
		* @param desc Descriptor of the buffer to create.
		* @return Handle to the created vertex buffer.
		*
		* @note The returned handle is used for subsequent operations like
		*       updating, mapping, or binding to a vertex layout.
		*/
		LUM_NODISCARD
		virtual RBufferHandle CreateVertexBuffer ( const RBufferDescriptor& desc ) = 0;

		/*====================================================================
		* @brief Creates an element/index buffer (EBO).
		*
		* Allocates a GPU buffer for storing index data for indexed drawing.
		*
		* @param desc Descriptor of the buffer to create.
		* @return Handle to the created element buffer.
		*
		* @note The buffer can later be attached to a vertex layout for indexed draw calls.
		*/
		LUM_NODISCARD
		virtual RBufferHandle CreateElementBuffer ( const RBufferDescriptor& desc ) = 0;

		/*====================================================================
		* @brief Creates a uniform buffer (UBO).
		*
		* Allocates a GPU buffer for storing uniform data that shaders can access.
		*
		* @param desc Descriptor of the uniform buffer to create.
		* @return Handle to the created uniform buffer.
		*
		* @note Binding to shaders is done using SetUniformBufferBinding().
		*/
		LUM_NODISCARD
		virtual RBufferHandle CreateUniformBuffer ( const RBufferDescriptor& desc ) = 0;

		// TODO implement
		LUM_NODISCARD
		virtual RBufferHandle CreateShaderStorageBuffer ( const RBufferDescriptor& desc ) = 0;

		/*====================================================================
		* @brief Updates the contents of a GPU buffer.
		*
		* Copies data from CPU memory into a GPU buffer. Can update the full buffer
		* or a subrange specified by offset and size.
		*
		* @param buff Handle of the buffer to update.
		* @param data Pointer to source data in CPU memory.
		* @param offset Byte offset from the beginning of the buffer (default 0).
		* @param size Byte size of data to copy (default 0 = whole buffer).
		*
		* @note Use this for dynamic updates of vertex, element, or uniform buffers.
		*/
		virtual void UpdateBuffer ( const RBufferHandle& buff, cvptr data, usize offset = 0, usize size = 0 ) = 0;

		/*====================================================================
		* @brief Deletes a GPU buffer.
		*
		* Frees the GPU memory associated with the buffer handle.
		*
		* @param buff Handle of the buffer to delete.
		*
		* @note After deletion, the handle becomes invalid and must not be used.
		*/
		virtual void DeleteBuffer ( RBufferHandle& buff ) = 0;

		/*====================================================================
		* @brief Maps a GPU buffer into CPU address space.
		*
		* Allows direct CPU access to buffer memory for reading or writing.
		*
		* @param buff Handle of the buffer to map.
		* @param flags Mapping flags specifying access type (e.g., rhi::map_flags::Write).
		* @param offset Byte offset from the beginning of the buffer to map (default 0).
		* @param size Byte size of memory to map (default 0 = whole buffer).
		* @return Pointer to mapped GPU memory.
		*
		* @note Call UnmapBuffer() after finishing CPU access.
		*/
		LUM_NODISCARD
		virtual vptr MapBuffer ( const RBufferHandle& buff, Flags<RMapFlag> flags, usize offset = 0, usize size = 0 ) = 0;

		/*====================================================================
		* @brief Unmaps a previously mapped GPU buffer.
		*
		* Completes CPU access and ensures data is synchronized with the GPU.
		*
		* @param buff Handle of the buffer to unmap.
		*
		* @note Always call after MapBuffer() to avoid undefined behavior.
		*/
		virtual void UnmapBuffer ( const RBufferHandle& buff ) = 0;

		virtual void SetShaderStorageBinding(const RBufferHandle& ssbo, uint32 binding) = 0;

		/*====================================================================
		* @brief Attaches an element buffer (EBO) to a vertex layout (VAO).
		*
		* Binds the index buffer to a vertex array object, enabling indexed draw calls.
		*
		* @param ebo Handle of the element buffer to attach.
		* @param vao Handle of the vertex layout to attach the buffer to.
		*
		* @note Must be done before issuing draw calls that use indices.
		*/
		virtual void AttachElementBufferToLayout ( const RBufferHandle& ebo, const RVertexLayoutHandle& vao ) = 0;

		/*====================================================================
		* @brief Binds a uniform buffer (UBO) to a shader binding point.
		*
		* Connects a GPU uniform buffer to a specific binding slot in the shader.
		*
		* @param ubo Handle of the uniform buffer to bind.
		* @param binding Shader binding point (e.g., LUM_UBO_CAMERA).
		*
		* @note The shader must define a corresponding uniform block at this binding point.
		*/
		virtual void SetUniformBufferBinding ( const RBufferHandle& ubo, int32 binding ) = 0;






		///////////////////////////////////////////////////
		/// Framebuffers
		///////////////////////////////////////////////////

		/*====================================================================
		*  @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer ( 0 for whole buffer ).
		*  @param size Byte size of data to write ( 0 for whole buffer ).
		*
		*/
		LUM_NODISCARD
		virtual RFramebufferHandle CreateFramebuffer ( ) = 0;

		/*====================================================================
		* @brief Creates a texture for use in a framebuffer.
		*
		*  Allocates a 2D texture based on the given descriptor. The texture format
		*  depends on the type of attachment: depth or color.
		*
		*  @param desc Specifies the width, height, and attachment type of the texture.
		*
		*  @return A handle representing the created texture. Returns an empty handle
		*          if the requested size is invalid or the maximum number of textures is reached.
		*/
		LUM_NODISCARD
		virtual RTextureHandle CreateFramebufferTexture ( const RFramebufferTextureDescriptor& desc ) = 0;

		// TO FIX:
		virtual void SetFramebufferColorTexture ( const RFramebufferHandle& fbo, const RTextureHandle& tex, uint8 index ) = 0;
		// TO IMPLEMENT:
		virtual void SetFramebufferDepthTexture ( const RFramebufferHandle& fbo, const RTextureHandle& tex ) = 0;
		virtual void SetFramebufferStencilTexture ( const RFramebufferHandle& fbo, const RTextureHandle& tex ) = 0;

		/*====================================================================
		* @brief Clears the specified framebuffer with the given color and depth values.
		*
		* This function sets the clear color and depth, then clears the color and depth
		* buffers of the framebuffer. The framebuffer is automatically bound during this operation.
		*
		* @param fbo Handle to the framebuffer to clear.
		* @param color RGBA color used to clear the color buffer.
		* @param depth Depth value used to clear the depth buffer (clamped between 0.0 and 1.0).
		*/
		virtual void ClearFramebuffer ( RFramebufferHandle fbo, ChannelRGBA color, float32 depth ) = 0;

		/*====================================================================
		* @brief Deletes the specified framebuffer and releases its resources.
		*	
		* After calling this function, the framebuffer handle becomes invalid and should
		* not be used in subsequent rendering operations.
		* 
		* @param buff Handle to the framebuffer to delete.
		*
		*/
		virtual void DeleteFramebuffer ( RFramebufferHandle& fbo ) = 0;

		/*====================================================================
		* @brief Sets the current framebuffer for rendering.
		*
		* This makes the given framebuffer the target for all subsequent draw operations.
		* 
		* @param buff Handle to the framebuffer to bind.
		*
		*/
		virtual void BindFramebuffer ( const RFramebufferHandle& fbo ) = 0;

		/*====================================================================
		* @brief Unbinds any currently bound framebuffer.
		*
		*  After calling this, rendering will target the default framebuffer
		*  (typically the screen) instead of any previously bound framebuffer.
		*/
		virtual void UnbindFramebuffer ( ) = 0;





		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		/*====================================================================
		* @brief Creates Vertex layout (VAO).
		*
		*  @param desc Descriptor.
		*  @param vbo Vertex buffer to connect.
		* 
		*  @return Handle to vertex layout.
		*
		*/
		LUM_NODISCARD
		virtual RVertexLayoutHandle CreateVertexLayout ( const RVertexLayoutDescriptor& desc, const RBufferHandle& vbo ) = 0;
		
		/*====================================================================
		* @brief Deletes vertex layout.
		*
		*  @param layout Layout handle to delete.
		*
		*/
		virtual void DeleteVertexLayout ( RVertexLayoutHandle& layout ) = 0;





		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		/*====================================================================
		* @brief Creates a shader from a descriptor.
		*
		*  Uses vertex/fragment sources from the descriptor.
		*  @param desc ShaderDescriptor with source file names.
		*  @return Handle to the created shader.
		* 
		*/
		LUM_NODISCARD
		virtual RShaderHandle CreateShader ( const RShaderDescriptor& desc) = 0;
		
		/*====================================================================
		* @brief Binds a shader for rendering.
		*
		*  The bound shader will be used on next draw/dispatch.
		*  @param shader Shader handle to bind.
		* 
		*/
		virtual void BindShader ( const RShaderHandle& shader ) = 0;
		
		/*====================================================================
		* @brief Deletes a shader.
		*
		*  Frees GPU resources and invalidates the handle.
		*  @param shader Shader handle to delete.
		* 
		*/
		virtual void DeleteShader ( RShaderHandle& shader ) = 0;
		
		/*====================================================================
		* @brief Sets a 4x4 matrix uniform in a shader.
		* 
		*  @param shader Shader handle.
		*  @param location Name of the uniform.
		*  @param mat Matrix to set.
		* 
		*/
		virtual void SetMat4 ( const RShaderHandle& shader, ccharptr location, const math::Mat4& mat ) = 0;
		
		/*====================================================================
		* @brief Sets a float uniform in a shader.
		*/
		virtual void Setf ( const RShaderHandle& shader, ccharptr location, float32 value )	= 0;
		
		/*====================================================================
		* @brief Sets an int uniform in a shader.
		*/
		virtual void Seti ( const RShaderHandle& shader, ccharptr location, int32 value )	= 0;
		
		/*====================================================================
		* @brief Sets a vec4 uniform in a shader.
		*/
		virtual void SetVec4 ( const RShaderHandle& shader, ccharptr location, const math::Vec4& vec ) = 0;
		
		/*====================================================================
		* @brief Sets a vec3 uniform in a shader.
		*/
		virtual void SetVec3 ( const RShaderHandle& shader, ccharptr location, const math::Vec3& vec ) = 0;
		
		/*====================================================================
		* @brief Sets a vec2 uniform in a shader.
		*/
		virtual void SetVec2 ( const RShaderHandle& shader, ccharptr location, const math::Vec2& vec ) = 0;




		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		/*====================================================================
		* @brief Creates a 2D texture.
		*
		*  Uses the descriptor to initialize GPU texture.
		*  @param desc Texture properties and data.
		*  @return Handle to the created texture.
		* 
		*/
		LUM_NODISCARD 
		virtual RTextureHandle CreateTexture2D ( const RTextureDescriptor& desc ) = 0;

		/*====================================================================
		* @brief Creates a 3D texture.
		*
		*  Uses the descriptor to initialize GPU 3D texture.
		*  @param desc Texture properties and data.
		*  @return Handle to the created texture.
		* 
		*/
		LUM_NODISCARD 
		virtual RTextureHandle CreateTexture3D ( const RTextureDescriptor& desc ) = 0;
		
		LUM_NODISCARD
		virtual RTextureHandle CreateCubemapTexture ( const RTextureCubemapDescriptor& desc ) = 0;

		virtual void UnbindTexture ( RTextureType type ) = 0;
		virtual void UpdateTexture ( const RTextureHandle& tex, const RTextureUpdateDescriptor& desc ) = 0;

		/*====================================================================
		* @brief Deletes a texture.
		*
		*  Frees GPU memory and invalidates the handle.
		*  @param texture Handle of the texture to delete.
		* 
		*/
		virtual void DeleteTexture ( RTextureHandle& texture ) = 0;

		/*====================================================================
		* @brief Binds a texture to a given binding slot.
		*
		* This function associates the specified texture with a shader binding slot.
		* If no binding is provided, a default or previously assigned binding is used.
		* Each implementation of the rendering backend must define how the binding is applied.
		*
		* @param texture Handle to the texture to bind.
		* @param binding Optional binding slot index for the texture. If not provided,
		*                a default or pre-assigned slot will be used (LUM_NULL_BINDING).
		*
		*/
		virtual void BindTexture ( const RTextureHandle& texture, uint16 binding ) = 0;





		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////
		
		/*====================================================================
		* @brief Creates Sampler.
		*
		*  @param desc SamplerDescriptor of sampler.
		*
		*/
		LUM_NODISCARD
		virtual RSamplerHandle CreateSampler ( const RSamplerDescriptor& desc ) = 0;
		virtual void BindSampler ( const RSamplerHandle& sampler, uint16 binding ) = 0;
		virtual void DeleteSampler ( RSamplerHandle sampler ) = 0;




		///////////////////////////////////////////////////
		/// Pipelines
		///////////////////////////////////////////////////

		virtual RPipelineHandle CreatePipeline ( const RPipelineDescriptor& desc ) = 0;
		virtual void DeletePipeline ( RPipelineHandle& pipeline ) = 0;
		virtual void BindPipeline ( const RPipelineHandle& pipeline ) = 0;


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		/*====================================================================
		* @brief Sets the active viewport.
		*
		* Defines the transformation from normalized device coordinates
		* to window-space pixels for subsequent draw calls.
		*
		* The viewport remains bound until explicitly changed.
		*
		* @param x Left coordinate of the viewport.
		* @param y Bottom coordinate of the viewport.
		* @param width Width of the viewport.
		* @param height Height of the viewport.
		*/
		virtual void SetViewport ( int32 x, int32 y, int32 width, int32 height ) = 0;

		/*====================================================================
		* @brief Sets the left coordinate of the viewport.
		*
		* Updates only the X coordinate while preserving the current Y position,
		* width, and height of the viewport.
		*
		* The viewport defines the transformation from normalized device coordinates
		* to window-space pixels. This change takes effect immediately for all
		* subsequent draw calls.
		*
		* @param x Left coordinate of the viewport in screen-space pixels.
		*/
		virtual void SetViewportX ( int32 x ) = 0;

		/*====================================================================
		* @brief Sets the bottom coordinate of the viewport.
		*
		* Updates only the Y coordinate while preserving the current X position,
		* width, and height of the viewport.
		*
		* The viewport defines the transformation from normalized device coordinates
		* to window-space pixels. This change takes effect immediately for all
		* subsequent draw calls.
		*
		* @param y Bottom coordinate of the viewport in screen-space pixels.
		*/
		virtual void SetViewportY ( int32 y ) = 0;

		/*====================================================================
		* @brief Sets the width of the viewport.
		*
		* Updates only the viewport width while preserving the current X and Y
		* coordinates and height.
		*
		* The viewport defines the transformation from normalized device coordinates
		* to window-space pixels. This change takes effect immediately for all
		* subsequent draw calls.
		*
		* @param width Width of the viewport in pixels. Must be greater than 0.
		*/
		virtual void SetViewportWidth ( int32 width ) = 0;

		/*====================================================================
		* @brief Sets the height of the viewport.
		*
		* Updates only the viewport height while preserving the current X and Y
		* coordinates and width.
		*
		* The viewport defines the transformation from normalized device coordinates
		* to window-space pixels. This change takes effect immediately for all
		* subsequent draw calls.
		*
		* @param height Height of the viewport in pixels. Must be greater than 0.
		*/
		virtual void SetViewportHeight ( int32 height ) = 0;


		/*====================================================================
		* @brief Enables or disables the scissor test.
		*
		* When enabled, fragments outside the defined scissor rectangle
		* are discarded during rasterization.
		* State is persistent until changed.
		*
		* Commonly used for UI, split-screen, and render region clipping.
		*
		* @param toggle True to enable the scissor test, false to disable it.
		*/
		virtual void ToggleScissors ( bool toggle ) = 0;

		/*====================================================================
		* @brief Checks whether the scissor test is currently enabled.
		*
		* Returns the current state of the scissor test without modifying it.
		* When the scissor test is enabled, fragments outside the defined scissor
		* rectangle are discarded during rasterization.
		*
		* This query reflects the state set by the most recent ToggleScissor() call.
		*
		* @return True if scissor testing is enabled, false otherwise.
		*/
		virtual bool IsScissorEnabled ( ) const noexcept = 0;

		/*====================================================================
		* @brief Sets the active scissor rectangle.
		*
		* Defines the rectangular region, in screen-space pixels,
		* that limits fragment rendering when the scissor test is enabled.
		*
		* The rectangle remains bound until explicitly changed.
		*
		* @param x Left coordinate of the scissor rectangle.
		* @param y Bottom coordinate of the scissor rectangle.
		* @param width Width of the scissor rectangle.
		* @param height Height of the scissor rectangle.
		*/
		virtual void SetScissors ( int32 x, int32 y, int32 width, int32 height ) = 0;
		
		/*====================================================================
		* @brief Sets the left coordinate of the scissor rectangle.
		*
		* Updates only the X coordinate while preserving the current Y position,
		* width, and height of the scissor rectangle.
		*
		* The scissor test must be enabled via ToggleScissor() for this to take effect.
		* Fragments outside the scissor rectangle will be discarded during rasterization.
		*
		* The setting persists until explicitly changed.
		*
		* @param x Left coordinate of the scissor rectangle in screen-space pixels.
		*/
		virtual void SetScissorX ( int32 x ) = 0;
		
		/*====================================================================
		* @brief Sets the bottom coordinate of the scissor rectangle.
		*
		* Updates only the Y coordinate while preserving the current X position,
		* width, and height of the scissor rectangle.
		*
		* The scissor test must be enabled via ToggleScissor() for this to take effect.
		* Fragments outside the scissor rectangle will be discarded during rasterization.
		*
		* The setting persists until explicitly changed.
		*
		* @param y Bottom coordinate of the scissor rectangle in screen-space pixels.
		*/
		virtual void SetScissorY ( int32 y ) = 0;
		
		/*====================================================================
		* @brief Sets the width of the scissor rectangle.
		*
		* Updates only the width while preserving the current X and Y coordinates
		* and height of the scissor rectangle.
		*
		* The scissor test must be enabled via ToggleScissor() for this to take effect.
		* Fragments outside the scissor rectangle will be discarded during rasterization.
		*
		* The setting persists until explicitly changed.
		*
		* @param width Width of the scissor rectangle in pixels. Must be greater than 0.
		*/
		virtual void SetScissorWidth ( int32 width ) = 0;

		/*====================================================================
		* @brief Sets the height of the scissor rectangle.
		*
		* Updates only the height while preserving the current X and Y coordinates
		* and width of the scissor rectangle.
		*
		* The scissor test must be enabled via ToggleScissor() for this to take effect.
		* Fragments outside the scissor rectangle will be discarded during rasterization.
		*
		* The setting persists until explicitly changed.
		*
		* @param height Height of the scissor rectangle in pixels. Must be greater than 0.
		*/
		virtual void SetScissorHeight ( int32 height ) = 0;



		// Cull setters

		/*====================================================================
		* @brief Enables or disables face culling.
		*
		* Controls whether the rasterizer discards back-facing (or front-facing,
		* depending on current cull mode) primitives.
		* State is persistent until changed.
		*
		* Used to reduce overdraw and improve rendering performance.
		*
		* @param toggle True to enable face culling, false to disable it.
		*/
		virtual void ToggleCull ( bool toggle ) = 0;

		/*====================================================================
		* @brief Checks whether face culling is currently enabled.
		*
		* Returns the current state of face culling without modifying it.
		* When face culling is enabled, the rasterizer discards primitives based
		* on their winding order and the configured cull mode.
		*
		* This query reflects the state set by the most recent ToggleCull() call.
		*
		* @return True if face culling is enabled, false otherwise.
		*/
		virtual bool IsCullEnabled ( ) const noexcept = 0;

		/*====================================================================
		* @brief Selects which polygon faces are culled.
		*
		* Controls the rasterizer culling mode used to discard primitives
		* based on their winding order.
		*
		* Requires face culling to be enabled.
		*
		* @param face Cull face selection (Front or Back).
		*/
		virtual void SetCullFace ( RFace face ) = 0;

		/*====================================================================
		* @brief Sets the front-face winding order.
		*
		* Defines which vertex winding order is considered front-facing
		* during rasterization.
		*
		* Used in combination with face culling to determine which
		* primitives are discarded.
		*
		* @param order Vertex winding order defining front-facing polygons
		*              (e.g. Clockwise or CounterClockwise).
		*/
		virtual void SetFrontFace ( RWindingOrder ) = 0;




		// Blend setters

		/*====================================================================
		* @brief Enables or disables color blending.
		*
		* Controls whether fragment shader outputs are blended with the
		* current contents of the render target.
		*
		* Affects all subsequent draw calls until the state is changed.
		*
		* Commonly used for transparency, particles, and UI rendering.
		*
		* @param toggle True to enable blending, false to disable it.
		*/
		virtual void ToggleBlend ( bool toggle ) = 0;

		/*====================================================================
		* @brief Checks whether color blending is currently enabled.
		*
		* Returns the current state of blending without modifying it.
		* When blending is enabled, fragment shader outputs are combined with
		* the current contents of the render target according to the configured
		* blend factors and operations.
		*
		* This query reflects the state set by the most recent ToggleBlend() call.
		*
		* @return True if blending is enabled, false otherwise.
		*/
		virtual bool IsBlendEnabled ( ) const noexcept = 0;

		/*====================================================================
		* @brief Sets the constant blend color used in blend operations.
		*
		* Defines the RGBA color value used when blend factors are set to
		* ConstantColor or OneMinusConstantColor. This color acts as a fixed
		* reference value in the blend equation, independent of source and
		* destination colors.
		*
		* The setting persists until explicitly changed.
		*
		* Common use cases:
		* - Uniform transparency effects (fade entire scene to black)
		* - Color tinting (multiply all fragments by a constant color)
		* - Custom blend modes requiring a fixed reference color
		*
		* Example: Fade to black effect
		* @code
		* SetBlendConstantColor({0.0f, 0.0f, 0.0f, 0.5f});
		* SetBlendFactors(ConstantAlpha, OneMinusConstantAlpha, One, Zero);
		* @endcode
		*
		* @param rgba The constant RGBA color to use in blending operations.
		*/
		virtual void SetBlendConstantColor ( ChannelRGBA rgba ) = 0;
		
		/*====================================================================
		* @brief Sets all blend factors for both color and alpha channels.
		*
		* Configures how source and destination colors are combined during blending.
		* The source factor is multiplied with the fragment shader output, and the
		* destination factor is multiplied with the current framebuffer color.
		* The results are then combined using the blend operation.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* Blend factors persist until explicitly changed.
		*
		* Common combinations:
		* - Alpha blending: (SrcAlpha, OneMinusSrcAlpha, One, OneMinusSrcAlpha)
		* - Additive: (One, One, One, One)
		* - Multiplicative: (DstColor, Zero, DstAlpha, Zero)
		*
		* @param srcColor Source blend factor for RGB channels.
		* @param dstColor Destination blend factor for RGB channels.
		* @param srcAlpha Source blend factor for alpha channel.
		* @param dstAlpha Destination blend factor for alpha channel.
		*/
		virtual void SetBlendFactors ( RBlendFactor srcColor, RBlendFactor dstColor, RBlendFactor srcAlpha, RBlendFactor dstAlpha ) = 0;

		/*====================================================================
		* @brief Sets blend factors for color channels only.
		*
		* Configures how source and destination RGB values are weighted during blending.
		* Alpha channel blend factors remain unchanged.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Useful when you need different blending behavior for color and alpha,
		* or when updating only color blending without affecting alpha.
		*
		* @param srcColor Source blend factor for RGB channels.
		* @param dstColor Destination blend factor for RGB channels.
		*/
		virtual void SetBlendColorFactors ( RBlendFactor srcColor, RBlendFactor dstColor ) = 0;

		/*====================================================================
		* @brief Sets blend factors for alpha channel only.
		*
		* Configures how source and destination alpha values are weighted during blending.
		* RGB channel blend factors remain unchanged.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Commonly used to preserve alpha coverage in transparent materials
		* while using different color blending.
		*
		* @param srcAlpha Source blend factor for alpha channel.
		* @param dstAlpha Destination blend factor for alpha channel.
		*/
		virtual void SetBlendAlphaFactors ( RBlendFactor srcAlpha, RBlendFactor dstAlpha ) = 0;

		/*====================================================================
		* @brief Sets the blend factor for the source color component.
		*
		* Configures how the source RGB values are weighted during color blending.
		* This controls the multiplier applied to the incoming fragment's color
		* before combining it with the destination color in the framebuffer.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Commonly used in combination with SetBlendDstColorFactor() to achieve
		* effects like transparency, additive blending, or multiplicative blending. 
		*
		* @param factor Source blend factor for RGB channels.
		*/
		virtual void SetBlendSrcColorFactor ( RBlendFactor factor ) = 0;

		/*====================================================================
		* @brief Sets the blend factor for the destination color component.
		*
		* Configures how the destination RGB values (already in framebuffer) are
		* weighted during color blending. This controls the multiplier applied to
		* the existing color before combining it with the incoming fragment's color.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Commonly paired with SetBlendSrcColorFactor() to control how new pixels
		* blend with existing framebuffer content.
		*
		* @param factor Destination blend factor for RGB channels.
		*/
		virtual void SetBlendDstColorFactor ( RBlendFactor factor ) = 0;

		/*====================================================================
		* @brief Sets the blend factor for the source alpha component.
		*
		* Configures how the source alpha value is weighted during alpha blending.
		* This controls the multiplier applied to the incoming fragment's alpha
		* before combining it with the destination alpha in the framebuffer.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Allows independent control of alpha blending separate from color blending,
		* useful for preserving alpha coverage while using different color blend modes.
		*
		* @param factor Source blend factor for alpha channel.
		*/
		virtual void SetBlendSrcAlphaFactor ( RBlendFactor factor ) = 0;

		/*====================================================================
		* @brief Sets the blend factor for the destination alpha component.
		*
		* Configures how the destination alpha value (already in framebuffer) is
		* weighted during alpha blending. This controls the multiplier applied to
		* the existing alpha before combining it with the incoming fragment's alpha.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Commonly used with SetBlendSrcAlphaFactor() to control alpha composition
		* independently from RGB color blending.
		*
		* @param factor Destination blend factor for alpha channel.
		*/
		virtual void SetBlendDstAlphaFactor ( RBlendFactor factor ) = 0;

		/*====================================================================
		* @brief Sets the blend operation for color and alpha channels.
		*
		* Defines how the source and destination values (after applying blend factors)
		* are mathematically combined to produce the final color.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The blend operation persists until explicitly changed.
		*
		* @param colorOp Blend operation for RGB channels (e.g., Add, Subtract, Min, Max).
		* @param alphaOp Blend operation for alpha channel (e.g., Add, Subtract, Min, Max).
		*/
		virtual void SetBlendOp ( RBlendOp colorOp, RBlendOp alphaOp ) = 0;

		/*====================================================================
		* @brief Sets the blend operation for RGB color channels.
		*
		* Configures the mathematical operation used to combine source and destination
		* RGB values after they have been multiplied by their respective blend factors.
		* This determines how the weighted colors are actually merged together.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Common operations include ADD (standard blending), SUBTRACT (darkening effects),
		* REVERSE_SUBTRACT (inverted darkening), MIN, and MAX.
		*
		* The final color is computed as: ColorOp(SrcColor * SrcFactor, DstColor * DstFactor)
		*
		* @param op Blend operation for RGB channels.
		*/
		virtual void SetBlendColorOp ( RBlendOp op ) = 0;

		/*====================================================================
		* @brief Sets the blend operation for alpha channel.
		*
		* Configures the mathematical operation used to combine source and destination
		* alpha values after they have been multiplied by their respective blend factors.
		* This determines how the weighted alpha values are actually merged together.
		*
		* Blending must be enabled via toggle_blend() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Allows independent control of alpha composition separate from color blending.
		* Useful for specialized alpha coverage techniques or compositing operations.
		*
		* The final alpha is computed as: AlphaOp(SrcAlpha * SrcFactor, DstAlpha * DstFactor)
		*
		* @param op Blend operation for alpha channel.
		*/
		virtual void SetBlendAlphaOp ( RBlendOp op ) = 0;

		virtual void SetBlendFactorsForTarget(uint8 target) = 0;
		virtual void ToggleBlendForTarget(uint8 target, bool enable) = 0;



		// Depth setters

		/*====================================================================
		* @brief Enables or disables depth buffer writes.
		*
		* Controls whether fragment depth values are written to the
		* depth buffer during depth testing.
		*
		* Depth testing may still occur when depth writes are disabled.
		*
		* State is persistent until changed.
		*	
		* Commonly disabled for transparent objects and overlays.
		*
		* @param toggle True to enable depth writes, false to disable them.
		*/
		virtual void ToggleDepthWrite ( bool toggle ) = 0;
		
		/*====================================================================
		* @brief Checks whether depth buffer writes are currently enabled.
		*
		* Returns the current state of depth writing without modifying it.
		* When depth writes are enabled, fragment depth values are written to
		* the depth buffer. Depth testing may still occur even when writes
		* are disabled.
		*
		* This query reflects the state set by the most recent ToggleDepthWrite() call.
		*
		* @return True if depth writes are enabled, false otherwise.
		*/
		virtual bool IsDepthWriteEnabled ( ) const noexcept = 0;

		/*====================================================================
		* @brief Enables or disables depth testing.
		*
		* When enabled, the depth test compares each fragment's depth value against
		* the current depth buffer value using the comparison function set by SetDepthFunc().
		* Fragments that fail the test are discarded before blending.
		*
		* State is persistent until changed.
		*
		* Essential for correct rendering of 3D scenes with overlapping geometry.
		*
		* @param enable True to enable depth testing, false to disable it.
		*/
		virtual void ToggleDepthTest ( bool enable ) = 0;

		/*====================================================================
		* @brief Checks whether depth testing is currently enabled.
		*
		* Returns the current state of depth testing without modifying it.
		* When depth testing is enabled, each fragment's depth value is compared
		* against the current depth buffer value using the configured comparison
		* function. Fragments that fail the test are discarded.
		*
		* This query reflects the state set by the most recent ToggleDepthTest() call.
		*
		* @return True if depth testing is enabled, false otherwise.
		*/
		virtual bool IsDepthTestEnabled ( ) const noexcept = 0;

		/*====================================================================
		* @brief Sets the depth comparison function.
		*
		* Determines the condition under which a fragment passes the depth test.
		* The test compares the incoming fragment's depth value against the value
		* currently stored in the depth buffer.
		*
		* Depth testing must be enabled via toggle_depth_test() before this takes effect.
		* The comparison function remains active until explicitly changed.
		*
		* Common modes: Less (default, typical 3D), LessOrEqual, Greater, Always.
		*
		* @param func Comparison function used for depth testing
		*             (e.g., CompareFlag::Less, CompareFlag::LessOrEqual).
		*/
		virtual void SetDepthFunc ( RCompareFlag func ) = 0;




		// Stencil setters

		/*====================================================================
		* @brief Enables or disables stencil testing.
		*
		* When enabled, the stencil test compares a reference value against the value
		* stored in the stencil buffer using a comparison function. Fragments that fail
		* the test are discarded before color and depth processing.
		*
		* State is persistent until changed.
		*
		* Commonly used for masking, outlining, shadow volumes, and portal rendering.
		*
		* @param toggle True to enable stencil testing, false to disable it.
		*/
		virtual void ToggleStencilTest ( bool toggle ) = 0;

		/*====================================================================
		* @brief Queries whether stencil testing is currently enabled.
		*
		* Returns true if stencil testing is active and fragments are being
		* compared against the stencil buffer according to the set stencil state.
		* Returns false if stencil testing is disabled and all fragments bypass
		* the stencil stage.
		*
		* Useful for checking render state before performing operations that
		* depend on stencil masking, like outlining, shadow volumes, or portal rendering.
		*
		* @return True if stencil test is enabled, false otherwise.
		*/
		virtual bool IsStencilTestEnabled ( ) const noexcept = 0;

		/*====================================================================
		* @brief Sets the reference value for stencil testing and operations.
		*
		* Defines the reference value used in stencil comparison functions and
		* write operations. This value is compared against stencil buffer contents
		* when stencil testing is enabled.
		*
		* Can be set separately for front-facing and back-facing polygons, or
		* applied to both simultaneously.
		*
		* Stencil testing must be enabled via ToggleStencilTest() before this takes effect.
		* The reference value persists until explicitly changed.
		*
		* @param ref Reference value for stencil comparisons (typically 0-255).
		* @param face Specifies which polygon faces the reference applies to
		*             (Front, Back, or FrontBack).
		*/
		virtual void SetStencilReference ( int32 ref, RFace face = RFace::FrontBack ) = 0;

		/*====================================================================
		* @brief Sets stencil buffer operations for stencil and depth test outcomes.
		*
		* Defines how the stencil buffer is modified depending on the result of
		* stencil and depth tests during fragment processing.
		*
		* The operations are evaluated in the following order:
		* 1. Stencil test
		* 2. Depth test (if stencil test passes)
		*
		* Each parameter specifies the operation applied to the stencil value
		* under a specific condition.
		*
		* Can be configured separately for front-facing and back-facing polygons,
		* or applied to both simultaneously.
		*
		* Stencil testing must be enabled via ToggleStencilTest() for this to take effect.
		* The operations persist until explicitly changed.
		*
		* @param stencilFailOp Operation applied when the stencil test fails.
		* @param depthFailOp   Operation applied when the stencil test passes but the depth test fails.
		* @param passOp        Operation applied when both stencil and depth tests pass.
		* @param face          Specifies which polygon faces the operations apply to
		*                      (Front, Back, or FrontBack).
		*/
		virtual void SetStencilOp ( RStencilOp stencilFailOp, RStencilOp depthFailOp, RStencilOp passOp, RFace face = RFace::FrontBack ) = 0;

		/*====================================================================
		* @brief Sets the stencil operation executed when the stencil test fails.
		*
		* Defines how the stencil buffer value is modified when the stencil comparison
		* function fails for a fragment.
		*
		* This operation is applied before any depth testing occurs.
		*
		* Can be set separately for front-facing and back-facing polygons, or
		* applied to both simultaneously.
		*
		* Stencil testing must be enabled via ToggleStencilTest() for this to take effect.
		* The operation persists until explicitly changed.
		*
		* @param op   Stencil operation to apply when the stencil test fails.
		* @param face Specifies which polygon faces the operation applies to
		*             (Front, Back, or FrontBack).
		*/
		virtual void SetStencilOpOnStencilFail ( RStencilOp op, RFace face = RFace::FrontBack ) = 0;

		/*====================================================================
		* @brief Sets the stencil operation executed when the depth test fails.
		*
		* Defines how the stencil buffer value is modified when the stencil test
		* passes but the depth test fails.
		*
		* This operation is evaluated after a successful stencil test
		* but before fragment output.
		*
		* Can be set separately for front-facing and back-facing polygons, or
		* applied to both simultaneously.
		*
		* Stencil testing must be enabled via ToggleStencilTest() for this to take effect.
		* The operation persists until explicitly changed.
		*
		* @param op   Stencil operation to apply when the depth test fails.
		* @param face Specifies which polygon faces the operation applies to
		*             (Front, Back, or FrontBack).
		*/
		virtual void SetStencilOpOnDepthFail ( RStencilOp op, RFace face = RFace::FrontBack ) = 0;

		/*====================================================================
		* @brief Sets the stencil operation executed when both stencil and depth tests pass.
		*
		* Defines how the stencil buffer value is modified when a fragment
		* passes both the stencil comparison and the depth test.
		*
		* This operation is typically used to increment, decrement, or replace
		* stencil values after successful rendering.
		*
		* Can be set separately for front-facing and back-facing polygons, or
		* applied to both simultaneously.
		*
		* Stencil testing must be enabled via ToggleStencilTest() for this to take effect.
		* The operation persists until explicitly changed.
		*
		* @param op   Stencil operation to apply when both tests pass.
		* @param face Specifies which polygon faces the operation applies to
		*             (Front, Back, or FrontBack).
		*/
		virtual void SetStencilOpOnDepthPass ( RStencilOp op, RFace face = RFace::FrontBack ) = 0;


		// Rasterizer setters

		/*====================================================================
		* @brief Enables or disables depth bias during rasterization.
		*
		* When enabled, applies the currently configured depth bias
		* to subsequent draw calls. State is persistent until changed.
		*
		* Used to prevent depth fighting (e.g. shadow mapping, decals).
		*
		* @param toggle True to enable depth bias, false to disable it.
		*/
		virtual void ToggleDepthBias ( bool toggle ) = 0;
		
		/*====================================================================
		* @brief Checks if depth bias (polygon offset) is currently enabled.
		*
		* Query function to determine whether depth bias calculations are active.
		* Depth bias must be enabled before setting bias factors or clamp values.
		*
		* @return True if depth bias is enabled, false otherwise.
		*/
		virtual bool IsDepthBiasEnabled ( ) const noexcept = 0;

		/*====================================================================
		* @brief Sets the depth bias scale factors for polygon offset.
		*
		* Configures how depth values are offset to prevent z-fighting artifacts.
		* The actual offset is calculated as: offset = slope * dZ + constant,
		* where dZ is the depth slope of the polygon.
		*
		* @param slope Scale factor applied to polygon's depth slope. The initial value is 0.
		* @param constant Constant depth offset multiplied by implementation-specific value. The initial value is 0.
		*/
		virtual void SetDepthBiasFactors ( float32 slope, float32 constant ) = 0;

		/*====================================================================
		* @brief Sets the maximum depth offset clamp value.
		*
		* Limits the calculated depth bias to prevent excessive offset values.
		* Positive values clamp from above, negative values clamp from below.
		* If zero, no clamping is applied. Requires depth bias to be enabled.
		*
		* @param clamp Maximum (or minimum if negative) depth offset limit. The initial value is 0.
		*/
		virtual void SetDepthBiasClamp ( float32 clamp ) = 0;

		/*====================================================================
		*  @brief Sets the scale and units used to calculate depth values.
		*
		*  Keeps the binding until changed. Used by shaders on next draw/dispatch.
		*
		*  @param slopFactor Specifies a scale factor that is used to create a variable depth offset for each polygon.
		*  @param constantBias Is multiplied by an implementation-specific value to create a constant depth offset.
		*/
		virtual void SetDepthBiasSlope ( float32 slopeFactor ) = 0;
		
		/*====================================================================
		* @brief Sets the constant depth bias offset.
		*
		* Defines a fixed depth offset added to each fragment, independent of polygon slope.
		* The value is scaled by an implementation-specific constant that guarantees a minimum
		* resolvable difference in depth buffer values.
		*	
		* Depth bias must be enabled via ToggleDepthBias() before this takes effect.
		* The setting persists until explicitly changed.
		*
		* Used in combination with slope factor to fine-tune depth offset behavior.
		*
		* @param constantBias Implementation-scaled constant depth offset.
		*                     Typical values range from 1.0 to 10.0 for shadows.
		*/
		virtual void SetDepthBiasConstant ( float32 constantBias ) = 0;

		/*====================================================================
		*  @brief Controls the interpretation of polygons for rasterization.
		*	face describes which polygons mode applies to: 
		*	both front and back-facing polygons.
		*	The polygon mode affects only the final rasterization of polygons. 
		*	In particular, a polygon's vertices are lit and the polygon is clipped and possibly culled before these modes are applied. 
		*
		*
		*  @param mode Specifies how polygons will be rasterized.
		*	The initial value is TopologyMode::Fill for both front- and back-facing polygons. 
		*  @param face Specifies the polygons that mode applies to. 
		*	The initial value is Face::FrontBack. 
		*/
		virtual void SetTopology ( RTopologyMode mode, RFace face = RFace::FrontBack ) = 0;

		/*====================================================================
		* @brief Sets the size of rendered points.
		*
		* Specifies the diameter of points when rendering with point topology.
		* Has effect only when the rasterizer is configured for point rendering.
		*
		* @param size Point size in pixels.
		*/
		virtual void SetPointSize ( float32 size ) = 0;

		/*====================================================================
		* @brief Sets the width of rendered lines.
		*
		* Specifies the thickness of lines when rendering with line topology.
		* Has effect only when the rasterizer is configured for line rendering.
		*
		* @param width Line width in pixels.
		*/
		virtual void SetLineWidth ( float32 width ) = 0;

		virtual void ToggleMultisample(bool toggle) = 0;
		virtual void ToggleSampleCoverage(bool toggle) = 0;
		virtual void ToggleSampleAlphaToCoverage(bool toggle) = 0;
		virtual void ToggleSampleAlphaToOne(bool toggle) = 0;
		virtual void SetSampleCoverage(float32 value, bool invert) = 0;



		/*====================================================================
		* @brief Returns the current blend state.
		*
		* Provides read-only access to the blend configuration, including
		* blend enable, source/destination factors, and blend operations.
		* Useful for checking if transparent objects, particle systems, or
		* post-processing effects are configured correctly.
		*
		* @return Reference to the current BlendState.
		*/
		virtual const RBlendState& GetBlendState ( ) const noexcept = 0;

		/*====================================================================
		* @brief Returns the current face culling state.
		*
		* Allows querying which faces are culled (front/back) and the winding order.
		* Useful for validating render state before drawing opaque geometry or
		* performing custom cull logic.
		*
		* @return Reference to the current CullState.
		*/
		virtual const RCullState& GetCullState ( ) const noexcept = 0;

		/*====================================================================
		* @brief Returns the current scissor state.
		*
		* Provides access to scissor rectangle configuration.
		* Fragments outside the scissor rect are discarded.
		* Commonly used for UI clipping, minimap rendering, or split-screen.
		*
		* @return Reference to the current ScissorState.
		*/
		virtual const RScissorState& GetScissorState ( ) const noexcept = 0;

		/*====================================================================
		* @brief Returns the current depth and stencil state.
		*
		* Provides access to depth test, depth write, stencil enable,
		* and stencil operations.
		* Useful for debugging rendering issues involving occlusion,
		* shadow volumes, or stencil masking.
		*
		* @return Reference to the current DepthStencilState.
		*/
		virtual const RDepthStencilState& GetDepthStencilState ( ) const noexcept = 0;

		/*====================================================================
		* @brief Returns the current rasterizer state.
		*
		* Exposes configuration such as polygon fill mode, cull face,
		* depth bias, and slope-scaled bias.
		* Important for debugging wireframe rendering, shadow acne,
		* or rendering to offscreen targets.
		*
		* @return Reference to the current RasterizerState.
		*/
		virtual const RRasterizerState& GetRasterizerState ( ) const noexcept = 0;

		/*====================================================================
		* @brief Returns the current viewport state.
		*
		* Provides access to viewport dimensions and depth range.
		* Essential for rendering to multiple render targets, split-screen,
		* or handling window resize events.
		*
		* @return Reference to the current ViewportState.
		*/
		virtual const RViewportState& GetViewport ( ) const noexcept = 0;

		/*====================================================================
		* @brief Checks if a buffer handle is valid.
		*
		* @param handle Buffer handle to validate.
		* @return True if the handle refers to an existing buffer, false otherwise.
		*/
		virtual bool IsValid ( RBufferHandle handle ) const = 0;

		/*====================================================================
		* @brief Checks if a texture handle is valid.
		*
		* @param handle Texture handle to validate.
		* @return True if the handle refers to an existing texture, false otherwise.
		*/
		virtual bool IsValid ( RTextureHandle handle ) const = 0;

		/*====================================================================
		* @brief Checks if a shader handle is valid.
		*
		* @param handle Shader handle to validate.
		* @return True if the handle refers to an existing shader, false otherwise.
		*/
		virtual bool IsValid ( RShaderHandle handle ) const = 0;

		/*====================================================================
		* @brief Checks if a framebuffer handle is valid.
		*
		* @param handle Framebuffer handle to validate.
		* @return True if the handle refers to an existing framebuffer, false otherwise.
		*/
		virtual bool IsValid ( RFramebufferHandle handle ) const = 0;

		/*====================================================================
		* @brief Checks if a vertex layout handle is valid.
		*
		* @param handle Vertex layout handle to validate.
		* @return True if the handle refers to an existing vertex layout, false otherwise.
		*/
		virtual bool IsValid ( RVertexLayoutHandle handle ) const = 0;

		/*====================================================================
		* @brief Checks if a pipeline handle is valid.
		*
		* @param handle Pipeline handle to validate.
		* @return True if the handle refers to an existing pipeline, false otherwise.
		*/
		virtual bool IsValid ( RPipelineHandle handle ) const = 0;

		/*====================================================================
		* @brief Checks if a sampler handle is valid.
		*
		* @param handle Sampler handle to validate.
		* @return True if the handle refers to an existing sampler, false otherwise.
		*/
		virtual bool IsValid ( RSamplerHandle handle ) const = 0;

		/*====================================================================
		* @brief Sets the color write mask for individual RGBA channels.
		*
		* Controls which color components are written to the framebuffer.
		* Useful for masking specific channels during rendering operations
		* like stencil-only passes or specific post-processing effects.
		*
		* @param r Enable/disable red channel writes.
		* @param g Enable/disable green channel writes.
		* @param b Enable/disable blue channel writes.
		* @param a Enable/disable alpha channel writes.
		*/
		virtual void SetColorMask ( bool r, bool g, bool b, bool a ) = 0;

		/*====================================================================
		* @brief Sets the color write mask using a ColorMask structure.
		*
		* Convenience overload for setting all channel masks at once.
		*
		* @param rgba ColorMask structure containing all channel enable flags.
		*/
		virtual void SetColorMask ( RColorMask rgba ) = 0;

		/*====================================================================
		* @brief Sets the clear color for subsequent clear operations.
		*
		* Defines the RGBA color used when clearing the color buffer.
		* Does not perform the clear operation itself.
		*
		* @param color RGBA color value to use for clearing.
		*/
		virtual void SetClearColor ( ChannelRGBA color ) = 0;

		/*====================================================================
		* @brief Clears the color buffer using the current clear color.
		*
		* Fills the entire color buffer with the previously set clear color.
		*/
		virtual void ClearColor ( ) = 0;

		/*====================================================================
		* @brief Sets the clear color and immediately clears the color buffer.
		*
		* Convenience function combining SetClearColor and ClearColor.
		*
		* @param color RGBA color value to clear with.
		*/
		virtual void ClearColor ( ChannelRGBA color ) = 0;

		/*====================================================================
		* @brief Clears the depth buffer to its default value (typically 1.0).
		*
		* Resets depth information for subsequent depth-tested rendering.
		*/
		virtual void ClearDepth ( ) = 0;

		/*====================================================================
		* @brief Clears the stencil buffer to zero.
		*
		* Resets stencil values for subsequent stencil operations.
		*/
		virtual void ClearStencil ( ) = 0;

		/*====================================================================
		* @brief Clears specified buffers in a single operation.
		*
		* Efficiently clears multiple buffers (color, depth, stencil) simultaneously.
		* More efficient than calling individual clear functions separately.
		*
		* @param flags Combination of ClearFlag values (Color, Depth, Stencil).
		*/
		virtual void Clear(Flags<RClearFlag> flags) = 0;

		// TODO
		virtual void Draw(const RVertexLayoutHandle& vao, uint32 vertexCount) = 0;
		virtual void DrawInstanced(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount) = 0;
		virtual void DrawInstancedBase(const RVertexLayoutHandle& vao, uint32 vertexCount, uint32 instanceCount, uint32 baseInstance) = 0;
		virtual void DrawElements(const RVertexLayoutHandle&, uint32 indicesCount) = 0;
		virtual void DrawElementsInstanced(const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount) = 0;
		virtual void DrawElementsInstancedBase(const RVertexLayoutHandle& vao, uint32 indicesCount, uint32 instanceCount, uint32 baseInstance) = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;


#		if LUM_ENABLE_RENDER_PROFILER == 1
			inline void GetProfilerInfo() {
				std::cout << "Cache hit rate: " << mProfiler.GetCacheHitRate() << '\n';
			}
#		endif

	protected:
		
		LUM_COMPILE_VARIABLE
		static uint32 MAX_TEXTURE_UNITS = 32;

		LUM_COMPILE_VARIABLE
		static uint32 MAX_SAMPLER_UNITS = 32;

		std::array<RTextureHandle, MAX_TEXTURE_UNITS> mCurrentTextures{};
		std::array<RSamplerHandle, MAX_SAMPLER_UNITS> mCurrentSamplers{};

		RShaderHandle		mCurrentShader{};
		RFramebufferHandle	mCurrentFramebuffer{};
		RPipelineHandle		mCurrentPipeline{};

		// Cache of current states

		RBlendState			mBlendState{};
		RCullState			mCullState{};
		RScissorState		mScissorState{};
		RDepthStencilState	mDepthStencilState{};
		RRasterizerState		mRasterizerState{};
		RViewportState		mViewportState{};
		RColorMask			mColorMask{};

		// SOURCE OF TRUST - enabled states ( don't look at bEnabled at states )
		Flags<RState>		mEnabledStates{};

		ChannelRGBA			mClearColor{};
		
#		if LUM_ENABLE_RENDER_PROFILER == 1
			performance::Profiler mProfiler{};

#			define LUM_PROFILER_BEGIN_FRAME()	mProfiler.StartRecording()
#			define LUM_PROFILER_END_FRAME()		mProfiler.EndRecording()
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
		
		LUM_COMPILE_VARIABLE
		static uint32 skMaxShaders = 8;

		LUM_COMPILE_VARIABLE
		static uint32 skMaxSamplers = 500;

		LUM_COMPILE_VARIABLE
		static uint32 skMaxBuffers = 10000;

		LUM_COMPILE_VARIABLE
		static uint32 skMaxLayouts = 10000;

		LUM_COMPILE_VARIABLE
		static uint32 skMaxTextures = 1000;

		LUM_COMPILE_VARIABLE
		static uint32 skMaxFramebuffers = 100;

		LUM_COMPILE_VARIABLE
		static uint32 skMaxPipelines = 100;

		cstd::handle_pool<Sampler, RSamplerHandle>			mSamplers		{ skMaxSamplers };
		cstd::handle_pool<RShader, RShaderHandle>				mShaders		{ skMaxShaders };
		cstd::handle_pool<RBuffer, RBufferHandle>				mBuffers		{ skMaxBuffers };
		cstd::handle_pool<RVertexLayout, RVertexLayoutHandle> mLayouts		{ skMaxLayouts };
		cstd::handle_pool<RTexture, RTextureHandle>			mTextures		{ skMaxTextures };
		cstd::handle_pool<RFramebuffer, RFramebufferHandle>	mFramebuffers	{ skMaxFramebuffers };
		cstd::handle_pool<Pipeline, RPipelineHandle>			mPipelines		{ skMaxPipelines };



	};

	/*====================================================================
	* @brief Creates render device.
	*
	*  Initializes and creates render device including used backend (OpenGL for now).
	* 
	*  @param window Pointer to the window.
	*
	*  @return Pointer to the device.
	*
	*/
	RDevice* CreateDevice(Window* window);

}