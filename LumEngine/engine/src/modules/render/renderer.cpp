

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

	void Renderer::Initialize(const FRendererContext& ctx) {

		mContext = ctx;

		init();

	}

	void Renderer::SetEnvironmentTexture(rhi::RTextureHandle tex) {
		mEnvironmentPass.mTexture = tex;
	}

	void Renderer::UpdateCamera(const FRenderCamera& camera) {

		mCurrentCamera.mPosition = glm::vec4(camera.mPosition, 0.0);
		mCurrentCamera.mProjection = camera.mProjection;
		mCurrentCamera.mView = camera.mView;

		upload_camera_uniform();

	}

	void Renderer::AddPointLight(const FPointLight& light) {

		LUM_ASSERT(mLights.mActivePoints + 1 <= mLights.mPoints.size(), "Max point lights reached");

		mLights.mPoints[ mLights.mActivePoints++ ] = light;

	}

	void Renderer::SetDirectionalLight(const FDirectionalLight& light) {

		mDirectionalLightUBO.mColor = glm::vec4(light.mColor, 0.0);
		mDirectionalLightUBO.mDirection = glm::vec4(light.mDirection, 0.0);
		mDirectionalLightUBO.mIntensity = light.mIntensity;

	}
	FDirectionalLight Renderer::GetDirectionalLight() {

		return { 
			glm::vec3(mDirectionalLightUBO.mDirection), 
			mDirectionalLightUBO.mIntensity, 
			glm::vec3(mDirectionalLightUBO.mColor) 
		};

	}

	void Renderer::Draw(const FRenderInstance& obj) {

		const FStaticMeshResource& res = mContext.mMeshMgr->GetStatic(obj.mStaticMesh);
		const auto& mat = obj.mMaterial;

		upload_model_matrix(obj);
		upload_material(obj.mMaterial);

		mContext.mRenderDevice->BindPipeline(mGeometryPass.mPipeline);
		mContext.mRenderDevice->BindShader(mGeometryPass.mShader);
		mContext.mRenderDevice->BindTexture(mat.mAlbedoTex, LUM_TEX_ALBEDO);
		mContext.mRenderDevice->BindTexture(mat.mNormalTex, LUM_TEX_NORMAL);
		mContext.mRenderDevice->BindTexture(mat.mMetallicTex, LUM_TEX_METALNESS);
		mContext.mRenderDevice->BindTexture(mat.mRoughnessTex, LUM_TEX_ROUGHNESS);

		mContext.mRenderDevice->DrawElements(res.mVao, res.mNumIndices);

	}

	void Renderer::BeginFrame() {
		mContext.mRenderDevice->BeginPass();
		upload_directional_light();
		upload_point_lights();
		clear_point_lights();
	}

	void Renderer::EndFrame() {
		
		mContext.mRenderDevice->BindPipeline(mEnvironmentPass.mPipeline);

		mContext.mRenderDevice->BindShader(mEnvironmentPass.mShader);
		mContext.mRenderDevice->BindTexture(mEnvironmentPass.mTexture, LUM_TEX_CUBEMAP);
		mContext.mRenderDevice->DrawElements(mEnvironmentPass.mVao, mEnvironmentPass.mNumIndices);

		mContext.mRenderDevice->EndPass();
	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init() {
		prep_buffers();
		prep_geometry_pass();
		prep_environment_pass();
		prep_shadow_pass();
	}

	void Renderer::clear_point_lights() {

		mLights.mActivePoints = 0;

	}

	void Renderer::update_shadow_pass() {

		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 500.f);
		glm::mat4 lightView = 
			glm::lookAt(
				glm::vec3(-mDirectionalLightUBO.mDirection) * 30.f, 
				glm::vec3(0.0f), 
				glm::vec3(0.0f, 1.0f, 0.0f)
			);

		mShadowPass.mLightSpaceMatrix = lightProjection * lightView;

	}

	void Renderer::upload_model_matrix( const FRenderInstance& obj ) {

		glm::quat rot = glm::quat(glm::radians(obj.mRotation));
		glm::mat4 rotation = glm::mat4_cast(rot);
		glm::mat4 model = glm::mat4(1.f);
		model = glm::mat4(1.f);
		model = glm::translate(model, obj.mPosition);
		model = model * rotation;
		model = glm::scale(model, obj.mScale);

		mContext.mRenderDevice->UpdateBuffer(mUniforms.mModelUniform, glm::value_ptr(model), 0, 0);

	}

	void Renderer::upload_lightspace_matrix(const glm::mat4& mat) {

		

	}

	void Renderer::upload_material(const FMaterialInstance& mat) {

		mMaterialUBO.mBaseColor = glm::vec4(mat.mBaseColor, 1.0);
		mMaterialUBO.mRoughness = mat.mRoughnessValue;
		mMaterialUBO.mMetallic = mat.mMetallicValue;

		mContext.mRenderDevice->UpdateBuffer(mUniforms.mMaterialUniform, &mMaterialUBO);

	}

	void Renderer::upload_point_lights() {

		mContext.mRenderDevice->UpdateBuffer(
			mUniforms.mPointLights, 
			mLights.mPoints.data(), 
			0, 
			sizeof(FPointLight) * mLights.mActivePoints
		);

		mContext.mRenderDevice->UpdateBuffer(
			mUniforms.mPointLights, 
			&mLights.mActivePoints, 
			sizeof(FPointLight) * LUM_MAX_LIGHTS, 
			sizeof(mLights.mActivePoints)
		);

	}
	void Renderer::upload_directional_light() {

		mContext.mRenderDevice->UpdateBuffer(
			mUniforms.mDirectionalLight,
			&mDirectionalLightUBO
		);

	}
	void Renderer::upload_camera_uniform() {

		mContext.mRenderDevice->UpdateBuffer(mUniforms.mCameraUniform, &mCurrentCamera, 0, 0);

	}

	void Renderer::prep_buffers() {
		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::EBufferUsage::Dynamic;
		desc.mMapFlags = rhi::EMapFlag::Write;

		{ // Camera Uniform
			desc.mSize = sizeof(detail::FCameraUniformBuffer);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mUniforms.mCameraUniform = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mUniforms.mCameraUniform, LUM_UBO_CAMERA_BINDING);
		}
		{ // Model Uniform
			desc.mSize = sizeof(detail::FModelUniformBuffer);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mUniforms.mModelUniform = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mUniforms.mModelUniform, LUM_UBO_MODEL_BINDING);
		}
		{ // Material Uniform
			desc.mSize = sizeof(detail::FMaterialUniformBuffer);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mUniforms.mMaterialUniform = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mUniforms.mMaterialUniform, LUM_UBO_MATERIAL_BINDING);
		}
		{ // Point Lights SSBO
			desc.mSize = sizeof(FPointLight) * LUM_MAX_LIGHTS + sizeof(int);
			desc.mBufferType = rhi::EBufferType::ShaderStorage;
			mUniforms.mPointLights = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetShaderStorageBinding(mUniforms.mPointLights, LUM_SSBO_LIGHTS_BINDING);
		}
		{ // Directional Light UBO
			desc.mSize = sizeof(mDirectionalLightUBO);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mUniforms.mDirectionalLight = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mUniforms.mDirectionalLight, LUM_UBO_DIRECTIONAL_LIGHT);
		}
		{ // Shadow Framebuffer
			mShadowPass.mDepthBuffer = mContext.mRenderDevice->CreateFramebuffer();
			rhi::RFramebufferTextureDescriptor desc;
			desc.mWidth = 1024;
			desc.mHeight = 1024;
			desc.mFormat = rhi::RImageLayout::Depth24Stencil8;
			desc.bGenerateMipmaps = false;
			desc.mAttachment = rhi::RFramebufferAttachment::DepthAttach;

			mShadowPass.mDepthTexture = mContext.mRenderDevice->CreateFramebufferTexture(desc);
			mContext.mRenderDevice->SetFramebufferDepthTexture(mShadowPass.mDepthBuffer, mShadowPass.mDepthTexture);

		}
	}
	void Renderer::prep_geometry_pass() {

		{ // Geometry pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::Less;
			desc.mCull.bEnabled = true;
			mGeometryPass.mPipeline = mContext.mRenderDevice->CreatePipeline(desc);
		}

		mGeometryPass.mShader = mContext.mShaderMgr->LoadShader("shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ERootID::Internal);
	}
	void Renderer::prep_environment_pass() {

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
			mEnvironmentPass.mVbo = mContext.mRenderDevice->CreateBuffer(desc);
		}
		{ // Environment EBO
			rhi::FBufferDescriptor desc;
			desc.mBufferUsage = rhi::EBufferUsage::Static;
			desc.mMapFlags = rhi::EMapFlag::Read;
			desc.mSize = ByteSize(indices);
			desc.mData = indices;
			desc.mBufferType = rhi::EBufferType::Element;
			mEnvironmentPass.mEbo = mContext.mRenderDevice->CreateBuffer(desc);
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
			mEnvironmentPass.mVao = mContext.mRenderDevice->CreateVertexLayout(desc, mEnvironmentPass.mVbo);
			mContext.mRenderDevice->AttachElementBufferToLayout(mEnvironmentPass.mEbo, mEnvironmentPass.mVao);
		}
		{ // Environment Pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			desc.mDepthStencil.mDepth.mCompare = rhi::RCompareFlag::LessEqual;
			mEnvironmentPass.mPipeline = mContext.mRenderDevice->CreatePipeline(desc);
		}

		mEnvironmentPass.mShader = mContext.mShaderMgr->LoadShader("shaders/skybox_pass.vert", "shaders/skybox_pass.frag", ERootID::Internal);

	}
	void Renderer::prep_shadow_pass() {

		{ // Shadow Pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			mShadowPass.mPipeline = mContext.mRenderDevice->CreatePipeline(desc);
		}

		//mShadowPass.mShader = mShaderManager->LoadShader("shaders/shadow_pass.vert", "shaders/shadow_pass.frag", ERootID::Internal);
	}

}