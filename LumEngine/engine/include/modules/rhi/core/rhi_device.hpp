// LumEngine Copyright (C) 2026 3zymek
// All rights reserved.
// Core of Render Hardware Interface
#pragma once

#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "core/shaders_define.h"
#include "core/flags.hpp"
#include "core/setup.hpp"

#include "rhi/core/rhi_buffer.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "rhi/core/rhi_shader.hpp"
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_texture.hpp"
#include "rhi/core/rhi_sampler.hpp"
#include "rhi/core/rhi_framebuffer.hpp"
#include "rhi/core/rhi_pipeline.hpp"
#if LUM_ENABLE_RENDER_PROFILER == 1
#	include "rhi/rhi_profiler.hpp"
#endif

namespace lum { class Window; }
namespace lum::rhi {

	/*!
	*  @brief Low-level GPU interface for resource creation and rendering.
	*
	*  @brief RHI_Device provides functions to create, bind, and manage GPU resources
	*  such as textures, samplers, shaders, and buffers. It also handles
	*  binding resources to the pipeline for drawing or compute operations.
	*
	*  @brief All operations are GPU-aware and should be used according to the
	*  current command buffer / pipeline state.
	*
	*  @brief Typical usage:
	*  @brief  - Create resources (textures, shaders, buffers)
	*  @brief  - Bind resources (shaders, textures, samplers)
	*  @brief  - Update uniforms or descriptors
	*  @brief  - Issue draw/dispatch commands
	*/
	class RenderDevice {
	public:

		///////////////////////////////////////////////////
		/// Buffers
		///////////////////////////////////////////////////

		/*! @brief Creates Vertex buffer ( VBO ).
		* 
		*  @param desc BufferDescriptor of VBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle CreateVertexBuffer( const BufferDescriptor& desc ) = 0;
		
		/*! @brief Creates Element buffer ( EBO ).
		*
		*  @param desc BufferDescriptor of EBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle CreateElementBuffer( const BufferDescriptor& desc ) = 0;
		
		/*! @brief Creates Uniform buffer ( UBO ).
		*
		*  @param desc BufferDescriptor of UBO ( containing size, flags, usage and data ).
		*  @return Handle to the buffer.
		* 
		*/
		LUM_NODISCARD 
		virtual BufferHandle CreateUniformBuffer( const BufferDescriptor& desc ) = 0;

		/*! @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer ( 0 for whole buffer ).
		*  @param size Byte size of data to write ( 0 for whole buffer ).
		*
		*/
		virtual void UpdateBuffer( const BufferHandle& buff, cvptr data, usize offset = 0, usize size = 0 ) = 0;
		
		/*! @brief Deletes buffer.
		*
		*  @param buff Buffer handle to delete.
		*
		*/
		virtual void DeleteBuffer( BufferHandle& buff ) = 0;
		
		/*! @brief Maps data of given buffer.
		*
		*  @param buff Buffer handle to map.
		*  @param flags Mapping flags ( example: rhi::map_flags::Write ).
		*  @param offset Byte offset from the beginning of the buffer to map ( 0 for whole buffer ).
		*  @param size Byte size of data to map ( 0 for whole buffer ).
		* 
		*  @return GPU data pointer to manage.
		*	
		*/
		LUM_NODISCARD 
		virtual vptr MapBuffer( const BufferHandle& buff, Mapflag flags, usize offset = 0, usize size = 0 ) = 0;
		
		/*! @brief Unmaps buffer.
		*
		*  @param buff Buffer handle to unmap.
		*
		*/
		virtual void UnmapBuffer( const BufferHandle& buff ) = 0;
		
		/*! @brief Connects element buffer ( EBO ) to vertex layout ( VAO ).
		*
		*  @param ebo Element buffer to connect.
		*  @param vao Vertex layout to connect.
		* 
		*/
		virtual void AttachElementBufferToLayout( const BufferHandle& ebo, const VertexLayoutHandle& vao ) = 0;
		
		/*! @brief Connects uniform buffer ( UBO ) to binding in shaders.
		*
		*  @param ubo Uniform buffer handle
		*  @param binding Binding in the shader ( example: LUM_UBO_CAMERA )
		*
		*/
		virtual void SetUniformBufferBinding( const BufferHandle& ubo, int32 binding ) = 0;





