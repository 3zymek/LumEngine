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
	class Renderer {
	public:

		Renderer( ) = default;

		/* @brief Initializes the renderer and allocates all GPU resources.
		*  @param ctx Context struct containing valid pointers to all subsystem managers.
		*/
		void Initialize( const RendererContext& ctx );

		rhi::TextureHandle GetFrameTexture( ) { return mScreenQuad.mPostprocessTex; }


		//---------------------------------------------------------
		// Environment
		//---------------------------------------------------------

		/* @brief Sets the environment cubemap texture used for skybox rendering and IBL precomputation.
		*  @param tex Handle to a valid HDR cubemap texture.
		*/
		void SetEnvironmentTexture( rhi::TextureHandle tex ) { mEnvironmentPass.SetCubemapTexture( tex ); }



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
		void AddPointLight( const PointLight& light ) { mLightPass.AddPointLight( light ); }

		/* @brief Submits a spot light to be included in the current frame's lighting.
		*  @param light Spot light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddSpotLight( const SpotLight& light ) { mLightPass.AddSpotLight( light ); }

		/* @brief Sets the active directional light for the current frame.
		*  @param light Directional light to set.
		*/
		void SetDirectionalLight( const DirectionalLight& light ) { mLightPass.SetDirectionalLight( light ); }

		/* @brief Returns the currently active directional light. */
		DirectionalLight GetDirectionalLight( ) { return mLightPass.GetDirectionalLight( ); }



		//---------------------------------------------------------
		// Draw submission
		//---------------------------------------------------------

		/* @brief Submits a render instance for drawing in the current frame.
		*  @param instance Render instance containing transform, mesh and material.
		*/
		void Submit( const RenderInstance& instance ) { mGeometryPass.Submit( instance ); }



		//---------------------------------------------------------
		// Frame
		//---------------------------------------------------------

		/* @brief Begins a new frame — clears per-frame light and draw state. */
		void BeginFrame( );

		/* @brief Ends the current frame — flushes all submitted draw calls through all passes. */
		void EndFrame( );


	private:

		/* @brief Cached context holding all subsystem manager references. */
		RendererContext mCtx;

		/* @brief GBuffer holding geometry pass output textures — albedo, normal and depth. */
		detail::DeferredBuffer mDefferedBuffer;

		/* @brief Fullscreen quad used for deferred lighting and post-process passes. */
		detail::ScreenQuad mScreenQuad;



		//---------------------------------------------------------
		// Passes
		//---------------------------------------------------------

		/* @brief Geometry pass — renders all submitted instances into the G-Buffer. */
		GeometryPass mGeometryPass;

		/* @brief Light pass — evaluates PBR lighting using G-Buffer and IBL data. */
		LightPass mLightPass;

		/* @brief Descriptor passed to the light pass each frame with IBL and shadow map handles. */
		LightPassExectueContext mLightPassExecuteCtx;

		/* @brief Environment pass — renders the HDR skybox and precomputes IBL maps. */
		EnvironmentPass mEnvironmentPass;

		PostprocessPass mPostprocessPass;


		ShadowSystem mShadowSys;

		//---------------------------------------------------------
		// Uniforms
		//---------------------------------------------------------

		/* @brief Uniform buffer holding per-frame camera matrices and position. */
		rhi::BufferHandle mCameraUbo;

		/* @brief CPU-side camera data uploaded to mCameraUBO once per frame. */
		detail::CameraGPU mCameraGpu{};


		TemporalAntiAliasing mTemporalAA{ };


		//---------------------------------------------------------
		// Internal
		//---------------------------------------------------------

		/* @brief Allocates GPU buffers and initializes all render passes. */
		void init( );

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