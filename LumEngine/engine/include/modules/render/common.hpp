//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Shared render data types — lights, instances, camera
//          and internal pass/uniform structures.
//
//=============================================================================//
#pragma once
#include "core/core_common.hpp"

namespace lum::render {

#	define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)


	/* @brief Directional light source — infinite distance, uniform direction.
	*  Aligned to 16 bytes for std140 uniform buffer compatibility.
	*/
	LUM_UNIFORM_BUFFER_STRUCT FDirectionalLight {

		/* @brief Normalized direction vector the light is pointing towards. */
		glm::vec3 mDirection = glm::vec3(0.f);

		/* @brief Intensity multiplier applied to the light color. */
		float32 mIntensity = 0.f;

		/* @brief Linear RGB color of the light. */
		glm::vec3 mColor = { 1.0f, 1.0f, 1.0f };

	};


	/* @brief Point light source — emits light in all directions from a single position. */
	struct FPointLight {

		/* @brief World-space position of the light. */
		glm::vec3 mPosition;

		/* @brief Intensity multiplier applied to the light color. */
		float32 mIntensity;

		/* @brief Linear RGB color of the light. */
		glm::vec3 mColor;

		/* @brief Maximum influence radius of the light in world units. */
		float32 mRadius;

	};


	/* @brief Represents a single renderable object submitted to the renderer per frame. */
	struct FRenderInstance {

		/* @brief World position. */
		glm::vec3 mPosition = { 0.f, 0.f, 0.f };

		/* @brief Non-uniform scale. */
		glm::vec3 mScale = { 1.f, 1.f, 1.f };

		/* @brief Euler rotation in degrees. */
		glm::vec3 mRotation = { 0.f, 0.f, 0.f };

		/* @brief Handle to the static mesh asset to render. */
		StaticMeshHandle mStaticMesh;

		/* @brief Material instance defining surface appearance. */
		FMaterialInstance mMaterial;

	};


	/* @brief Per-frame camera data provided by the caller to the renderer. */
	struct FRenderCamera {

		/* @brief World-space position of the camera. */
		glm::vec3 mPosition{ 1 };

		/* @brief View matrix transforming world space to camera space. */
		glm::mat4 mView{ 1 };

		/* @brief Projection matrix transforming camera space to clip space. */
		glm::mat4 mProjection{ 1 };

	};

	namespace detail {

		/* @brief Internal state for the environment (skybox) render pass. */
		struct FEnvironmentPass {

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


		/* @brief Internal state for the shadow map render pass. */
		struct FShadowPass {

			/* @brief Shader program used for depth-only shadow rendering. */
			rhi::RShaderHandle mShader;

			/* @brief Pipeline state for the shadow pass. */
			rhi::RPipelineHandle mPipeline;

			/* @brief Depth texture storing the shadow map. */
			rhi::RTextureHandle	mDepthTexture;

			/* @brief Framebuffer targeting the depth texture. */
			rhi::RFramebufferHandle mDepthBuffer;

			/* @brief Combined view-projection matrix from the light's perspective. */
			glm::mat4 mLightSpaceMatrix = glm::mat4(1.f);

		};

		/* @brief Internal state for the main geometry render pass. */
		struct FGeometryPass {

			/* @brief Pipeline state for geometry rendering. */
			rhi::RPipelineHandle mPipeline;

			/* @brief Shader program used for geometry rendering. */
			rhi::RShaderHandle mShader;

		};

		/* @brief Handles to all GPU uniform and storage buffers used by the renderer. */
		struct FRendererUniforms {

			/* @brief Uniform buffer holding per-frame camera matrices and position. */
			rhi::RBufferHandle mCameraUniform;

			/* @brief Uniform buffer holding per-draw model matrix. */
			rhi::RBufferHandle mModelUniform;

			/* @brief Uniform buffer holding per-draw material parameters. */
			rhi::RBufferHandle mMaterialUniform;

			/* @brief Shader storage buffer holding all active point lights. */
			rhi::RBufferHandle mPointLights;

			rhi::RBufferHandle mDirectionalLight;

		};


		/* @brief GPU-side camera uniform buffer layout. Aligned to std140. */
		LUM_UNIFORM_BUFFER_STRUCT FCameraUniformBuffer {
			glm::mat4 mView = glm::mat4(1.0f);
			glm::mat4 mProjection = glm::mat4(1.0f);
			glm::vec4 mPosition = glm::vec4(1.0f);
		};


		/* @brief GPU-side model uniform buffer layout. Aligned to std140. */
		LUM_UNIFORM_BUFFER_STRUCT FModelUniformBuffer {
			glm::mat4 mModel = glm::mat4(1.0f);
		};


		/* @brief GPU-side material uniform buffer layout. Aligned to std140. */
		LUM_UNIFORM_BUFFER_STRUCT FMaterialUniformBuffer {
			glm::vec4 mBaseColor = glm::vec4(1.0f);
			float32 mRoughness = 0.5f;
			float32 mMetallic = 0.0f;

			float32 _pad0 = 0.f;
			float32 _pad1 = 0.f;
		};

		LUM_UNIFORM_BUFFER_STRUCT FDirectionalLightUniformBuffer {
			glm::vec4 mDirection = glm::vec4(1.0f);
			glm::vec4 mColor = glm::vec4(1.0f);
			float32 mIntensity = 1.0f;

			float32 _pad0 = 0.f;
			float32 _pad1 = 0.f;
			float32 _pad2 = 0.f;
		};


		/* @brief Aggregates all active lights for a single frame. */
		struct FRenderLights {

			/* @brief Active directional light for this frame. */
			FDirectionalLight mDirectional;

			/* @brief Array of active point lights for this frame. */
			std::array<FPointLight, LUM_MAX_LIGHTS> mPoints{};

			/* @brief Number of currently active point lights. */
			uint32 mActivePoints = 0;

		};

	} // namespace lum::render::detail

} // namespace lum::render

#undef LUM_UNIFORM_BUFFER_STRUCT