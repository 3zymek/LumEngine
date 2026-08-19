//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: DeferredBuffer management for deferred rendering.
//          Handles creation, resizing and binding of geometry buffers.
//
//=============================================================================//
#pragma once

#include "Render/RenderCommon.hpp"

namespace lum::render::detail {

	enum class DeferredBufferAttachment : byte {
		Albedo,
		Normal,
		Depth
	};

	/* @brief G-Buffer used in deferred rendering.
	*  Manages albedo, normal and depth textures and the framebuffer
	*  they are attached to. Automatically recreates on window resize.
	*
	*  @brief Albedo  (RGBA)  — rgb: albedo color, a: roughness
	*  @brief Normal  (RGBA)  — rgb: world space normal, a: metallic
	*  @brief Depth   (D32F)  — depth buffer for position reconstruction
	*/
	class DeferredBuffer {
	public:

		/* @brief Initializes the DefferedBuffer with the given renderer context and dimensions.
		*  Creates all textures, framebuffer and subscribes to window resize events.
		*  @param ctx Renderer context providing access to RHI and event bus.
		*  @param w Initial width in pixels.
		*  @param h Initial height in pixels.
		*/
		void Initialize( RendererContext& ctx, uint32 w, uint32 h );

		/* @brief Binds all DefferedBuffer textures to their respective sampler slots.
		*  Call before the light pass draw call.
		*/
		void BindTextures( ) const;

		LUM_FORCEINLINE void BindFramebuffer( ) const { 
			mCtx( ).mRenderDev( ).BindFramebuffer( mFramebuffer );
		}

		LUM_FORCEINLINE void UnbindFramebuffer( ) const { 
			mCtx( ).mRenderDev( ).BindFramebuffer( rhi::kDefaultFramebuffer );
		}

		void Clear( ) {
			BindFramebuffer( );
			mCtx( ).mRenderDev( ).Clear( rhi::BufferBit::Color | rhi::BufferBit::Depth | rhi::BufferBit::Stencil );
		}

		rhi::TextureHandle GetAttachment( DeferredBufferAttachment tex );

	private:

		SafePtr<RendererContext> mCtx = nullptr;

		rhi::FramebufferHandle mFramebuffer{};	/* @brief Framebuffer with albedo and normal attachments. */
		rhi::TextureHandle mAlbedoTex{};
		rhi::TextureHandle mNormalTex{};
		rhi::TextureHandle mDepthTex{};

		void create_textures( uint32 width, uint32 height );
		void create_framebuffer( );
		void subscribe_event( );

	};

} // namespace lum::render::detail