		///////////////////////////////////////////////////
		/// Framebuffers
		///////////////////////////////////////////////////

		/*! @brief Updates data of given buffer.
		*
		*  @param buff Buffer handle to update.
		*  @param data Pointer to source data in CPU memeory.
		*  @param offset Byte offset from the beginning of the buffer ( 0 for whole buffer ).
		*  @param size Byte size of data to write ( 0 for whole buffer ).
		*
		*/
		LUM_NODISCARD
		virtual FramebufferHandle CreateFramebuffer() = 0;

		/*! @brief Creates a texture for use in a framebuffer.
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
		virtual TextureHandle CreateFramebufferTexture(const FramebufferTextureDescriptor& desc) = 0;

		// TO FIX:
		virtual void SetFramebufferColorTexture(const FramebufferHandle& fbo, const TextureHandle& tex, uint8 index) = 0;
		// TO IMPLEMENT:
		virtual void SetFramebufferDepthTexture(const FramebufferHandle& fbo, const TextureHandle& tex) = 0;
		virtual void SetFramebufferStencilTexture(const FramebufferHandle& fbo, const TextureHandle& tex) = 0;

		/*! @brief Clears the specified framebuffer with the given color and depth values.
		*
		* This function sets the clear color and depth, then clears the color and depth
		* buffers of the framebuffer. The framebuffer is automatically bound during this operation.
		*
		* @param fbo Handle to the framebuffer to clear.
		* @param color RGBA color used to clear the color buffer.
		* @param depth Depth value used to clear the depth buffer (clamped between 0.0 and 1.0).
		*/
		virtual void ClearFramebuffer(FramebufferHandle fbo, ChannelRGBA color, float32 depth) = 0;

		/*! @brief Deletes the specified framebuffer and releases its resources.
		*	
		* After calling this function, the framebuffer handle becomes invalid and should
		* not be used in subsequent rendering operations.
		* 
		* @param buff Handle to the framebuffer to delete.
		*
		*/
		virtual void DeleteFramebuffer(FramebufferHandle& fbo) = 0;

		/*! @brief Sets the current framebuffer for rendering.
		*
		* This makes the given framebuffer the target for all subsequent draw operations.
		* 
		* @param buff Handle to the framebuffer to bind.
		*
		*/
		virtual void BindFramebuffer(const FramebufferHandle& fbo) = 0;

		/*! @brief Unbinds any currently bound framebuffer.
		*
		*  After calling this, rendering will target the default framebuffer
		*  (typically the screen) instead of any previously bound framebuffer.
		*/
		virtual void UnbindFramebuffer() = 0;





		///////////////////////////////////////////////////
		/// Layouts
		///////////////////////////////////////////////////

		/*! @brief Creates Vertex layout (VAO).
		*
		*  @param desc Descriptor.
		*  @param vbo Vertex buffer to connect.
		* 
		*  @return Handle to vertex layout.
		*
		*/
		LUM_NODISCARD
		virtual VertexLayoutHandle CreateVertexLayout( const VertexLayoutDescriptor& desc, const BufferHandle& vbo ) = 0;
		
		/*! @brief Deletes vertex layout.
		*
		*  @param layout Layout handle to delete.
		*
		*/
		virtual void DeleteVertexLayout( VertexLayoutHandle& layout ) = 0;





		///////////////////////////////////////////////////
		/// Shaders
		///////////////////////////////////////////////////

		/*! @brief Creates a shader from a descriptor.
		*
		*  Uses vertex/fragment sources from the descriptor.
		*  @param desc ShaderDescriptor with source file names.
		*  @return Handle to the created shader.
		* 
		*/
		LUM_NODISCARD
		virtual ShaderHandle CreateShader( const ShaderDescriptor& desc)	= 0;
		
		/*! @brief Binds a shader for rendering.
		*
		*  The bound shader will be used on next draw/dispatch.
		*  @param shader Shader handle to bind.
		* 
		*/
		virtual void BindShader( const ShaderHandle& shader ) = 0;
		
		/*! @brief Deletes a shader.
		*
		*  Frees GPU resources and invalidates the handle.
		*  @param shader Shader handle to delete.
		* 
		*/
		virtual void DeleteShader( ShaderHandle& shader ) = 0;
		
