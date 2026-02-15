#pragma once
#include "object.hpp"
#include "rhi/core/rhi_device.hpp"
#include "camera.hpp"
#include "light.hpp"

#define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)

namespace lum {

	enum class TexturePreset : uint8 {
		Albedo,
		Normal,
		Metalness,
		Roughness,
	};

	enum class PipelinePreset : uint8 {
		Solid,
		Transparent,
		NoDepthBuffer,
		NoDepthWrite
	};

	struct Renderer {
	public:

		Renderer(Window* window) : mWindow(window) {
			mRenderDevice = rhi::CreateDevice(window);
			create_samplers();
			create_buffers();
			create_shaders();
			create_pipelines();
		}

		void LoadModel(Object& obj, ccharptr filename) {
			bool success;
			ModelData newModel = AssetService::LoadMesh(filename, success);
			if (!success) {
				LUM_LOG_ERROR("Something went wrong during loading model %s", filename);
				return;
			}
			obj.mModel = newModel;

			rhi::BufferDescriptor vboDesc;
			vboDesc.mBufferUsage = rhi::BufferUsage::Static;
			vboDesc.mData = obj.mModel.mVertices.data();
			vboDesc.mSize = ByteSize(obj.mModel.mVertices);
			auto vboHandle = mRenderDevice->CreateVertexBuffer(vboDesc);

			rhi::BufferDescriptor eboDesc;
			eboDesc.mBufferUsage = rhi::BufferUsage::Static;
			eboDesc.mData = obj.mModel.mIndices.data();
			eboDesc.mSize = ByteSize(obj.mModel.mIndices);
			auto eboHandle = mRenderDevice->CreateElementBuffer(eboDesc);

			rhi::VertexLayoutDescriptor layoutDesc;
			std::vector<rhi::VertexAttribute> attr(3);
			attr[0].mFormat = rhi::DataFormat::Vec3;
			attr[0].mRelativeOffset = offsetof(Vertex, position);
			attr[0].mShaderLocation = LUM_LAYOUT_POSITION;

			attr[1].mFormat = rhi::DataFormat::Vec2;
			attr[1].mRelativeOffset = offsetof(Vertex, uv);
			attr[1].mShaderLocation = LUM_LAYOUT_UV;

			attr[2].mFormat = rhi::DataFormat::Vec3;
			attr[2].mRelativeOffset = offsetof(Vertex, normal);
			attr[2].mShaderLocation = LUM_LAYOUT_NORMAL;

			layoutDesc.mStride = sizeof(Vertex);
			layoutDesc.mAttributes = attr;

			auto layHandle = mRenderDevice->CreateVertexLayout(layoutDesc, vboHandle);

			mRenderDevice->AttachElementBufferToLayout(eboHandle, layHandle);

			obj.mRenderResources.mVbo = vboHandle;
			obj.mRenderResources.mEbo = eboHandle;
			obj.mRenderResources.mVao = layHandle;

		}

		void LoadTexture2D(Object& obj, ccharptr filename, uint16 binding) {

			bool success;
			TextureData newTexture = AssetService::LoadTexture(filename, success);
			if (!success) {
				LUM_LOG_ERROR("Something went wrong during loading texture %s", filename);
				return;
			}

			rhi::TextureDescriptor descriptor;
			descriptor.mFilename = filename;
			descriptor.mData = newTexture.mPixels.data();
			descriptor.mRect.mWidth = newTexture.mWidth;
			descriptor.mRect.mHeight = newTexture.mHeight;
			auto handle = mRenderDevice->CreateTexture2D(descriptor);

			obj.mRenderResources.mTexture = handle;
			obj.mRenderResources.mTextureBinding = binding;

		}
		void LoadTexture2D(Object& obj, ccharptr filename, const TexturePreset& preset) {

			bool success;
			TextureData newTexture = AssetService::LoadTexture(filename, success);
			if (!success) {
				LUM_LOG_ERROR("Something went wrong during loading texture %s", filename);
				return;
			}

			rhi::TextureDescriptor descriptor = sTexturePresetsLookup[ToUnderlyingEnum(preset)];
			descriptor.mData = newTexture.mPixels.data();
			descriptor.mRect.mWidth = newTexture.mWidth;
			descriptor.mRect.mHeight = newTexture.mHeight;

			auto handle = mRenderDevice->CreateTexture2D(descriptor);

			obj.mRenderResources.mTexture = handle;
			obj.mRenderResources.mTextureBinding = skTextureBindingLookups[ToUnderlyingEnum(preset)];

		}
		void LoadTexture2D(Object& obj, const rhi::TextureDescriptor& desc, uint16 binding) {

			auto handle = mRenderDevice->CreateTexture2D(desc);

			obj.mRenderResources.mTexture = handle;
			obj.mRenderResources.mTextureBinding = binding;

		}

