#pragma once

#include "core/core_common.hpp"
#include "testfield/texture_manager.hpp"
#include "testfield/material_manager.hpp"
#include "testfield/model_manager.hpp"
#include "entity/components/c_transform.hpp"
#include "camera.hpp"

#define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)

namespace lum {

	struct Object {
		CTransform mTransform;
		StaticMeshHandle mStaticMesh;
		FMaterialInstance mMaterial;
	};

	class SRenderer {
	public:
		
		SRenderer(rhi::RDevice* device, MTextureManager* texMgr, MMaterialManager* matMgr, MMeshManager* meshMgr) 
			: mRenderDevice(device), mTextureManager(texMgr), mMaterialManager(matMgr), mMeshManager(meshMgr) 
				{ create_uniforms(); }

		void BeginFrame() {
			mRenderDevice->BeginFrame();
		}
		void EndFrame() {
			mRenderDevice->EndFrame();
		}

		void UpdateCamera(Camera& camera) {

			camera.Update();

			mCurrentCamera.pos = camera.position;
			mCurrentCamera.proj = camera.projection;
			mCurrentCamera.view = camera.view;

			update_camera_uniform();

		}

		void Draw(const Object& obj, rhi::RShaderHandle shader) {

			const FStaticMeshResource& res = mMeshManager->GetStatic(obj.mStaticMesh);
			const auto& mat = obj.mMaterial;

			update_model_matrix(obj.mTransform);
			update_material(obj.mMaterial);

			mRenderDevice->ToggleDepthTest(true);
			mRenderDevice->BindShader(shader);
			mRenderDevice->BindTexture(mat.mAlbedoMap, LUM_TEX_ALBEDO);
			mRenderDevice->BindTexture(mat.mNormalMap, LUM_TEX_NORMAL);
			mRenderDevice->BindTexture(mat.mMetallicMap, LUM_TEX_METALNESS);
			mRenderDevice->BindTexture(mat.mRoughnessMap, LUM_TEX_ROUGHNESS);

			mRenderDevice->DrawElements(res.mVao, res.mNumIndices);

		}


	private:

		rhi::RBufferHandle mCameraUniform;
		rhi::RBufferHandle mModelUniform;
		rhi::RBufferHandle mMaterialUniform;

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

		void update_camera_uniform() {

			mRenderDevice->UpdateBuffer(mCameraUniform, &mCurrentCamera, 0, 0);

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
		}

		MTextureManager*	mTextureManager		= nullptr;
		MMaterialManager*	mMaterialManager	= nullptr;
		MMeshManager*		mMeshManager		= nullptr;
		rhi::RDevice*		mRenderDevice		= nullptr;

	};

}