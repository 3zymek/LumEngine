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

#define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)

namespace lum {

	// Forward declarations
	class MTextureManager;
	class MMeshManager;
	class MShaderManager;
	class EditorCamera;
	class StaticMeshHandle;
	//=======================

}

namespace lum::render {

	struct DirectionalLight {

		glm::vec3 mDirection = glm::vec3(0.f);
		float32 mIntensity = 0.f;
		glm::vec3 mColor = { 1, 1, 1 };
		float32 __padding = 0;

	};

	struct Object {

		/* @brief World position. */
		glm::vec3 mPosition = { 0.f, 0.f, 0.f };

		/* @brief Non-uniform scale. */
		glm::vec3 mScale = { 1.f, 1.f, 1.f };

		/* @brief Euler rotation in degrees. */
		glm::vec3 mRotation = { 0.f, 0.f, 0.f };

		StaticMeshHandle mStaticMesh;

		FMaterialInstance mMaterial;

	};

	struct FRendererContext {
		
		rhi::RDevice*		mRenderDevice		= nullptr;
		MTextureManager*	mTextureManager		= nullptr;
		MMaterialManager*	mMaterialManager	= nullptr;
		MMeshManager*		mMeshManager		= nullptr;
		MShaderManager*		mShaderManager		= nullptr;

	};

	struct FCameraData {

		glm::vec3 mPosition{ 1 };
		glm::mat4 mView{ 1 };
		glm::mat4 mProjection{ 1 };

		/* @brief Yaw angle in degrees, represents horizontal rotation. */
		float32 mYaw = -90.0f;

		/* @brief Pitch angle in degrees, represents vertical rotation. */
		float32 mPitch = 0.0f;

	};

	namespace detail {

		struct FEnvironmentPass {

			rhi::RShaderHandle			mShader;
			rhi::RPipelineHandle		mPipeline;
			rhi::RTextureHandle			mTexture;
			rhi::RBufferHandle			mVbo;
			rhi::RBufferHandle			mEbo;
			rhi::RVertexLayoutHandle	mVao;

			uint32 mNumIndices = 0;

		};

		struct FShadowPass {

			rhi::RShaderHandle		mShader;
			rhi::RPipelineHandle	mPipeline;
			rhi::RTextureHandle		mDepthTexture;
			rhi::RFramebufferHandle mDepthBuffer;
			glm::mat4 mLightSpaceMatrix = glm::mat4(1.f);

		};

		struct FRendererUniforms {

			rhi::RBufferHandle mCameraUniform;
			rhi::RBufferHandle mModelUniform;
			rhi::RBufferHandle mMaterialUniform;
			rhi::RBufferHandle mLightShaderStorage;

		};

		struct FGeometryPass {

			rhi::RPipelineHandle	mPipeline;
			rhi::RShaderHandle		mShader;

		};

		LUM_UNIFORM_BUFFER_STRUCT CameraUniformBuffer {
			glm::mat4 mView;
			glm::mat4 mProjection;
			glm::vec3 mPosition;
		};

		LUM_UNIFORM_BUFFER_STRUCT ModelUniformBuffer {
			glm::mat4 mModel;
		};

		LUM_UNIFORM_BUFFER_STRUCT MaterialUniformBuffer {
			glm::vec3 mBaseColor = glm::vec3(1.0f);
			float32 mRougness = 0.5f;
			float32 mMetallic = 0.0f;
		};

	} // namespace lum::render::detail

	// TODO fix from experimental to release
	class Renderer {
	public:

		Renderer( ) = default;

		void Initialize(const FRendererContext& ctx) {
		
			mRenderDevice = ctx.mRenderDevice;
			mTextureManager = ctx.mTextureManager;
			mMaterialManager = ctx.mMaterialManager;
			mMeshManager = ctx.mMeshManager;
			mShaderManager = ctx.mShaderManager;

			init();
		
		}

		void SetEnvionmentTexture( rhi::RTextureHandle tex );

		void UpdateCamera( FCameraData& camera );

		void Draw( const Object& obj );
		void BeginFrame( );
		void EndFrame( );

		DirectionalLight mDirectionalLight;

	private:

		// Context

		MTextureManager*	mTextureManager = nullptr;
		MMaterialManager*	mMaterialManager = nullptr;
		MMeshManager*		mMeshManager = nullptr;
		MShaderManager*		mShaderManager = nullptr;
		rhi::RDevice*		mRenderDevice = nullptr;

		// Passes

		detail::FRendererUniforms	mUniforms;
		detail::FEnvironmentPass	mEnvironmentPass;
		detail::FShadowPass			mShadowPass;
		detail::FGeometryPass		mGeometryPass;

		detail::CameraUniformBuffer mCurrentCamera;

		detail::MaterialUniformBuffer mCurrentMaterial;

		void init() {
			init_buffers();
			init_geometry_pass();
			init_environment_pass();
			init_shadow_pass();
		}

		void upload_model_matrix( const Object& obj );
		void upload_material( const FMaterialInstance& mat );
		void upload_lights( );
		void upload_camera_uniform( );

		void init_buffers( );
		void init_geometry_pass( );
		void init_environment_pass( );
		void init_shadow_pass( );

	};

} // namespace lum::render

#undef LUM_UNIFORM_BUFFER_STRUCT