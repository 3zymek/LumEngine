//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Shared render data types — lights, instances, camera
//          and internal pass/uniform structures.
//
//=============================================================================//
#pragma once

#include "rhi/rhi_common.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/material.hpp"

namespace lum {

	namespace ev { class EventBus; }

	/// Forward declare ///
	class MTextureManager;
	class MMaterialManager;
	class MShaderManager;
	class MMeshManager;
	///////////////////////
}

namespace lum::render {

#	define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)

	/* @brief Aggregates all external subsystem pointers required to initialize the renderer. */
	struct FRendererContext {

		/* @brief Pointer to the active RHI device. */
		rhi::RenderDevice* mRenderDevice = nullptr;

		/* @brief Pointer to the active texture manager. */
		MTextureManager* mTextureMgr = nullptr;

		/* @brief Pointer to the active material manager. */
		MMaterialManager* mMaterialMgr = nullptr;

		/* @brief Pointer to the active mesh manager. */
		MMeshManager* mMeshMgr = nullptr;

		/* @brief Pointer to the active shader manager. */
		MShaderManager* mShaderMgr = nullptr;

		/* @brief Pointer to the active event bus. */
		ev::EventBus* mEventBus = nullptr;

	};

	constexpr void ValidateRendererContext( const FRendererContext& ctx ) {

		LUM_ASSERT( ctx.mRenderDevice != nullptr, "RenderDevice is null" );
		LUM_ASSERT( ctx.mTextureMgr != nullptr, "TextureManager is null" );
		LUM_ASSERT( ctx.mMaterialMgr != nullptr, "MaterialManager is null" );
		LUM_ASSERT( ctx.mMeshMgr != nullptr, "MeshManager is null" );
		LUM_ASSERT( ctx.mShaderMgr != nullptr, "ShaderManager is null" );
		LUM_ASSERT( ctx.mEventBus != nullptr, "EventBus is null" );

	}

	/* @brief Directional light source — infinite distance, uniform direction.
	*  Aligned to 16 bytes for std140 uniform buffer compatibility.
	*/
	struct FDirectionalLight {

		/* @brief Normalized direction vector the light is pointing towards. */
		glm::vec3 mDirection = glm::vec3( 0.0f, 1.0f, 0.0f );

		/* @brief Intensity multiplier applied to the light color. */
		float32 mIntensity = 10.f;
		
		/* @brief Linear RGB color of the light. */
		glm::vec3 mColor = { 1.0f, 1.0f, 1.0f };

		/* Matrix used to calculate directrional light shadows. */
		glm::mat4 mLightSpaceMatrix = glm::mat4( 1.0f );

	};


	/* @brief Point light source — emits light in all directions from a single position. */
	struct FPointLight {

		glm::vec3 mPosition = glm::vec3( 0.0f );
		float32 mIntensity = 10.f;
		glm::vec3 mColor = glm::vec3( 1.0f );
		float32 mRadius = 10.f;

	};

	struct FSpotLight {

		glm::vec3 mPosition = glm::vec3( 0.0f );
		float32 mIntensity = 10.0f;

		glm::vec3 mDirection = glm::vec3( 0.0f, 1.0f, 0.0f );
		float32 mRadius = 35.0f;;

		glm::vec3 mColor = glm::vec3( 1.0f );
		float32 mInnerCone = 10.0f;

		float32 mOuterCone = 50.0f;

		float32 _pad[ 3 ];

	};


	/* @brief Represents a single renderable object submitted to the renderer per frame. */
	struct FRenderInstance {

		CTransform* mTransform = nullptr;

		CStaticMesh* mStaticMesh = nullptr;

		CMaterial* mMaterial = nullptr;

	};


	/* @brief Per-frame camera data provided by the caller to the renderer. */
	struct FRenderCamera {

		/* @brief World-space position of the camera. */
		glm::vec3 mPosition = glm::vec3( 0.0f );

		/* @brief View matrix transforming world space to camera space. */
		glm::mat4 mView = glm::mat4( 1.0f );

		/* @brief Projection matrix transforming camera space to clip space. */
		glm::mat4 mProjection = glm::mat4( 1.0f );

	};

	namespace detail {

		struct FScreenQuad {

			rhi::RVertexLayoutHandle mVao;
			rhi::RBufferHandle mVbo;
			rhi::RBufferHandle mEbo;
			rhi::RFramebufferHandle mSceneFbo;
			rhi::RFramebufferHandle mPostprocessFbo;
			rhi::RTextureHandle mSceneTexture;
			rhi::RTextureHandle mPostprocessTexture;

		};

		/* @brief Internal state for the environment (skybox) render pass. */
		struct FCubemap {

			/* @brief Shader program used for skybox rendering. */
			rhi::RShaderHandle mShader;

			/* @brief Pipeline state for the environment pass. */
			rhi::RPipelineHandle mPipeline;

			/* @brief Cubemap or equirectangular environment texture. */
			rhi::RTextureHandle	mTexture;

			/* @brief Vertex buffer holding skybox geometry. */
			rhi::RBufferHandle mVbo;

			/* @brief Index buffer for skybox geometry. */
			rhi::RBufferHandle mEbo;

			/* @brief Vertex layout describing skybox vertex attributes. */
			rhi::RVertexLayoutHandle mVao;

			/* @brief Number of indices in the skybox index buffer. */
			uint32 mNumIndices = 0;

		};

		LUM_UNIFORM_BUFFER_STRUCT FLightSpaceMatricesUBOData {

			glm::mat4 mDirectionalLightSpaceMatrix = glm::mat4( 1.0f );

		};


		/* @brief GPU-side camera uniform buffer layout. Aligned to std140. */
		LUM_UNIFORM_BUFFER_STRUCT FCameraUBOData {
			glm::mat4 mView = glm::mat4( 1.0f );
			glm::mat4 mProjection = glm::mat4( 1.0f );
			glm::mat4 mInvViewProj = glm::mat4( 1.0f );
			glm::vec4 mPosition = glm::vec4( 1.0f );
		};


		/* @brief GPU-side model uniform buffer layout. Aligned to std140. */
		LUM_UNIFORM_BUFFER_STRUCT FModelUBOData {
			glm::mat4 mModel = glm::mat4( 1.0f );
		};


		/* @brief GPU-side material uniform buffer layout. Aligned to std140. */
		LUM_UNIFORM_BUFFER_STRUCT FMaterialUBOData {
			glm::vec4 mBaseColor = glm::vec4( 1.0f );
			float32 mRoughness = 0.5f;
			float32 mMetallic = 0.0f;

			float32 _pad0 = 0.f;
			float32 _pad1 = 0.f;
		};

		LUM_UNIFORM_BUFFER_STRUCT FDirectionalLightUBOData {

			glm::vec4 mDirection = glm::vec4( 0.0f, -1.0f, 0.0f, 0.0f );
			glm::vec4 mColor = glm::vec4( 1.00, 0.97, 0.90, 0.0f );
			float32 mIntensity = 5.0f;

			float32 _pad0 = 0.f;
			float32 _pad1 = 0.f;
			float32 _pad2 = 0.f;

		};


	} // namespace lum::render::detail

} // namespace lum::render