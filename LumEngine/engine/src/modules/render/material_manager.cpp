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

	MaterialBaseHandle MMaterialManager::UploadBase( const FMaterialBase& base ) {

		return mBaseMaterials.Append(base);

	}

	FMaterialInstance MMaterialManager::CreateInstance( MaterialBaseHandle base ) {

		if (!mBaseMaterials.Contains(base))
			base = mDefaultMaterial;

		FMaterialInstance instance;
		FMaterialBase matBase = mBaseMaterials[base];

		instance = static_cast<FMaterialInstance>(matBase);

		return instance;

	}

	FMaterialInstance MMaterialManager::GetDefaultInstance() {
		return CreateInstance(mDefaultMaterial);
	}

	void MMaterialManager::SetBaseMap(MaterialBaseHandle material, EMaterialMember mem, rhi::RTextureHandle tex) {
		switch (mem) {
		case EMaterialMember::Albedo: mBaseMaterials[material].mAlbedoTex = tex; break;
		case EMaterialMember::Normal: mBaseMaterials[material].mNormalTex = tex; break;
		case EMaterialMember::Metallic: mBaseMaterials[material].mMetallicTex = tex; break;
		case EMaterialMember::Roughness: mBaseMaterials[material].mRoughnessTex = tex; break;
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
			base.mAlbedoTex = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mNormalTex = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mMetallicTex = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mRoughnessTex = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);
			base.mAmbientTex = mTextureManager->GetFallbackTexture(EFallbackTexture::Default);

			mDefaultMaterial = UploadBase(base);

		}

	}

} // namespace lum