		void UpdateCamera(Camera& camera) {

			camera.Update();

			mCurrentCamera.pos = camera.position;
			mCurrentCamera.proj = camera.projection;
			mCurrentCamera.view = camera.view;

			update_camera_uniform();

		}

		void BeginFrame() {
			mRenderDevice->BeginFrame();
		}
		void EndFrame() {
			mRenderDevice->UpdateBuffer(mLightsSSBO, mLights);
			mRenderDevice->EndFrame();
		}

		void Draw(const Object& obj) {

			const auto& res = obj.mRenderResources;

			update_model_matrix(obj);

			mRenderDevice->BindShader(mGeometryShader);
			mRenderDevice->BindPipeline(mSolidPipeline);
			mRenderDevice->BindSampler(mSamplerRepeat, res.mTextureBinding);
			mRenderDevice->BindTexture(res.mTexture, res.mTextureBinding);
			mRenderDevice->DrawElements(res.mVao, obj.mModel.mIndices.size());

		}

		PointLight mLights[LUM_MAX_LIGHTS]{};

	private:

		Window* mWindow = nullptr;
		rhi::RenderDevice* mRenderDevice = nullptr;
		std::vector<Object> mObjects;

		rhi::SamplerHandle mSamplerLinear;
		rhi::SamplerHandle mSamplerNearest;
		rhi::SamplerHandle mSamplerClamp;
		rhi::SamplerHandle mSamplerRepeat;

		rhi::BufferHandle mCameraUniform;
		rhi::BufferHandle mModelUniform;
		rhi::BufferHandle mLightsSSBO;

		rhi::ShaderHandle mGeometryShader;

		rhi::PipelineHandle mSolidPipeline;

		LUM_UNIFORM_BUFFER_STRUCT CameraUBO {
			glm::mat4 view;
			glm::mat4 proj;
			glm::vec3 pos;
		} mCurrentCamera;

		LUM_UNIFORM_BUFFER_STRUCT ModelUBO {
			math::Mat4 model;
		};