		/*! @brief Sets a 4x4 matrix uniform in a shader.
		* 
		*  @param shader Shader handle.
		*  @param location Name of the uniform.
		*  @param mat Matrix to set.
		* 
		*/
		virtual void SetMat4( const ShaderHandle& shader, ccharptr location, const math::Mat4& mat ) = 0;
		
		/*!
		* @brief Sets a float uniform in a shader.
		*/
		virtual void Setf( const ShaderHandle& shader, ccharptr location, float32 value )	= 0;
		
		/*!
		* @brief Sets an int uniform in a shader.
		*/
		virtual void Seti( const ShaderHandle& shader, ccharptr location, int32 value )	= 0;
		
		/*!
		* @brief Sets a vec4 uniform in a shader.
		*/
		virtual void SetVec4( const ShaderHandle& shader, ccharptr location, const math::Vec4& vec ) = 0;
		
		/*!
		* @brief Sets a vec3 uniform in a shader.
		*/
		virtual void SetVec3( const ShaderHandle& shader, ccharptr location, const math::Vec3& vec ) = 0;
		
		/*!
		* @brief Sets a vec2 uniform in a shader.
		*/
		virtual void SetVec2( const ShaderHandle& shader, ccharptr location, const math::Vec2& vec ) = 0;




		///////////////////////////////////////////////////
		/// Textures
		///////////////////////////////////////////////////

		/*! @brief Creates a 2D texture.
		*
		*  Uses the descriptor to initialize GPU texture.
		*  @param desc Texture properties and data.
		*  @return Handle to the created texture.
		* 
		*/
		LUM_NODISCARD 
		virtual TextureHandle CreateTexture2D( const TextureDescriptor& desc ) = 0;

		/*! @brief Creates a 3D texture.
		*
		*  Uses the descriptor to initialize GPU 3D texture.
		*  @param desc Texture properties and data.
		*  @return Handle to the created texture.
		* 
		*/
		LUM_NODISCARD 
		virtual TextureHandle CreateTexture3D( const TextureDescriptor& desc ) = 0;
		
		LUM_NODISCARD
		virtual TextureHandle CreateCubemapTexture(const TextureCubemapDescriptor& desc) = 0;

		/*! @brief Deletes a texture.
		*
		*  Frees GPU memory and invalidates the handle.
		*  @param texture Handle of the texture to delete.
		* 
		*/
		virtual void DeleteTexture( TextureHandle& texture ) = 0;
		
		/*! @brief Binds a texture to the active slot/unit.
		*
		*  The bound texture will be used by shaders on the next draw/dispatch.
		*  @param texture Texture to bind.
		* 
		*/
		virtual void SetTextureBinding( const TextureHandle& texture, uint16 binding ) = 0;

		/*! @brief Binds a texture to a given binding slot.
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
		virtual void BindTexture( const TextureHandle& texture, uint16 binding = LUM_NULL_BINDING ) = 0;





		///////////////////////////////////////////////////
		/// Samplers
		///////////////////////////////////////////////////
		
		/*! @brief Creates Sampler.
		*
		*  @param desc SamplerDescriptor of sampler.
		*
		*/
		LUM_NODISCARD
		virtual SamplerHandle CreateSampler( const SamplerDescriptor& desc ) = 0;
		
		/*!
		*  @brief Binds a sampler to a GPU slot.
		*
		*  Keeps the binding until changed. Used by shaders on next draw/dispatch.
		*
		*  @param sampler  Sampler handle to bind.
		*  @param binding  GPU binding slot index.
		*/
		virtual void SetSamplerBinding( const SamplerHandle& sampler, uint16 binding ) = 0;
		virtual void BindSampler( const SamplerHandle& sampler, uint16 binding = LUM_NULL_BINDING )	= 0;
		virtual void DeleteSampler( SamplerHandle sampler ) = 0;




		///////////////////////////////////////////////////
		/// Pipelines
		///////////////////////////////////////////////////

		virtual PipelineHandle CreatePipeline(const PipelineDescriptor& desc) = 0;
		virtual void DeletePipeline(PipelineHandle& pipeline) = 0;
		virtual void BindPipeline(const PipelineHandle& pipeline) = 0;


