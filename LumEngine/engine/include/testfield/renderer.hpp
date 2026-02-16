#pragma once
#include "object.hpp"
#include "rhi/core/rhi_device.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material_manager.hpp"
#include "core/utils/handle_pool.hpp"

#define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)

namespace lum {

	struct TextureHandle : cstd::BaseHandle {};

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

		PointLight mLights[LUM_MAX_LIGHTS]{};
		MaterialManager mMaterialManager;

		Renderer(Window* window) : mWindow(window) {
			mRenderDevice = rhi::CreateDevice(window);
			create_samplers();
			create_buffers();
			create_shaders();
			create_pipelines();
			create_textures();
			mMaterialManager.Init(this);
		}

		void LoadModel(Object& obj, ccharptr filename);

		TextureHandle LoadTexture2D(ccharptr filename, uint16 binding);
		TextureHandle LoadTexture2D(ccharptr filename, const TexturePreset& preset);
		TextureHandle LoadTexture2D(const rhi::TextureDescriptor& desc, uint16 binding);

		rhi::TextureHandle GetTexture(const TextureHandle& handle);

		void UpdateCamera(Camera& camera);

		void BeginFrame();
		void EndFrame();

		void Draw(const Object& obj);

	private:

		friend class MaterialManager;
	   
		cstd::handle_pool<rhi::TextureHandle, TextureHandle> mTextures { limits::gMaxTextures };

		Window* mWindow = nullptr;
		rhi::RenderDevice* mRenderDevice = nullptr;

		rhi::TextureHandle mDefaultTexture;

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

		void update_model_matrix	( const Object& obj );
		void update_camera_uniform	( );

		void create_shaders		( );
		void create_samplers	( );
		void create_buffers		( );
		void create_pipelines	( );
		void create_textures	( );

	};

}