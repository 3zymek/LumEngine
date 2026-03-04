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
		mTextureMgr = texMgr;

		init();

	}

	MaterialBaseHandle MMaterialManager::UploadBase( const FMaterialBase& base ) {

		return mBaseMaterials.Append(base);

	}

	MaterialBaseHandle MMaterialManager::UploadBase( const FMaterialDescriptor& desc ) {

		FMaterialBase base;

		if (desc.mAlbedoTex) {
			base.mAlbedoTex = mTextureMgr->Load(desc.mAlbedoTex.value(), TexturePreset::Albedo);
		}
		else base.mAlbedoTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);

		if (desc.mNormalTex) {
			base.mNormalTex = mTextureMgr->Load(desc.mNormalTex.value(), TexturePreset::Normal);
		}
		else base.mNormalTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultNormal);

		if (desc.mRoughnessTex) {
			base.mRoughnessTex = mTextureMgr->Load(desc.mRoughnessTex.value(), TexturePreset::Roughness);
		}
		else base.mRoughnessTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);

		if (desc.mMetallicTex) {
			base.mMetallicTex = mTextureMgr->Load(desc.mMetallicTex.value(), TexturePreset::Metallic);
		}
		else base.mMetallicTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);

		if (desc.mBaseColor) {
			base.mBaseColor = desc.mBaseColor.value();
		}
		if (desc.mRoughnessValue) {
			base.mRoughnessValue = desc.mRoughnessValue.value();
		}
		if (desc.mMetallicValue) {
			base.mMetallicValue = desc.mMetallicValue.value();
		}

		return mBaseMaterials.Append(base);

	}


	FMaterialInstance MMaterialManager::CreateInstance( MaterialBaseHandle base ) {

		if (!mBaseMaterials.Contains(base))
			base = mDefaultMaterial;

		FMaterialInstance instance;
		FMaterialBase& matBase = mBaseMaterials[base];

		instance = static_cast<FMaterialInstance>(matBase);

		return instance;

	}

	FMaterialInstance MMaterialManager::GetDefaultInstance( ) {
		return CreateInstance(mDefaultMaterial);
	}

	void MMaterialManager::SetBaseMap( MaterialBaseHandle material, EMaterialMember mem, rhi::RTextureHandle tex ) {
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
		create_defaults();
	}

	rhi::RTextureHandle MMaterialManager::validate_texture(rhi::RTextureHandle tex) {

		if (!mRenderDevice->IsValid(tex)) {
			LUM_LOG_DEBUG("No texture given, setting default fallback");
			return mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);
		}
		else
			return tex;

	}

	void MMaterialManager::create_defaults() {

		{ // Base material
			FMaterialBase base;
			base.mAlbedoTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);
			base.mNormalTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultNormal);
			base.mMetallicTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);
			base.mRoughnessTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);
			base.mAmbientTex = mTextureMgr->GetFallbackTexture(FallbackTexture::DefaultAlbedo);

			mDefaultMaterial = UploadBase(base);

		}

	}

} // namespace lum