		///////////////////////////////////////////////////
		/// Other
		///////////////////////////////////////////////////

		/*!
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
		virtual void SetViewport(int32 x, int32 y, int32 width, int32 height) = 0;

		/*!
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
		virtual void SetViewportX(int32 x) = 0;

		/*!
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
		virtual void SetViewportY(int32 y) = 0;

		/*!
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
		virtual void SetViewportWidth(int32 width) = 0;

		/*!
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
		virtual void SetViewportHeight(int32 height) = 0;


		/*!
		* @brief Enables or disables the scissor test.
		*
		* When enabled, fragments outside the defined scissor rectangle
		* are discarded during rasterization.
		* State is persistent until changed.
		*
		* Commonly used for UI, split-screen, and render region clipping.
		*
		* @param enable True to enable the scissor test, false to disable it.
		*/
		virtual void ToggleScissors(bool) = 0;

		/*!
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
		virtual bool IsScissorEnabled() const noexcept = 0;

		/*!
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
		virtual void SetScissors(int32 x, int32 y, int32 width, int32 height) = 0;
		
		/*!
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
		virtual void SetScissorX(int32 x) = 0;
		
		/*!
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
		virtual void SetScissorY(int32 y) = 0;
		
		/*!
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
		virtual void SetScissorWidth(int32 width) = 0;

		/*!
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
		virtual void SetScissorHeight(int32 height) = 0;



		// Cull setters

		/*!
		* @brief Enables or disables face culling.
		*
		* Controls whether the rasterizer discards back-facing (or front-facing,
		* depending on current cull mode) primitives.
		* State is persistent until changed.
		*
		* Used to reduce overdraw and improve rendering performance.
		*
		* @param enable True to enable face culling, false to disable it.
		*/
		virtual void ToggleCull(bool) = 0;

		/*!
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
		virtual bool IsCullEnabled() const noexcept = 0;

		/*!
		* @brief Selects which polygon faces are culled.
		*
		* Controls the rasterizer culling mode used to discard primitives
		* based on their winding order.
		*
		* Requires face culling to be enabled.
		*
		* @param face Cull face selection (Front or Back).
		*/
		virtual void SetCullFace(Face face) = 0;

		/*!
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
		virtual void SetFrontFace(WindingOrder) = 0;




		// Blend setters

		/*!
		* @brief Enables or disables color blending.
		*
		* Controls whether fragment shader outputs are blended with the
		* current contents of the render target.
		*
		* Affects all subsequent draw calls until the state is changed.
		*
		* Commonly used for transparency, particles, and UI rendering.
		*
		* @param enabled True to enable blending, false to disable it.
		*/
		virtual void ToggleBlend(bool enabled) = 0;

		/*!
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
		virtual bool IsBlendEnabled() const noexcept = 0;

		virtual void SetBlendConstantColor(ChannelRGBA rgba) = 0;
		
		/*!
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
		virtual void SetBlendFactors(BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha) = 0;

		/*!
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
		virtual void SetBlendColorFactors(BlendFactor srcColor, BlendFactor dstColor) = 0;

		/*!
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
		virtual void SetBlendAlphaFactors(BlendFactor srcAlpha, BlendFactor dstAlpha) = 0;

		/*!
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
		virtual void SetBlendSrcColorFactor(BlendFactor factor) = 0;

		/*!
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
		virtual void SetBlendDstColorFactor(BlendFactor factor) = 0;

		/*!
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
		virtual void SetBlendSrcAlphaFactor(BlendFactor factor) = 0;

		/*!
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
		virtual void SetBlendDstAlphaFactor(BlendFactor factor) = 0;

		/*!
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
		virtual void SetBlendOp(BlendOp colorOp, BlendOp alphaOp) = 0;

		/*!
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
		virtual void SetBlendColorOp(BlendOp op) = 0;

		/*!
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
		virtual void SetBlendAlphaOp(BlendOp op) = 0;

		virtual void SetBlendFactorsForTarget(uint8 target) = 0;
		virtual void ToggleBlendForTarget(uint8 target, bool enable) = 0;



		// Depth setters

		/*!
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
		* @param enable True to enable depth writes, false to disable them.
		*/
		virtual void ToggleDepthWrite(bool) = 0;
		
