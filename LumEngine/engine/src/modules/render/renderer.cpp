

#include "render/renderer.hpp"
#include "core/core_common.hpp"
#include "render/texture_manager.hpp"
#include "render/shader_manager.hpp"
#include "entity/components/transform.hpp"
#include "render/camera.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Renderer::SetEnvionmentTexture(rhi::RTextureHandle tex) {
		mEnvironmentPass.mTexture = tex;
	}

	void Renderer::UpdateCamera(EditorCamera& camera) {

		camera.Update();

		mCurrentCamera.mPosition = camera.position;
		mCurrentCamera.mProjection = camera.projection;
		mCurrentCamera.mView = camera.view;

		upload_camera_uniform();

	}

	void Renderer::Draw(const Object& obj) {

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

	void Renderer::BeginFrame() {
		mRenderDevice->BeginFrame();
		upload_lights();
	}

	void Renderer::EndFrame() {

		mRenderDevice->BindPipeline(mEnvironmentPass.mPipeline);

		mRenderDevice->BindShader(mEnvironmentPass.mShader);
		mRenderDevice->BindTexture(mEnvironmentPass.mTexture, LUM_TEX_CUBEMAP);
		mRenderDevice->DrawElements(mEnvironmentPass.mVao, mEnvironmentPass.mNumIndices);

		mRenderDevice->EndFrame();
	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::upload_model_matrix(const CTransform& obj) {

		glm::quat rot = glm::quat(glm::radians(obj.mRotation));
		glm::mat4 rotation = glm::mat4_cast(rot);
		glm::mat4 model = glm::mat4(1.f);
		model = glm::mat4(1.f);
		model = glm::translate(model, obj.mPosition);
		model = model * rotation;
		model = glm::scale(model, obj.mScale);

		mRenderDevice->UpdateBuffer(mUniforms.mModelUniform, glm::value_ptr(model), 0, 0);

	}

	void Renderer::upload_material(const FMaterialInstance& mat) {

		mCurrentMaterial.mBaseColor = mat.mBaseColor;
		mCurrentMaterial.mRougness = mat.mRoughness;
		mCurrentMaterial.mMetallic = mat.mMetallic;

		mRenderDevice->UpdateBuffer(mUniforms.mMaterialUniform, &mCurrentMaterial);

	}

	void Renderer::upload_lights() {
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

	void Renderer::upload_camera_uniform() {

		mRenderDevice->UpdateBuffer(mUniforms.mCameraUniform, &mCurrentCamera, 0, 0);

	}

	void Renderer::init_buffers() {
		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::EBufferUsage::Dynamic;
		desc.mMapFlags = rhi::EMapFlag::Write;

		{ // Camera Uniform
			desc.mSize = sizeof(detail::CameraUniformBuffer);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mUniforms.mCameraUniform = mRenderDevice->CreateBuffer(desc);
			mRenderDevice->SetUniformBufferBinding(mUniforms.mCameraUniform, LUM_UBO_CAMERA_BINDING);
		}
		{ // Model Uniform
			desc.mSize = sizeof(detail::ModelUniformBuffer);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mUniforms.mModelUniform = mRenderDevice->CreateBuffer(desc);
			mRenderDevice->SetUniformBufferBinding(mUniforms.mModelUniform, LUM_UBO_MODEL_BINDING);
		}
		{ // Material Uniform
			desc.mSize = sizeof(detail::MaterialUniformBuffer);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mUniforms.mMaterialUniform = mRenderDevice->CreateBuffer(desc);
			mRenderDevice->SetUniformBufferBinding(mUniforms.mMaterialUniform, LUM_UBO_MATERIAL_BINDING);
		}
		{ // Light SSBO
			desc.mSize = sizeof(DirectionalLight) + sizeof(glm::mat4);
			desc.mBufferType = rhi::EBufferType::ShaderStorage;
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
	void Renderer::init_geometry_pass() {

		{ // Geometry pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::Less;
			desc.mCull.bEnabled = true;
			mGeometryPass.mPipeline = mRenderDevice->CreatePipeline(desc);
		}

		mGeometryPass.mShader = mShaderManager->LoadShader("shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ERootID::Internal);
	}
	void Renderer::init_environment_pass() {

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
			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::EBufferUsage::Static;
			desc.mMapFlags = rhi::EMapFlag::Read;
			desc.mSize = ByteSize(vertices);
			desc.mData = vertices;
			desc.mBufferType = rhi::EBufferType::Vertex;
			mEnvironmentPass.mVbo = mRenderDevice->CreateBuffer(desc);
		}
		{ // Environment EBO
			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::EBufferUsage::Static;
			desc.mMapFlags = rhi::EMapFlag::Read;
			desc.mSize = ByteSize(indices);
			desc.mData = indices;
			desc.mBufferType = rhi::EBufferType::Element;
			mEnvironmentPass.mEbo = mRenderDevice->CreateBuffer(desc);
		}
		{ // Environment VAO
			rhi::RVertexAttribute attrs[]{
				{
					.mFormat = rhi::EDataFormat::Vec3,
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
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::LessEqual;
			mEnvironmentPass.mPipeline = mRenderDevice->CreatePipeline(desc);
		}

		mEnvironmentPass.mShader = mShaderManager->LoadShader("shaders/skybox_pass.vert", "shaders/skybox_pass.frag", ERootID::Internal);

	}
	void Renderer::init_shadow_pass() {

		{ // Shadow Pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			mShadowPass.mPipeline = mRenderDevice->CreatePipeline(desc);
		}

		mShadowPass.mShader = mShaderManager->LoadShader("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", ERootID::Internal);
	}

}