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

namespace lum {

	/// Forward declare ///
	class MTextureManager;
	class MMeshManager;
	class MShaderManager;
	class EditorCamera;
	class StaticMeshHandle;
	///////////////////////
}

namespace lum::render {

	/* @brief Aggregates all external subsystem pointers required to initialize the renderer. */
	struct FRendererContext {

		/* @brief Pointer to the active RHI device. */
		rhi::RDevice* mRenderDevice = nullptr;

		/* @brief Pointer to the active texture manager. */
		MTextureManager* mTextureMgr = nullptr;

		/* @brief Pointer to the active material manager. */
		MMaterialManager* mMaterialMgr = nullptr;

		/* @brief Pointer to the active mesh manager. */
		MMeshManager* mMeshMgr = nullptr;

		/* @brief Pointer to the active shader manager. */
		MShaderManager* mShaderMgr = nullptr;

	};

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
		void SetEnvironmentTexture( rhi::RTextureHandle tex );

		/* @brief Updates the active camera used for rendering this frame.
		*  @param camera Camera data containing view, projection and position.
		*/
		void UpdateCamera( const FRenderCamera& camera );

		/* @brief Submits a point light to be included in the current frame's lighting.
		*  @param light Point light to add. Ignored if LUM_MAX_LIGHTS is reached.
		*/
		void AddPointLight( const FPointLight& light );

		void SetDirectionalLight(const FDirectionalLight& light);
		FDirectionalLight GetDirectionalLight();

		/* @brief Submits a render instance for drawing in the current frame.
		*  @param obj Render instance containing transform, mesh and material.
		*/
		void Draw( const FRenderInstance& obj );

		/* @brief Begins a new frame — clears per-frame state and prepares passes. */
		void BeginFrame( );

		/* @brief Ends the current frame — flushes all submitted draw calls. */
		void EndFrame( );

	private:

		/* @brief Cached context holding all subsystem manager references. */
		FRendererContext mContext;

		/* @brief Per-frame light data submitted via AddPointLight. */
		detail::FRenderLights mLights;



		//---------------------------------------------------------
		// Passes
		//---------------------------------------------------------

		/* @brief GPU buffer handles for all uniform and storage buffers. */
		detail::FRendererUniforms mUniforms;

		/* @brief Internal state for the environment (skybox) pass. */
		detail::FEnvironmentPass mEnvironmentPass;

		/* @brief Internal state for the shadow map pass. */
		detail::FShadowPass	mShadowPass;

		/* @brief Internal state for the main geometry pass. */
		detail::FGeometryPass mGeometryPass;


		//---------------------------------------------------------
		// Uniforms
		//---------------------------------------------------------

		/* @brief CPU-side camera uniform buffer, uploaded once per frame. */
		detail::FCameraUniformBuffer mCurrentCamera {};

		/* @brief CPU-side material uniform buffer, uploaded per draw call. */
		detail::FMaterialUniformBuffer mMaterialUBO {};

		detail::FDirectionalLightUniformBuffer mDirectionalLightUBO {};


		/* @brief Allocates GPU buffers and initializes all render passes. */
		void init( );

		/* @brief Resets active light counts — called at the start of each frame. */
		void clear_point_lights( );

		void update_shadow_pass();

		/* @brief Computes and uploads the model matrix for the given instance. */
		void upload_model_matrix( const FRenderInstance& obj );

		void upload_lightspace_matrix(const glm::mat4& mat);

		/* @brief Uploads material parameters for the given material instance. */
		void upload_material( const FMaterialInstance& mat );

		/* @brief Uploads all active point lights to the shader storage buffer. */
		void upload_point_lights( );

		void upload_directional_light();

		/* @brief Uploads the current camera uniform buffer to the GPU. */
		void upload_camera_uniform( );

		/* @brief Allocates and initializes all uniform and storage GPU buffers. */
		void prep_buffers( );

		/* @brief Initializes pipeline and shader for the geometry pass. */
		void prep_geometry_pass( );

		/* @brief Initializes pipeline, shader and geometry for the environment pass. */
		void prep_environment_pass( );

		/* @brief Initializes pipeline, shader and framebuffer for the shadow pass. */
		void prep_shadow_pass( );


	};

} // namespace lum::render