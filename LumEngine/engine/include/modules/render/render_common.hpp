//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Shared render data types — lights, instances, camera
//          and internal pass/uniform structures.
//
//=============================================================================//
#pragma once
#include "rhi/core/rhi_device.hpp"
#include "core/utils/handle_pool.hpp"
#include "entity/components/transform.hpp"
#include "entity/components/material.hpp"

namespace lum {

	namespace ev { class EventBus; }

	/// Forward declare ///
	class MTextureManager;
	class MMaterialManager;
	class MShaderManager;
	class MMeshManager;
	class MSceneManager;
	struct CStaticMesh;
	///////////////////////

	struct StaticMeshHandle : cstd::BaseHandle<uint32> { };
	struct DynamicMeshHandle : cstd::BaseHandle<uint32> { };

}

namespace lum::render {

#	define LUM_UBO_ALIGNMENT alignas(16)

	/* @brief Aggregates all external subsystem pointers required to initialize the renderer. */
	struct RendererContext {

		rhi::RenderDevice*	mRenderDev = nullptr;
		MTextureManager*	mTextureMgr = nullptr;
		MMaterialManager*	mMaterialMgr = nullptr;
		MMeshManager*		mMeshMgr = nullptr;
		MShaderManager*		mShaderMgr = nullptr;
		ev::EventBus*		mEvBus = nullptr;
	};

	struct PostprocessExecutables {
		

	};

	constexpr void ValidateRendererContext( const RendererContext& ctx ) {

		LUM_ASSERT( ctx.mRenderDev != nullptr, "RenderDevice is null" );
		LUM_ASSERT( ctx.mTextureMgr != nullptr, "TextureManager is null" );
		LUM_ASSERT( ctx.mMaterialMgr != nullptr, "MaterialManager is null" );
		LUM_ASSERT( ctx.mMeshMgr != nullptr, "MeshManager is null" );
		LUM_ASSERT( ctx.mShaderMgr != nullptr, "ShaderManager is null" );
		LUM_ASSERT( ctx.mEvBus != nullptr, "EventBus is null" );

	}

	/* @brief Directional light source — infinite distance, uniform direction.
	*  Aligned to 16 bytes for std140 uniform buffer compatibility.
	*/
	struct DirectionalLight {

		/* @brief Normalized direction vector the light is pointing towards. */
		Vector3 mDirection = Vector3( 0.0f, 1.0f, 0.0f );

		/* @brief Intensity multiplier applied to the light color. */
		float32 mIntensity = 10.f;
		
		/* @brief Linear RGB color of the light. */
		Vector3 mColor = { 1.0f, 1.0f, 1.0f };

		/* Matrix used to calculate directrional light shadows. */
		Matrix4 mLightSpaceMatrix = Matrix4( 1.0f );

	};


	/* @brief Point light source — emits light in all directions from a single position. */
	struct PointLight {

		Vector3 mPosition = Vector3( 0.0f );
		float32 mIntensity = 10.f;
		Vector3 mColor = Vector3( 1.0f );
		float32 mRadius = 10.f;

	};

	struct SpotLight {

		Vector3 mPosition = Vector3( 0.0f );
		float32 mIntensity = 10.0f;

		Vector3 mDirection = Vector3( 0.0f, 1.0f, 0.0f );
		float32 mRadius = 35.0f;;

		Vector3 mColor = Vector3( 1.0f );
		float32 mInnerCone = 10.0f;

		float32 mOuterCone = 50.0f;

		float32 mPad[ 3 ]{};

	};


	/* @brief Represents a single renderable object submitted to the renderer per frame. */
	struct RenderInstance {

		CTransform* mTransform = nullptr;

		CStaticMesh* mStaticMesh = nullptr;

		CMaterialInstance* mMaterial = nullptr;

	};


	/* @brief Per-frame camera data provided by the caller to the renderer. */
	struct RenderCamera {

		/* @brief World-space position of the camera. */
		Vector3 mPosition = Vector3( 0.0f );

		/* @brief View matrix transforming world space to camera space. */
		Matrix4 mView = Matrix4( 1.0f );

		/* @brief Projection matrix transforming camera space to clip space. */
		Matrix4 mProjection = Matrix4( 1.0f );

	};

	namespace detail {

		struct ScreenQuad {

			rhi::VertexLayoutHandle mVao;

			rhi::BufferHandle mVbo;
			rhi::BufferHandle mEbo;

			rhi::FramebufferHandle mSceneFbo;
			rhi::FramebufferHandle mPostprocessFbo;

			rhi::TextureHandle mSceneTex;
			rhi::TextureHandle mPostprocessTex;

		};

		/* @brief Internal state for the environment (skybox) render pass. */
		struct Cubemap {

			/* @brief Shader program used for skybox rendering. */
			rhi::ShaderHandle mShader;

			/* @brief Pipeline state for the environment pass. */
			rhi::PipelineHandle mPipeline;

			/* @brief Cubemap or equirectangular environment texture. */
			rhi::TextureHandle mTexture;

			/* @brief Vertex buffer holding skybox geometry. */
			rhi::BufferHandle mVbo;

			/* @brief Index buffer for skybox geometry. */
			rhi::BufferHandle mEbo;

			/* @brief Vertex layout describing skybox vertex attributes. */
			rhi::VertexLayoutHandle mVao;

			/* @brief Number of indices in the skybox index buffer. */
			uint32 mNumIndices = 0;

		};

		struct LUM_UBO_ALIGNMENT LightSpaceMatricesGPU {

			Matrix4 mDirectionalLightSpaceMatrix = Matrix4( 1.0f );

		};


		/* @brief GPU-side camera uniform buffer layout. Aligned to std140. */
		struct LUM_UBO_ALIGNMENT CameraGPU {
			Matrix4 mView = Matrix4( 1.0f );
			Matrix4 mProjection = Matrix4( 1.0f );
			Matrix4 mInvViewProj = Matrix4( 1.0f );
			Vector4 mPosition = Vector4( 1.0f );
		};


		/* @brief GPU-side model uniform buffer layout. Aligned to std140. */
		struct LUM_UBO_ALIGNMENT ModelGPU {
			Matrix4 mModel = Matrix4( 1.0f );
		};


		/* @brief GPU-side material uniform buffer layout. Aligned to std140. */
		struct LUM_UBO_ALIGNMENT MaterialGPU {
			Vector4 mBaseColor = Vector4( 1.0f );
			float32 mRoughness = 0.5f;
			float32 mMetallic = 0.0f;

			float32 mPad[ 2 ]{};
			
		};

		struct LUM_UBO_ALIGNMENT DirectionalLightGPU {

			Vector4 mDirection = Vector4( 0.0f, -1.0f, 0.0f, 0.0f );
			Vector4 mColor = Vector4( 1.00, 0.97, 0.90, 0.0f );
			float32 mIntensity = 5.0f;

			float32 mPad[ 3 ]{};

		};


	} // namespace lum::render::detail

} // namespace lum::render