		static inline rhi::TextureDescriptor sTexturePresetsLookup[] = {
			{ // ALBEDO
				.mInternalFormat = rhi::InternalImageFormat::SRGB8_Alpha8,
				.mLoadedFormat = rhi::LoadedImageFormat::RGBA,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true,
			},
			{ // NORMAL
				.mInternalFormat = rhi::InternalImageFormat::RGBA8,
				.mLoadedFormat = rhi::LoadedImageFormat::RGBA,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
			{ // METALNESS
				.mInternalFormat = rhi::InternalImageFormat::R8,
				.mLoadedFormat = rhi::LoadedImageFormat::R,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
			{ // ROUGHNESS
				.mInternalFormat = rhi::InternalImageFormat::R8,
				.mLoadedFormat = rhi::LoadedImageFormat::R,
				.mDataType = rhi::TextureDataType::UnsignedByte,
				.bGenerateMipmaps = true
			},
		};
		LUM_COMPILE_VARIABLE
		static uint16 skTextureBindingLookups[] = {
			LUM_TEX_ALBEDO,
			LUM_TEX_NORMAL,
			LUM_TEX_METALNESS,
			LUM_TEX_ROUGHNESS
		};

		void update_model_matrix(const Object& obj) {

			glm::quat rot = glm::quat(glm::radians(obj.mTransform.rotation));
			glm::mat4 rotation = glm::mat4_cast(rot);
			glm::mat4 model = glm::mat4(1.f);
			model = glm::mat4(1.f);
			model = glm::translate(model, obj.mTransform.position);
			model = model * rotation;
			model = glm::scale(model, obj.mTransform.scale);

			mRenderDevice->UpdateBuffer(mModelUniform, glm::value_ptr(model), 0, 0);

		}

		void update_camera_uniform() {

			mRenderDevice->UpdateBuffer(mCameraUniform, &mCurrentCamera, 0, 0);

		}

		void create_samplers() {
			// Sampler 1: Linear + Mipmaps + Anisotropic
			{
				rhi::SamplerDescriptor desc;
				desc.mMinFilter = rhi::SamplerMinFilter::LinearMipmapLinear;
				desc.mMagFilter = rhi::SamplerMagFilter::Linear;
				desc.mWrapS = rhi::SamplerWrap::Repeat;
				desc.mWrapT = rhi::SamplerWrap::Repeat;
				desc.mAnisotropy = 16;  // max quality

				mSamplerLinear = mRenderDevice->CreateSampler(desc);
			}

			// Sampler 2: Nearest
			{
				rhi::SamplerDescriptor desc;
				desc.mMinFilter = rhi::SamplerMinFilter::Nearest;
				desc.mMagFilter = rhi::SamplerMagFilter::Nearest;
				desc.mWrapS = rhi::SamplerWrap::ClampEdge;
				desc.mWrapT = rhi::SamplerWrap::ClampEdge;

				mSamplerNearest = mRenderDevice->CreateSampler(desc);
			}

			// Sampler 3: Clamp
			{
				rhi::SamplerDescriptor desc;
				desc.mMinFilter = rhi::SamplerMinFilter::Linear;
				desc.mMagFilter = rhi::SamplerMagFilter::Linear;
				desc.mWrapS = rhi::SamplerWrap::ClampEdge;
				desc.mWrapT = rhi::SamplerWrap::ClampEdge;
				desc.mWrapR = rhi::SamplerWrap::ClampEdge;

				mSamplerClamp = mRenderDevice->CreateSampler(desc);
			}

			// Sampler 4: Repeat
			{
				rhi::SamplerDescriptor desc;
				desc.mMinFilter = rhi::SamplerMinFilter::Linear;
				desc.mMagFilter = rhi::SamplerMagFilter::Linear;
				desc.mWrapS = rhi::SamplerWrap::Repeat;
				desc.mWrapT = rhi::SamplerWrap::Repeat;
				desc.mWrapR = rhi::SamplerWrap::Repeat;

				mSamplerRepeat = mRenderDevice->CreateSampler(desc);
			}
		}
		void create_buffers() {

			rhi::BufferDescriptor camDesc;
			camDesc.mSize = sizeof(CameraUBO);
			camDesc.mMapFlags = rhi::Mapflag::Write;
			camDesc.mBufferUsage = rhi::BufferUsage::Dynamic;

			rhi::BufferDescriptor modelDesc;
			modelDesc.mSize = sizeof(ModelUBO);
			modelDesc.mMapFlags = rhi::Mapflag::Write;
			modelDesc.mBufferUsage = rhi::BufferUsage::Dynamic;

			rhi::BufferDescriptor lightsDesc;
			lightsDesc.mBufferUsage = rhi::BufferUsage::Dynamic;
			lightsDesc.mMapFlags = rhi::Mapflag::Write;
			lightsDesc.mSize = sizeof(PointLight) * LUM_MAX_LIGHTS;
			lightsDesc.mData = mLights;

			mCameraUniform = mRenderDevice->CreateUniformBuffer(camDesc);
			mModelUniform = mRenderDevice->CreateUniformBuffer(modelDesc);
			mLightsSSBO = mRenderDevice->CreateShaderStorageBuffer(lightsDesc);
			mRenderDevice->SetUniformBufferBinding(mCameraUniform, LUM_UBO_CAMERA_BINDING);
			mRenderDevice->SetUniformBufferBinding(mModelUniform, LUM_UBO_MODEL_BINDING);
			mRenderDevice->SetShaderStorageBinding(mLightsSSBO, LUM_SSBO_LIGHTS_BINDING);

		}
		void create_shaders() {
			mGeometryShader = mRenderDevice->CreateShader({ "geometry_pass.vert", "geometry_pass.frag" });
		}
		void create_pipelines() {
			rhi::PipelineDescriptor pipDesc;
			
			pipDesc.mDepthStencil.mDepth.bEnabled = true;
			pipDesc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			pipDesc.mDepthStencil.mDepth.mCompare = rhi::CompareFlag::Less;

			
			pipDesc.mCull.bEnabled = true;
			pipDesc.mCull.mFace = rhi::Face::Back;
			pipDesc.mCull.mWindingOrder = rhi::WindingOrder::CounterClockwise;

			pipDesc.mBlend.bEnabled = false;

			pipDesc.mRasterizer.mPolygon.mTopologyMode = rhi::TopologyMode::Fill;

			mSolidPipeline = mRenderDevice->CreatePipeline(pipDesc);
		}

	};

}