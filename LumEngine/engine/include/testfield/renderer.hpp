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
		
		rhi::RDevice* mRenderDevice = nullptr;
		MTextureManager* mTextureManager = nullptr;
		MMaterialManager* mMaterialManager = nullptr;
		MMeshManager* mMeshManager = nullptr;
		MShaderManager* mShaderManager = nullptr;

	};

	struct FEnvironmentResource {

		rhi::RTextureHandle			mTexture;
		rhi::RBufferHandle			mVbo;
		rhi::RBufferHandle			mEbo;
		rhi::RVertexLayoutHandle	mVao;

		uint32 mNumIndices = 0;

	};

	struct FShadowPass {

		rhi::RFramebufferHandle mDepthBuffer;
		rhi::RTextureHandle mDepthTexture;
		rhi::RPipelineHandle mPipeline;
		glm::mat4 mLightSpaceMatrix;

	};

	class SRenderer {
	public:

		SRenderer(const FRendererContext& ctx) : 
			mRenderDevice(ctx.mRenderDevice), 
			mTextureManager(ctx.mTextureManager), 
			mMaterialManager(ctx.mMaterialManager), 
			mMeshManager(ctx.mMeshManager),
			mShaderManager(ctx.mShaderManager) 
		{
			create_uniforms();
			create_shaders();
			create_skybox_mesh();
			create_pipelines();
		}

		void SetEnvionmentTexture(rhi::RTextureHandle tex) {
			mEnvironment.mTexture = tex;
		}

		void BeginFrame() {
			mRenderDevice->BeginFrame();
			update_lights();
			update_shadows();
		}
		void EndFrame() {

			mRenderDevice->BindPipeline(mSkyboxPipeline);

			mRenderDevice->BindShader(mSkyboxShader);
			mRenderDevice->BindTexture(mEnvironment.mTexture, LUM_TEX_CUBEMAP);
			mRenderDevice->DrawElements(mEnvironment.mVao, mEnvironment.mNumIndices);

			mRenderDevice->EndFrame();
		}

		void UpdateCamera(Camera& camera) {

			camera.Update();

			mCurrentCamera.pos = camera.position;
			mCurrentCamera.proj = camera.projection;
			mCurrentCamera.view = camera.view;

			update_camera_uniform();

		}

		void Draw(const Object& obj) {

			const FStaticMeshResource& res = mMeshManager->GetStatic(obj.mStaticMesh);
			const auto& mat = obj.mMaterial;

			update_model_matrix(obj.mTransform);
			update_material(obj.mMaterial);

			mRenderDevice->BindPipeline(mGeometryPipeline);
			mRenderDevice->BindShader(mGeometryShader);
			mRenderDevice->BindTexture(mat.mAlbedoMap, LUM_TEX_ALBEDO);
			mRenderDevice->BindTexture(mat.mNormalMap, LUM_TEX_NORMAL);
			mRenderDevice->BindTexture(mat.mMetallicMap, LUM_TEX_METALNESS);
			mRenderDevice->BindTexture(mat.mRoughnessMap, LUM_TEX_ROUGHNESS);

			mRenderDevice->DrawElements(res.mVao, res.mNumIndices);

		}

		DirectionalLight* mDirectionalLight = nullptr;

	private:

		MTextureManager*	mTextureManager = nullptr;
		MMaterialManager*	mMaterialManager = nullptr;
		MMeshManager*		mMeshManager = nullptr;
		MShaderManager*		mShaderManager = nullptr;
		rhi::RDevice*		mRenderDevice = nullptr;

		rhi::RBufferHandle mCameraUniform;
		rhi::RBufferHandle mModelUniform;
		rhi::RBufferHandle mMaterialUniform;
		rhi::RBufferHandle mLightShaderStorage;

		rhi::RPipelineHandle	mGeometryPipeline;
		rhi::RShaderHandle		mGeometryShader;
		rhi::RPipelineHandle	mSkyboxPipeline;
		rhi::RShaderHandle		mSkyboxShader;
		rhi::RShaderHandle mShadowShader;

		FEnvironmentResource mEnvironment;

		FShadowPass mShadowPass;

		LUM_UNIFORM_BUFFER_STRUCT CameraUBO {
			glm::mat4 view;
			glm::mat4 proj;
			glm::vec3 pos;
		} mCurrentCamera;

		LUM_UNIFORM_BUFFER_STRUCT ModelUBO {
			glm::mat4 model;
		};

		LUM_UNIFORM_BUFFER_STRUCT MaterialUBO {
			glm::vec3 mBaseColor = glm::vec3(1.0f);
			float32 mRougness = 0.5f;
			float32 mMetallic = 0.0f;
		} mCurrentMaterial;

		void update_model_matrix(const CTransform& obj) {

			glm::quat rot = glm::quat(glm::radians(obj.rotation));
			glm::mat4 rotation = glm::mat4_cast(rot);
			glm::mat4 model = glm::mat4(1.f);
			model = glm::mat4(1.f);
			model = glm::translate(model, obj.position);
			model = model * rotation;
			model = glm::scale(model, obj.scale);

			mRenderDevice->UpdateBuffer(mModelUniform, glm::value_ptr(model), 0, 0);

		}

		void update_material(const FMaterialInstance& mat) {

			mCurrentMaterial.mBaseColor = mat.mBaseColor;
			mCurrentMaterial.mRougness = mat.mRoughness;
			mCurrentMaterial.mMetallic = mat.mMetallic;

			mRenderDevice->UpdateBuffer(mMaterialUniform, &mCurrentMaterial);

		}

		void update_lights() {
			if(mDirectionalLight)
				mRenderDevice->UpdateBuffer(mLightShaderStorage, mDirectionalLight, 0, sizeof(DirectionalLight));

			glm::vec3 lightDir = glm::normalize(mDirectionalLight->mDirection);
			glm::vec3 lightPos = -lightDir * 20.0f;

			glm::mat4 view = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

			mShadowPass.mLightSpaceMatrix = proj * view;

			mRenderDevice->UpdateBuffer(mLightShaderStorage, glm::value_ptr(mShadowPass.mLightSpaceMatrix), sizeof(DirectionalLight), sizeof(glm::mat4));
		}

		// TODO remove lights from renderer and use ecs components
		void update_camera_uniform() {

			mRenderDevice->UpdateBuffer(mCameraUniform, &mCurrentCamera, 0, 0);

		}

		void update_shadows() {
			mRenderDevice->BindPipeline(mShadowPass.mPipeline);
			mRenderDevice->BindShader(mShadowShader);
		}

		void create_uniforms() {
			{
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Dynamic;
				desc.mMapFlags = rhi::RMapFlag::Write;
				desc.mSize = sizeof(CameraUBO);
				mCameraUniform = mRenderDevice->CreateUniformBuffer(desc);
				mRenderDevice->SetUniformBufferBinding(mCameraUniform, LUM_UBO_CAMERA_BINDING);
			}
			{
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Dynamic;
				desc.mMapFlags = rhi::RMapFlag::Write;
				desc.mSize = sizeof(ModelUBO);
				mModelUniform = mRenderDevice->CreateUniformBuffer(desc);
				mRenderDevice->SetUniformBufferBinding(mModelUniform, LUM_UBO_MODEL_BINDING);
			}
			{
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Dynamic;
				desc.mMapFlags = rhi::RMapFlag::Write;
				desc.mSize = sizeof(MaterialUBO);
				mMaterialUniform = mRenderDevice->CreateUniformBuffer(desc);
				mRenderDevice->SetUniformBufferBinding(mMaterialUniform, LUM_UBO_MATERIAL_BINDING);
			}
			{
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Dynamic;
				desc.mMapFlags = rhi::RMapFlag::Write;
				desc.mSize = sizeof(DirectionalLight) + sizeof(glm::mat4);
				mLightShaderStorage = mRenderDevice->CreateShaderStorageBuffer(desc);
				mRenderDevice->SetShaderStorageBinding(mLightShaderStorage, LUM_SSBO_LIGHTS_BINDING);
			}
			{
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

		void create_skybox_mesh() {
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
			mEnvironment.mNumIndices = ArraySize(indices);

			{
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Static;
				desc.mMapFlags = rhi::RMapFlag::Read;
				desc.mSize = ByteSize(vertices);
				desc.mData = vertices;
				mEnvironment.mVbo = mRenderDevice->CreateVertexBuffer(desc);
			}
			{
				rhi::RBufferDescriptor desc;
				desc.mBufferUsage = rhi::RBufferUsage::Static;
				desc.mMapFlags = rhi::RMapFlag::Read;
				desc.mSize = ByteSize(indices);
				desc.mData = indices;
				mEnvironment.mEbo = mRenderDevice->CreateElementBuffer(desc);
			}
			{
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
				mEnvironment.mVao = mRenderDevice->CreateVertexLayout(desc, mEnvironment.mVbo);
			}

			mRenderDevice->AttachElementBufferToLayout(mEnvironment.mEbo, mEnvironment.mVao);
			
		}

		void create_shaders() {
			mGeometryShader = mShaderManager->LoadShader("shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ERootID::Internal);
			mSkyboxShader = mShaderManager->LoadShader("shaders/skybox_pass.vert", "shaders/skybox_pass.frag", ERootID::Internal);
			mShadowShader = mShaderManager->LoadShader("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", ERootID::Internal);
		}

		void create_pipelines() {
			{
				rhi::RPipelineDescriptor desc;
				desc.mDepthStencil.mDepth.bEnabled = true;
				desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
				desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::Less;
				desc.mCull.bEnabled = true;
				mGeometryPipeline = mRenderDevice->CreatePipeline(desc);
			}

			{
				rhi::RPipelineDescriptor desc;
				desc.mDepthStencil.mDepth.bEnabled = true;
				desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
				desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::LessEqual;
				mSkyboxPipeline = mRenderDevice->CreatePipeline(desc);
			}

			{
				rhi::RPipelineDescriptor desc;
				desc.mDepthStencil.mDepth.bEnabled = true;
				desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
				mShadowPass.mPipeline = mRenderDevice->CreatePipeline(desc);
			}

		}

	};

}