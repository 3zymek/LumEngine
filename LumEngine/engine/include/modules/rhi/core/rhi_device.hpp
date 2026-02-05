#pragma once
#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/utils/handle_pool.hpp"
#include "rhi/core/rhi_buffer.hpp"
#include "rhi/core/rhi_vertex_layout.hpp"
#include "rhi/core/rhi_shader.hpp"
#include "rhi/rhi_common.hpp"
#include "rhi/core/rhi_texture.hpp"
#include "rhi/core/rhi_sampler.hpp"
#include "core/shaders_define.h"
#include "rhi/core/rhi_framebuffer.hpp"
#include "rhi/core/rhi_pipeline.hpp"
#include "core/flags.hpp"
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
		virtual void ClearFramebuffer(FramebufferHandle fbo, glm::vec4 color, float32 depth) = 0;

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
		virtual void SetMat4( const ShaderHandle& shader, ccharptr location, const glm::mat4& mat ) = 0;
		
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
		virtual void SetVec4( const ShaderHandle& shader, ccharptr location, const glm::vec4& vec ) = 0;
		
		/*!
		* @brief Sets a vec3 uniform in a shader.
		*/
		virtual void SetVec3( const ShaderHandle& shader, ccharptr location, const glm::vec3& vec ) = 0;
		
		/*!
		* @brief Sets a vec2 uniform in a shader.
		*/
		virtual void SetVec2( const ShaderHandle& shader, ccharptr location, const glm::vec2& vec ) = 0;




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

		virtual void Draw				( const VertexLayoutHandle& vao, uint32 vertex_count )		= 0;
		virtual void DrawElements		( const VertexLayoutHandle&, uint32 indices_count )			= 0;
		virtual void BeginFrame			( )															= 0;
		virtual void EndFrame			( )															= 0;

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
		virtual void SetViewportX(int32 x) = 0;
		virtual void SetViewportY(int32 y) = 0;
		virtual void SetViewportWidth(int32 width) = 0;
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
		virtual void SetScissorX(int32 x) = 0;
		virtual void SetScissorY(int32 y) = 0;
		virtual void SetScissorWidth(int32 width) = 0;
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
		virtual void SetCullWindingOrder(WindingOrder) = 0;




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
		virtual bool IsBlendEnabled() const noexcept = 0;

		//virtual void SetBlendConstantColor(glm::vec4 rgba) = 0; IMPLEMENT
		
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
		virtual void SetBlendSrcColorFactor(BlendFactor factor) = 0;
		virtual void SetBlendDstColorFactor(BlendFactor factor) = 0;
		virtual void SetBlendSrcAlphaFactor(BlendFactor factor) = 0;
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
		virtual void SetBlendColorOp(BlendOp op) = 0;
		virtual void SetBlendAlphaOp(BlendOp op) = 0;



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

		virtual void SetDepthBiasFactors(float32 slope, float32 constant);
		virtual void SetDepthBiasClamp(float32 clamp);

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

		virtual BlendState GetBlendState() const noexcept = 0;
		virtual CullState GetCullState() const noexcept = 0;
		virtual ScissorState GetScissorState() const noexcept = 0;
		virtual DepthStencilState GetDepthStencilState() const noexcept = 0;
		virtual RasterizerState GetRasterizerState() const noexcept = 0;

	protected:

		// Handle for currently used pipeline
		PipelineHandle mCurrentPipeline{};

		// Cache of current states

		BlendState			mBlendState{};
		CullState			mCullState{};
		ScissorState		mScissorState{};
		DepthStencilState	mDepthStencilState{};
		RasterizerState		mRasterizerState{};

		Flags<State>		mEnabledStates	{};
		
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