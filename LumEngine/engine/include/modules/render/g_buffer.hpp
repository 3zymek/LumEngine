//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: G-Buffer management for deferred rendering.
//          Handles creation, resizing and binding of geometry buffers.
//
//=============================================================================//
#pragma once

#include "render/common.hpp"
#include "event/events/window_events.hpp"

namespace lum::render::detail {

	enum class GBufferTexture : byte {
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
	class GBuffer {
	public:

		/* @brief Initializes the G-Buffer with the given renderer context and dimensions.
		*  Creates all textures, framebuffer and subscribes to window resize events.
		*  @param ctx Renderer context providing access to RHI and event bus.
		*  @param w Initial width in pixels.
		*  @param h Initial height in pixels.
		*/
		void Initialize( const FRendererContext& ctx, uint32 w, uint32 h );

		/* @brief Binds all G-Buffer textures to their respective sampler slots.
		*  Call before the light pass draw call.
		*/
		void BindTextures( ) const;

		/* @brief Binds the G-Buffer framebuffer as the active render target.
		*  Call before the geometry pass draw calls.
		*/
		LUM_FORCEINLINE 
		void BindBuffer( ) const { mContext.mRenderDevice->BindFramebuffer(mFramebuffer); }

		/* @brief Unbinds the G-Buffer framebuffer, restoring the default render target. */
		LUM_FORCEINLINE
		void UnbindBuffer( ) const { mContext.mRenderDevice->BindFramebuffer(rhi::gDefaultFramebuffer); }

		rhi::RTextureHandle GetTexture( GBufferTexture tex );

	private:

		FRendererContext mContext;

		rhi::RFramebufferHandle mFramebuffer; /* @brief Framebuffer with albedo and normal attachments. */
		rhi::RTextureHandle mAlbedo;          /* @brief Albedo + roughness texture (SRGB8_Alpha8). */
		rhi::RTextureHandle mNormal;          /* @brief World space normal + metallic texture (RGBA16F). */
		rhi::RTextureHandle mDepth;           /* @brief Depth texture for position reconstruction (Depth32F). */

		/* @brief Creates or recreates all G-Buffer textures at the given dimensions. */
		void create_textures( uint32 width, uint32 height );

		/* @brief Creates or recreates the framebuffer with current texture handles. */
		void create_framebuffer( );

		/* @brief Subscribes to window resize events to recreate textures and framebuffer. */
		void init( );

	};

} // namespace lum::render::detail