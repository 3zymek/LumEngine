//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Bridge between engine data and the RHI backend.
//
//=============================================================================//
#pragma once

#include "rhi/core/rhi_device.hpp"

#include "core/core_common.hpp"

#include "entity/components/transform.hpp"

#include "render/material_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/common.hpp"
#include "render/passes/geometry_pass.hpp"
#include "render/passes/light_pass.hpp"
#include "render/passes/environment_pass.hpp"
#include "render/passes/shadow_pass.hpp"
#include "render/g_buffer.hpp"

namespace lum::render {

	/* @brief High-level renderer — accepts per-frame draw calls, lights and camera data,
	*  and drives the underlying RHI passes to produce a final image.
	*/
	class Renderer {
	public:

		Renderer( ) = default;

		/* @brief Initializes the renderer and allocates all GPU resources.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const FRendererContext& ctx );

		/* @brief Sets the environment cubemap texture used for skybox rendering.
		*  @param tex Handle to the environment texture.
		*/
		void SetEnvironmentTexture( rhi::RTextureHandle tex ) { mEnvironmentPass.SetCubemapTexture(tex); }

		/* @brief Updates the active camera used for rendering this frame.
		*  @param camera Camera data containing view, projection and position.
		*/
		void UpdateCamera( const FRenderCamera& camera );

		/* @brief Submits a point light to be included in the current frame's lighting.
		*  @param light Point light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddPointLight( const FPointLight& light ) { mLightPass.AddPointLight(light); }

		/* @brief Submits a spot light to be included in the current frame's lighting.
		*  @param light Spot light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddSpotLight( const FSpotLight& light ) { mLightPass.AddSpotLight(light); }

		/* @brief Sets the active directional light for the current frame.
		*  @param light Directional light to set.
		*/
		void SetDirectionalLight( const FDirectionalLight& light ) { mLightPass.SetDirectionalLight(light); }

		/* @brief Returns the currently active directional light. */
		FDirectionalLight GetDirectionalLight( ) { return mLightPass.GetDirectionalLight(); }

		/* @brief Submits a render instance for drawing in the current frame.
		*  @param obj Render instance containing transform, mesh and material.
		*/
		void Submit( const FRenderInstance& instance ) { mGeometryPass.Submit(instance); }

		/* @brief Begins a new frame — clears per-frame state and prepares passes. */
		void BeginFrame( );

		/* @brief Ends the current frame — flushes all submitted draw calls. */
		void EndFrame( );

	private:

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief GBuffer holding geometry pass output textures — albedo, normal and depth. */
		detail::GBuffer mGBuffer;

		/* @brief Fullscreen quad used for deferred lighting and post-process passes. */
		detail::FScreenQuad mScreenQuad;

		//---------------------------------------------------------
		// Passes
		//---------------------------------------------------------

		/* @brief Geometry pass — renders all submitted instances into the G-Buffer. */
		GeometryPass mGeometryPass;

		/* @brief Light pass — evaluates lighting using G-Buffer data. */
		LightPass mLightPass;

		/* @brief Environment pass — renders the skybox cubemap. */
		EnvironmentPass mEnvironmentPass;

		/* @brief Shadow map generation pass for lights. */
		ShadowPass mShadowPass;

		rhi::RShaderHandle mPostprocessShader;


		//---------------------------------------------------------
		// Uniforms
		//---------------------------------------------------------

		/* @brief Uniform buffer holding per-frame camera matrices and position. */
		rhi::RBufferHandle mCameraBuffer;

		/* @brief CPU-side camera uniform buffer, uploaded once per frame. */
		detail::FCameraUniformBuffer mCameraStruct{};



		/* @brief Allocates GPU buffers and initializes all render passes. */
		void init( );

		void create_screenquad_fbo( );
		void create_screenquad_texture( uint32 w, uint32 h );

		/* @brief Uploads the current camera uniform buffer to the GPU. */
		void upload_camera_uniform( );

	};
} // namespace lum::render