		/*!
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
		virtual bool IsDepthWriteEnabled() const noexcept = 0;

		/*!
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
		virtual void ToggleDepthTest(bool enable) = 0;

		/*!
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
		virtual bool IsDepthTestEnabled() const noexcept = 0;

		/*!
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
		virtual void SetDepthFunc(CompareFlag func) = 0;




		// Stencil setters

		/*!
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
		* @param enable True to enable stencil testing, false to disable it.
		*/
		virtual void ToggleStencilTest(bool enable) = 0;

		/*!
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
		virtual bool IsStencilTestEnabled() const noexcept = 0;

		/*!
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
		*/ // TODO ------------------------------------------------------------------------
		virtual void SetStencilReference(int32 ref, Face face = Face::FrontBack) = 0;
		virtual void SetStencilOp(StencilOp sfail, StencilOp dpfail, StencilOp dppass, Face face = Face::FrontBack) = 0;
		virtual void SetStencilOpOnStencilFail(StencilOp op, Face face = Face::FrontBack) = 0;
		virtual void SetStencilOpOnDepthFail(StencilOp op, Face face = Face::FrontBack) = 0;
		virtual void SetStencilOpOnDepthPass(StencilOp op, Face face = Face::FrontBack) = 0;



		// Rasterizer setters

		/*!
		* @brief Enables or disables depth bias during rasterization.
		*
		* When enabled, applies the currently configured depth bias
		* to subsequent draw calls. State is persistent until changed.
		*
		* Used to prevent depth fighting (e.g. shadow mapping, decals).
		*
		* @param enable True to enable depth bias, false to disable it.
		*/
		virtual void ToggleDepthBias(bool) = 0;
		virtual bool IsDepthBiasEnabled() const noexcept = 0;

		virtual void SetDepthBiasFactors(float32 slope, float32 constant) = 0;
		virtual void SetDepthBiasClamp(float32 clamp) = 0;

		/*!
		*  @brief Sets the scale and units used to calculate depth values.
		*
		*  Keeps the binding until changed. Used by shaders on next draw/dispatch.
		*
		*  @param slopFactor Specifies a scale factor that is used to create a variable depth offset for each polygon.
		*  @param constantBias Is multiplied by an implementation-specific value to create a constant depth offset.
		*/
		virtual void SetDepthBiasSlope(float32 slopeFactor) = 0;
		
		/*!
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
		virtual void SetDepthBiasConstant(float32 constantBias) = 0;

		/*!
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
		virtual void SetTopology(TopologyMode mode, Face face = Face::FrontBack) = 0;




		/*!
		* @brief Returns the current blend state.
		*
		* Provides read-only access to the blend configuration, including
		* blend enable, source/destination factors, and blend operations.
		* Useful for checking if transparent objects, particle systems, or
		* post-processing effects are configured correctly.
		*
		* @return Reference to the current BlendState.
		*/
		virtual const BlendState& GetBlendState() const noexcept = 0;

		/*!
		* @brief Returns the current face culling state.
		*
		* Allows querying which faces are culled (front/back) and the winding order.
		* Useful for validating render state before drawing opaque geometry or
		* performing custom cull logic.
		*
		* @return Reference to the current CullState.
		*/
		virtual const CullState& GetCullState() const noexcept = 0;

		/*!
		* @brief Returns the current scissor state.
		*
		* Provides access to scissor rectangle configuration.
		* Fragments outside the scissor rect are discarded.
		* Commonly used for UI clipping, minimap rendering, or split-screen.
		*
		* @return Reference to the current ScissorState.
		*/
		virtual const ScissorState& GetScissorState() const noexcept = 0;

		/*!
		* @brief Returns the current depth and stencil state.
		*
		* Provides access to depth test, depth write, stencil enable,
		* and stencil operations.
		* Useful for debugging rendering issues involving occlusion,
		* shadow volumes, or stencil masking.
		*
		* @return Reference to the current DepthStencilState.
		*/
		virtual const DepthStencilState& GetDepthStencilState() const noexcept = 0;

		/*!
		* @brief Returns the current rasterizer state.
		*
		* Exposes configuration such as polygon fill mode, cull face,
		* depth bias, and slope-scaled bias.
		* Important for debugging wireframe rendering, shadow acne,
		* or rendering to offscreen targets.
		*
		* @return Reference to the current RasterizerState.
		*/
		virtual const RasterizerState& GetRasterizerState() const noexcept = 0;

