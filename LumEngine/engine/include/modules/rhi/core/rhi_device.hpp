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



		virtual void EnableScissors(bool) = 0;
		virtual void SetScissor(int32 x, int32 y, int32 width, int32 height) = 0;
		virtual void SetViewport(int32 x, int32 y, int32 width, int32 height) = 0;



		/*
		* 
		*  TO DO TOMMOROW
		*		- Stencil functions
		*		- Optymalize functions below
		*		- Maybe try to create skybox ( but finish this first )
		* 
		*/


		// Cull setters

		virtual void EnableCull(bool) = 0;
		virtual void SetCullFace(Face face) = 0;
		virtual void SetCullWindingOrder(WindingOrder) = 0;


		// Blend setters

		virtual void EnableBlend(bool enabled) = 0;
		//virtual void SetBlendConstantColor(glm::vec4 rgba) = 0; IMPLEMENT
		virtual void SetBlendFactors(BlendFactor srcColor, BlendFactor dstColor, BlendFactor srcAlpha, BlendFactor dstAlpha) = 0;
		virtual void SetBlendOp(BlendOp colorOp, BlendOp alphaOp) = 0;


		// Depth setters

		virtual void EnableDepthWrite(bool) = 0;
		virtual void EnableDepthTest(bool) = 0;
		virtual void SetDepthFunc(CompareFlag) = 0;


		// Stencil setters

		virtual void EnableStencilTest(bool) = 0;
		virtual void SetStencilReference(int32 ref, Face = Face::FrontBack) = 0;


		// Rasterizer setters

		virtual void EnableDepthBias(bool) = 0;
		virtual void SetDepthBias(float32 slopFactor = max_val<float32>(), float32 constantBias = max_val<float32>()) = 0;


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
		
		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxShaders = 8;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxSamplers = 500;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxBuffers = 10000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxLayouts = 10000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxTextures = 1000;

		LUM_CONST_VAR_QUALIFIER
		static uint32 skMaxFramebuffers = 100;

		LUM_CONST_VAR_QUALIFIER
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
	RenderDevice* create_device(Window* window);

}