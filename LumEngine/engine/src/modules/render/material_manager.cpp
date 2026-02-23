//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Manages shared material bases and per-entity instances.
//
//=============================================================================//

#include "render/material_manager.hpp"

#include "rhi/core/rhi_device.hpp"
#include "render/texture_manager.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MMaterialManager::Initialize(rhi::RDevice* device, MTextureManager* texMgr) {

		mRenderDevice = device;
		mTextureManager = texMgr;

		init();

	}

	MaterialBaseHandle MMaterialManager::UploadBase(const FMaterialBase& base) {

		return mBaseMaterials.Append(base);

	}

	FMaterialInstance MMaterialManager::CreateInstance(MaterialBaseHandle base) {

		if (!mBaseMaterials.Contains(base))
			base = mDefaultMaterial;

		FMaterialInstance instance;
		FMaterialBase matBase = mBaseMaterials[base];

		instance.mAlbedoMap = validate_texture(matBase.mAlbedoMap);
		instance.mNormalMap = validate_texture(matBase.mNormalMap);
		instance.mMetallicMap = validate_texture(matBase.mMetallicMap);
		instance.mRoughnessMap = validate_texture(matBase.mRoughnessMap);
		instance.mAmbientMap = validate_texture(matBase.mAmbientMap);

		instance.mBaseColor = matBase.mBaseColor;
		instance.mRoughness = matBase.mRoughness;
		instance.mMetallic = matBase.mMetallic;
		instance.mAmbient = matBase.mAmbient;

		return instance;

	}

	void MMaterialManager::SetBaseMap(MaterialBaseHandle material, EMaterialMember mem, rhi::RTextureHandle tex) {
		switch (mem) {
		case EMaterialMember::Albedo: mBaseMaterials[material].mAlbedoMap = tex; break;
		case EMaterialMember::Normal: mBaseMaterials[material].mNormalMap = tex; break;
		case EMaterialMember::Metallic: mBaseMaterials[material].mMetallicMap = tex; break;
		case EMaterialMember::Roughness: mBaseMaterials[material].mRoughnessMap = tex; break;
		}
	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void MMaterialManager::init() {
		create_materials();
	}

	rhi::RTextureHandle MMaterialManager::validate_texture(rhi::RTextureHandle tex) {

		if (!mRenderDevice->IsValid(tex)) {
			LUM_LOG_DEBUG("No texture given, setting default fallback");
			return mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
		}
		else
			return tex;

	}

	void MMaterialManager::create_materials() {

		{ // Base material
			FMaterialBase base;
			base.mAlbedoMap = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mNormalMap = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mMetallicMap = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mRoughnessMap = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mAmbientMap = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);

			mDefaultMaterial = UploadBase(base);

		}

	}

} // namespace lum