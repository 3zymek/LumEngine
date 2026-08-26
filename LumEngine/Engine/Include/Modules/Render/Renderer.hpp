//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Bridge between engine data and the RHI backend.
//
//=============================================================================//
#pragma once

#include "Render/RenderCommon.hpp"
#include "Render/Passes/GeometryPass.hpp"
#include "Render/Passes/LightPass.hpp"
#include "Render/Passes/EnvironmentPass.hpp"
#include "Render/ShadowSystem.hpp"
#include "Render/Passes/PostprocessPass.hpp"
#include "Render/Postprocess/Antialiasing/Taa.hpp"
#include "Render/DeferredBuffer.hpp"

namespace lum::render {

	/* @brief High-level renderer — accepts per-frame draw calls, lights and camera data,
	*  and drives the underlying RHI passes to produce a final image.
	*/
	class LUM_API Renderer {
	public:

		Renderer( ) = default;

		/* @brief Initializes the renderer and allocates all GPU resources.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const RendererContext& ctx, uint32 w, uint32 h );

		rhi::TextureHandle GetFrameTexture( ) { return m_ScreenQuad.m_PostprocessTex; }


		//---------------------------------------------------------
		// Environment
		//---------------------------------------------------------

		/* @brief Sets the environment cubemap texture used for skybox rendering and IBL precomputation.
		*  @param tex Handle to a valid HDR cubemap texture.
		*/
		void SetEnvironmentTexture( rhi::TextureHandle tex ) { m_EnvironmentPass.SetCubemapTexture( tex ); }



		//---------------------------------------------------------
		// Camera
		//---------------------------------------------------------

		/* @brief Updates the active camera used for rendering this frame.
		*  @param camera Camera data containing view, projection and position.
		*/
		void UpdateCamera( const RenderCamera& camera );



		//---------------------------------------------------------
		// Lights
		//---------------------------------------------------------

		/* @brief Submits a point light to be included in the current frame's lighting.
		*  @param light Point light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddPointLight( const PointLight& light ) { m_LightPass.AddPointLight( light ); }

		/* @brief Submits a spot light to be included in the current frame's lighting.
		*  @param light Spot light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddSpotLight( const SpotLight& light ) { m_LightPass.AddSpotLight( light ); }

		/* @brief Sets the active directional light for the current frame.
		*  @param light Directional light to set.
		*/
		void SetDirectionalLight( const DirectionalLight& light ) { m_LightPass.SetDirectionalLight( light ); }

		/* @brief Returns the currently active directional light. */
		DirectionalLight GetDirectionalLight( ) { return m_LightPass.GetDirectionalLight( ); }



		//---------------------------------------------------------
		// Draw submission
		//---------------------------------------------------------

		/* @brief Submits a render instance for drawing in the current frame.
		*  @param instance Render instance containing transform, mesh and material.
		*/
		void SubmitInstance( const RenderInstance& instance ) { m_GeometryPass.Submit( instance ); }



		//---------------------------------------------------------
		// Frame
		//---------------------------------------------------------

		/* @brief Begins a new frame — clears per-frame light and draw state. */
		void BeginFrame( );

		/* @brief Ends the current frame — flushes all submitted draw calls through all passes. */
		void EndFrame( );


	private:

		/* @brief Cached context holding all subsystem manager references. */
		RendererContext m_Ctx;
		detail::DeferredBuffer m_DefferedBuffer;
		detail::ScreenQuad m_ScreenQuad;



		//---------------------------------------------------------
		// Passes
		//---------------------------------------------------------

		GeometryPass m_GeometryPass;
		LightPass m_LightPass;
		LightPassExectueContext m_LightPassExecuteCtx; // Descriptor passed to the light pass each frame
		EnvironmentPass m_EnvironmentPass;
		PostprocessPass m_PostprocessPass;
		ShadowSystem m_ShadowSys;

		//---------------------------------------------------------
		// Uniforms
		//---------------------------------------------------------

		/* @brief Uniform buffer holding per-frame camera matrices and position. */
		rhi::BufferHandle m_CameraUbo;

		/* @brief CPU-side camera data uploaded to m_CameraUBO once per frame. */
		detail::CameraGPU m_CameraGpu{};


		TemporalAntiAliasing m_TemporalAa{ };


		//---------------------------------------------------------
		// Internal
		//---------------------------------------------------------

		/* @brief Allocates GPU buffers and initializes all render passes. */
		void init( uint32 w, uint32 h );

		/* @brief Allocates the screen quad FBO used for deferred and post-process passes. */
		void ensure_screenquad_fbo( );

		/* @brief Allocates the screen quad color texture at the given resolution.
		*  @param w Width in pixels.
		*  @param h Height in pixels.
		*/
		void ensure_screenquad_texture( uint32 w, uint32 h );

		/* @brief Uploads the current camera data to the GPU uniform buffer. */
		void upload_camera_uniform( );

	};

} // namespace lum::render