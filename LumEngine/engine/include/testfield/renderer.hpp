#pragma once

#include "core/core_common.hpp"
#include "testfield/texture_manager.hpp"
#include "testfield/material_manager.hpp"
#include "testfield/model_manager.hpp"
#include "testfield/shader_manager.hpp"
#include "entity/components/c_transform.hpp"
#include "camera.hpp"

#define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)

namespace lum {

	struct DirectionalLight {

		glm::vec3 mDirection = glm::vec3(0.f);
		float32 mIntensity = 0.f;
		glm::vec3 mColor = { 1, 1, 1 };
		float32 __padding = 0;

	};

	struct Object {

		CTransform mTransform;
		StaticMeshHandle mStaticMesh;
		FMaterialInstance mMaterial;

	};

	struct FRendererContext {
		
		rhi::RDevice*		mRenderDevice = nullptr;
		MTextureManager*	mTextureManager = nullptr;
		MMaterialManager*	mMaterialManager = nullptr;
		MMeshManager*		mMeshManager = nullptr;
		MShaderManager*		mShaderManager = nullptr;

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

	}

	class SRenderer {
	public:

		SRenderer(const FRendererContext& ctx) : 
			mRenderDevice(ctx.mRenderDevice), 
			mTextureManager(ctx.mTextureManager), 
			mMaterialManager(ctx.mMaterialManager), 
			mMeshManager(ctx.mMeshManager),
			mShaderManager(ctx.mShaderManager) 
		{
			init_buffers();
			init_geometry_pass();
			init_environment_pass();
			init_shadow_pass();
		}

		void SetEnvionmentTexture(rhi::RTextureHandle tex) {
			mEnvironmentPass.mTexture = tex;
		}

		void BeginFrame() {
			mRenderDevice->BeginFrame();
			upload_lights();
		}
		void EndFrame() {

			mRenderDevice->BindPipeline(mEnvironmentPass.mPipeline);

			mRenderDevice->BindShader(mEnvironmentPass.mShader);
			mRenderDevice->BindTexture(mEnvironmentPass.mTexture, LUM_TEX_CUBEMAP);
			mRenderDevice->DrawElements(mEnvironmentPass.mVao, mEnvironmentPass.mNumIndices);

			mRenderDevice->EndFrame();
		}

		void UpdateCamera(Camera& camera) {

			camera.Update();

			mCurrentCamera.mPosition = camera.position;
			mCurrentCamera.mProjection = camera.projection;
			mCurrentCamera.mView = camera.view;

			upload_camera_uniform();

		}

		void Draw(const Object& obj) {

			const FStaticMeshResource& res = mMeshManager->GetStatic(obj.mStaticMesh);
			const auto& mat = obj.mMaterial;

			upload_model_matrix(obj.mTransform);
			upload_material(obj.mMaterial);

			mRenderDevice->BindPipeline(mGeometryPass.mPipeline);
			mRenderDevice->BindShader(mGeometryPass.mShader);
			mRenderDevice->BindTexture(mat.mAlbedoMap, LUM_TEX_ALBEDO);
			mRenderDevice->BindTexture(mat.mNormalMap, LUM_TEX_NORMAL);
			mRenderDevice->BindTexture(mat.mMetallicMap, LUM_TEX_METALNESS);
			mRenderDevice->BindTexture(mat.mRoughnessMap, LUM_TEX_ROUGHNESS);

			mRenderDevice->DrawElements(res.mVao, res.mNumIndices);

		}

		DirectionalLight* mDirectionalLight = nullptr;

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

		void upload_model_matrix(const CTransform& obj) {

			glm::quat rot = glm::quat(glm::radians(obj.mRotation));
			glm::mat4 rotation = glm::mat4_cast(rot);
			glm::mat4 model = glm::mat4(1.f);
			model = glm::mat4(1.f);
			model = glm::translate(model, obj.mPosition);
			model = model * rotation;
			model = glm::scale(model, obj.mScale);

			mRenderDevice->UpdateBuffer(mUniforms.mModelUniform, glm::value_ptr(model), 0, 0);

		}
		void upload_material(const FMaterialInstance& mat) {

			mCurrentMaterial.mBaseColor = mat.mBaseColor;
			mCurrentMaterial.mRougness = mat.mRoughness;
			mCurrentMaterial.mMetallic = mat.mMetallic;

			mRenderDevice->UpdateBuffer(mUniforms.mMaterialUniform, &mCurrentMaterial);

		}
		void upload_lights() {
			if (!mDirectionalLight)
				return;

			mRenderDevice->UpdateBuffer(mUniforms.mLightShaderStorage, mDirectionalLight, 0, sizeof(DirectionalLight));

			glm::vec3 lightDir = glm::normalize(mDirectionalLight->mDirection);
			glm::vec3 lightPos = -lightDir * 20.0f;

			glm::mat4 view = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

			mShadowPass.mLightSpaceMatrix = proj * view;

			mRenderDevice->UpdateBuffer(mUniforms.mLightShaderStorage, glm::value_ptr(mShadowPass.mLightSpaceMatrix), sizeof(DirectionalLight), sizeof(glm::mat4));
		}
		void upload_camera_uniform() {

			mRenderDevice->UpdateBuffer(mUniforms.mCameraUniform, &mCurrentCamera, 0, 0);

		}