		/*!
		* @brief Returns the current viewport state.
		*
		* Provides access to viewport dimensions and depth range.
		* Essential for rendering to multiple render targets, split-screen,
		* or handling window resize events.
		*
		* @return Reference to the current ViewportState.
		*/
		virtual const ViewportState& GetViewport() const noexcept = 0;

		virtual bool IsValid(BufferHandle handle) const = 0;
		virtual bool IsValid(TextureHandle handle) const = 0;
		virtual bool IsValid(ShaderHandle handle) const = 0;
		virtual bool IsValid(FramebufferHandle handle) const = 0;
		virtual bool IsValid(VertexLayoutHandle handle) const = 0;
		virtual bool IsValid(PipelineHandle handle) const = 0;
		virtual bool IsValid(SamplerHandle handle) const = 0;

		virtual void SetColorMask(bool r, bool g, bool b, bool a) = 0;
		virtual void SetColorMask(ColorMask rgba) = 0;

		virtual void ClearColor(ChannelRGBA color) = 0;
		virtual void ClearDepth() = 0;
		virtual void ClearStencil() = 0;
		virtual void Clear(uint32 flags) = 0;

		virtual void Draw(const VertexLayoutHandle& vao, uint32 vertex_count) = 0;
		virtual void DrawElements(const VertexLayoutHandle&, uint32 indices_count) = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;


#if LUM_ENABLE_RENDER_PROFILER == 1
		inline void GetProfilerInfo() {
			std::cout << mProfiler.GetCacheHitRate() << '\n';
		}
#endif

	protected:

		// Handle for currently used pipeline
		PipelineHandle mCurrentPipeline{};

		// Cache of current states

		BlendState			mBlendState{};
		CullState			mCullState{};
		ScissorState		mScissorState{};
		DepthStencilState	mDepthStencilState{};
		RasterizerState		mRasterizerState{};
		ViewportState		mViewportState{};
		ColorMask			mColorMask{};

		// SOURCE OF TRUST - enabled states ( don't look at bEnabled at states )
		Flags<State>		mEnabledStates{};

#		if LUM_ENABLE_RENDER_PROFILER == 1
			performance::Profiler mProfiler{};

#			define LUM_PROFILER_BEGIN_FRAME() mProfiler.BeginFrame()
#			define LUM_PROFILER_END_FRAME() mProfiler.EndFrame()
#			define LUM_PROFILER_DRAW_CALL() mProfiler.RegisterDrawCall()
#			define LUM_PROFILER_CACHE_MISS() mProfiler.RegisterCacheMiss()
#			define LUM_PROFILER_CACHE_HIT() mProfiler.RegisterCacheHit()

#		else

#			define LUM_PROFILER_BEGIN_FRAME() ((void)0)
#			define LUM_PROFILER_END_FRAME() ((void)0)
#			define LUM_PROFILER_DRAW_CALL() ((void)0)
#			define LUM_PROFILER_CACHE_MISS() ((void)0)
#			define LUM_PROFILER_CACHE_HIT() ((void)0)

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

		cstd::handle_pool<Sampler, SamplerHandle>			mSamplers		{ skMaxSamplers };
		cstd::handle_pool<Shader, ShaderHandle>				mShaders		{ skMaxShaders };
		cstd::handle_pool<Buffer, BufferHandle>				mBuffers		{ skMaxBuffers };
		cstd::handle_pool<VertexLayout, VertexLayoutHandle> mLayouts		{ skMaxLayouts };
		cstd::handle_pool<Texture, TextureHandle>			mTextures		{ skMaxTextures };
		cstd::handle_pool<Framebuffer, FramebufferHandle>	mFramebuffers	{ skMaxFramebuffers };
		cstd::handle_pool<Pipeline, PipelineHandle>			mPipelines		{ skMaxPipelines };



	};

	/*! @brief Creates render device.
	*
	*  Initializes and creates render device including used backend (OpenGL for now).
	* 
	*  @param window Pointer to the window.
	*
	*  @return Pointer to the device.
	*
	*/
	RenderDevice* CreateDevice(Window* window);

}