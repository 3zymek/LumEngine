#pragma once

#include "core/utils/handle_pool.hpp"
#include "core/core_common.hpp"
#include "rhi/core/rhi_device.hpp"
#include "texture_manager.hpp"

namespace lum {

	struct FMaterialBase {

		rhi::RTextureHandle mAlbedoMap;
		rhi::RTextureHandle mNormalMap;
		rhi::RTextureHandle mRoughnessMap;
		rhi::RTextureHandle mMetallicMap;
		rhi::RTextureHandle mAmbientMap;

		glm::vec3 mBaseColor = glm::vec3(1.0f);
		float32 mRoughness = 0.5f;
		float32 mMetallic = 0.0f;
		float32 mAmbient = 1.0f;

	};

	struct MaterialBaseHandle : cstd::BaseHandle<uint32> {};

	struct FMaterialInstance {

		rhi::RTextureHandle mAlbedoMap;
		rhi::RTextureHandle mNormalMap;
		rhi::RTextureHandle mRoughnessMap;
		rhi::RTextureHandle mMetallicMap;
		rhi::RTextureHandle mAmbientMap;

		glm::vec3 mBaseColor = glm::vec3(1.0f);
		float32 mRoughness = 0.5f;
		float32 mMetallic = 0.0f;
		float32 mAmbient = 1.0f;

	};

	enum class EMaterialMember {
		
		Albedo,
		Normal,
		Metallic,
		Roughness,
	
	};

	class MMaterialManager {
	public:

		MMaterialManager ( rhi::RDevice* device, MTextureManager* texMgr ) : mRenderDevice(device), mTextureManager(texMgr) 
			{ create_materials(); }

		LUM_NODISCARD
		MaterialBaseHandle UploadBase ( const FMaterialBase& base ) {

			return mBaseMaterials.CreateHandle(base);

		}

		FMaterialInstance CreateInstance ( MaterialBaseHandle base ) {

			if (!mBaseMaterials.Exist(base))
				base = mDefaultMaterial;

			FMaterialInstance instance;
			FMaterialBase matBase = mBaseMaterials[base];
			
			instance.mAlbedoMap		= validate_texture(matBase.mAlbedoMap);
			instance.mNormalMap		= validate_texture(matBase.mNormalMap);
			instance.mMetallicMap	= validate_texture(matBase.mMetallicMap);
			instance.mRoughnessMap	= validate_texture(matBase.mRoughnessMap);
			instance.mAmbientMap	= validate_texture(matBase.mAmbientMap);

			instance.mBaseColor = matBase.mBaseColor;
			instance.mRoughness = matBase.mRoughness;
			instance.mMetallic = matBase.mMetallic;
			instance.mAmbient = matBase.mAmbient;

			return instance;

		}

		void SetBaseMap ( MaterialBaseHandle material, EMaterialMember mem, rhi::RTextureHandle tex ) {
			switch (mem) {
				case EMaterialMember::Albedo: mBaseMaterials[material].mAlbedoMap = tex; break;
				case EMaterialMember::Normal: mBaseMaterials[material].mNormalMap = tex; break;
				case EMaterialMember::Metallic: mBaseMaterials[material].mMetallicMap = tex; break;
				case EMaterialMember::Roughness: mBaseMaterials[material].mRoughnessMap = tex; break;
			}
		}


	private:

		MaterialBaseHandle mDefaultMaterial;

		MTextureManager* mTextureManager = nullptr;
		rhi::RDevice* mRenderDevice = nullptr;

		cstd::handle_pool<FMaterialBase, MaterialBaseHandle> mBaseMaterials{ limits::gMaxMaterials };

		rhi::RTextureHandle validate_texture ( rhi::RTextureHandle tex ) {

			if (!mRenderDevice->IsValid(tex)) {
				LUM_LOG_DEBUG("No texture given, setting default fallback");
				return mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			}
			else
				return tex;

		}

		void create_materials( ) {

			{ // Base material
				FMaterialBase base;
				base.mAlbedoMap		= mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
				base.mNormalMap		= mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
				base.mMetallicMap	= mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
				base.mRoughnessMap	= mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
				base.mAmbientMap	= mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
				
				mDefaultMaterial = UploadBase(base);

			}

		}

	};


}