		void init_buffers() {
			rhi::RBufferDescriptor desc;
			desc.mBufferUsage = rhi::RBufferUsage::Dynamic;
			desc.mMapFlags = rhi::RMapFlag::Write;

			{ // Camera Uniform
				desc.mSize = sizeof(detail::CameraUniformBuffer);
				desc.mBufferType = rhi::RBufferType::Uniform;
				mUniforms.mCameraUniform = mRenderDevice->CreateBuffer(desc);
				mRenderDevice->SetUniformBufferBinding(mUniforms.mCameraUniform, LUM_UBO_CAMERA_BINDING);
			}
			{ // Model Uniform
				desc.mSize = sizeof(detail::ModelUniformBuffer);
				desc.mBufferType = rhi::RBufferType::Uniform;
				mUniforms.mModelUniform = mRenderDevice->CreateBuffer(desc);
				mRenderDevice->SetUniformBufferBinding(mUniforms.mModelUniform, LUM_UBO_MODEL_BINDING);
			}
			{ // Material Uniform
				desc.mSize = sizeof(detail::MaterialUniformBuffer);
				desc.mBufferType = rhi::RBufferType::Uniform;
				mUniforms.mMaterialUniform = mRenderDevice->CreateBuffer(desc);
				mRenderDevice->SetUniformBufferBinding(mUniforms.mMaterialUniform, LUM_UBO_MATERIAL_BINDING);
			}
			{ // Light SSBO
				desc.mSize = sizeof(DirectionalLight) + sizeof(glm::mat4);
				desc.mBufferType = rhi::RBufferType::ShaderStorage;
				mUniforms.mLightShaderStorage = mRenderDevice->CreateBuffer(desc);
				mRenderDevice->SetShaderStorageBinding(mUniforms.mLightShaderStorage, LUM_SSBO_LIGHTS_BINDING);
			}
			{ // Shadow Framebuffer
				mShadowPass.mDepthBuffer = mRenderDevice->CreateFramebuffer();
				rhi::RFramebufferTextureDescriptor desc;
				desc.mWidth = 1024;
				desc.mHeight = 1024;
				desc.mFormat = rhi::RInternalImageFormat::Depth24Stencil8;
				desc.bGenerateMipmaps = false;
				desc.mAttachment = rhi::RFramebufferAttachment::DepthAttach;

				mShadowPass.mDepthTexture = mRenderDevice->CreateFramebufferTexture(desc);
				mRenderDevice->SetFramebufferDepthTexture(mShadowPass.mDepthBuffer, mShadowPass.mDepthTexture);

			}
		}
		void init_geometry_pass() {

			{ // Geometry pipeline
				rhi::RPipelineDescriptor desc;
				desc.mDepthStencil.mDepth.bEnabled = true;
				desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
				desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::Less;
				desc.mCull.bEnabled = true;
				mGeometryPass.mPipeline = mRenderDevice->CreatePipeline(desc);
			}

			mGeometryPass.mShader = mShaderManager->LoadShader("shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ERootID::Internal);
		}
		void init_environment_pass() {

			float32 vertices[] = {
				-1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1, -1,
				-1, -1,  1,  1, -1,  1,  1,  1,  1, -1,  1,  1,
			};
			uint32 indices[] = {
				0,1,2, 2,3,0, // front
				4,5,6, 6,7,4, // back
				0,3,7, 7,4,0, // left
				1,2,6, 6,5,1, // right
				3,2,6, 6,7,3, // top
				0,1,5, 5,4,0  // bottom
			};
			mEnvironmentPass.mNumIndices = ArraySize(indices);

			{ // Environment VBO
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Static;
				desc.mMapFlags = rhi::RMapFlag::Read;
				desc.mSize = ByteSize(vertices);
				desc.mData = vertices;
				desc.mBufferType = rhi::RBufferType::Vertex;
				mEnvironmentPass.mVbo = mRenderDevice->CreateBuffer(desc);
			}
			{ // Environment EBO
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Static;
				desc.mMapFlags = rhi::RMapFlag::Read;
				desc.mSize = ByteSize(indices);
				desc.mData = indices;
				desc.mBufferType = rhi::RBufferType::Element;
				mEnvironmentPass.mEbo = mRenderDevice->CreateBuffer(desc);
			}
			{ // Environment VAO
				rhi::RVertexAttribute attrs[]{
					{
						.mFormat = rhi::RDataFormat::Vec3,
						.mRelativeOffset = 0,
						.mShaderLocation = LUM_LAYOUT_POSITION
					}
				};
				rhi::RVertexLayoutDescriptor desc;
				desc.mStride = 3 * sizeof(float32);
				desc.mAttributes = attrs;
				mEnvironmentPass.mVao = mRenderDevice->CreateVertexLayout(desc, mEnvironmentPass.mVbo);
				mRenderDevice->AttachElementBufferToLayout(mEnvironmentPass.mEbo, mEnvironmentPass.mVao);
			}
			{ // Environment Pipeline
				rhi::RPipelineDescriptor desc;
				desc.mDepthStencil.mDepth.bEnabled = true;
				desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
				desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::LessEqual;
				mEnvironmentPass.mPipeline = mRenderDevice->CreatePipeline(desc);
			}

			mEnvironmentPass.mShader = mShaderManager->LoadShader("shaders/skybox_pass.vert", "shaders/skybox_pass.frag", ERootID::Internal);
			
		}
		void init_shadow_pass() {

			{ // Shadow Pipeline
				rhi::RPipelineDescriptor desc;
				desc.mDepthStencil.mDepth.bEnabled = true;
				desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
				mShadowPass.mPipeline = mRenderDevice->CreatePipeline(desc);
			}

			mShadowPass.mShader = mShaderManager->LoadShader("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", ERootID::Internal);
		}

	};

}