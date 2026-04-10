//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Manages shared material bases and per-entity instances.
//
//=============================================================================//

#include "render/material_manager.hpp"
#include "render/render_common.hpp"
#include "rhi/core/rhi_device.hpp"
#include "render/texture_manager.hpp"
#include "event/event_bus.hpp"
#include "event/events/entity_events.hpp"
#include "scene/format/material_parser.hpp"
#include "scene/format/tokenizer.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MMaterialManager::Initialize( render::FRendererContext* ctx ) {

		mContext = ctx;

		init( );

	}

	MaterialBaseHandle MMaterialManager::UploadBase( const FMaterialBase& base ) {

		return mBaseMaterials.Append( base );

	}

	MaterialBaseHandle MMaterialManager::UploadBase( const FMaterialDescriptor& desc ) {

		FMaterialBase base;

		if (desc.mAlbedoTex) {
			base.mAlbedoTex = mContext->mTextureMgr->Load( desc.mAlbedoTex.value( ), TexturePreset::Albedo );
		}
		else base.mAlbedoTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );

		if (desc.mNormalTex) {
			base.mNormalTex = mContext->mTextureMgr->Load( desc.mNormalTex.value( ), TexturePreset::Normal );
		}
		else base.mNormalTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultNormal );

		if (desc.mRoughnessTex) {
			base.mRoughnessTex = mContext->mTextureMgr->Load( desc.mRoughnessTex.value( ), TexturePreset::Roughness );
		}
		else base.mRoughnessTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultRoughness );

		if (desc.mMetallicTex) {
			base.mMetallicTex = mContext->mTextureMgr->Load( desc.mMetallicTex.value( ), TexturePreset::Metallic );
		}
		else base.mMetallicTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultMetallic );

		if (desc.mBaseColor) {
			base.mBaseColor = desc.mBaseColor.value( );
		}
		if (desc.mRoughnessValue) {
			base.mRoughnessValue = desc.mRoughnessValue.value( );
		}
		if (desc.mMetallicValue) {
			base.mMetallicValue = desc.mMetallicValue.value( );
		}

		return mBaseMaterials.Append( base );

	}


	CMaterialInstance MMaterialManager::CreateInstance( MaterialBaseHandle base ) {

		if (!mBaseMaterials.Contains( base ))
			base = mDefaultMaterial;

		CMaterialInstance instance;
		FMaterialBase& matBase = mBaseMaterials[ base ];

		instance.mAlbedoTex = matBase.mAlbedoTex;
		instance.mAmbientTex = matBase.mAmbientTex;
		instance.mBaseColor = matBase.mBaseColor;
		instance.mMetallicTex = matBase.mMetallicTex;
		instance.mMetallicValue = matBase.mMetallicValue;
		instance.mNormalTex = matBase.mNormalTex;
		instance.mRoughnessTex = matBase.mRoughnessTex;
		instance.mRoughnessValue = matBase.mRoughnessValue;

		return instance;

	}

	CMaterialInstance MMaterialManager::GetDefaultInstance( ) {
		return CreateInstance( mDefaultMaterial );
	}

	void MMaterialManager::SetBaseMap( MaterialBaseHandle material, MaterialMember mem, rhi::RTextureHandle tex ) {
		switch (mem) {
		case MaterialMember::Albedo: mBaseMaterials[ material ].mAlbedoTex = tex; break;
		case MaterialMember::Normal: mBaseMaterials[ material ].mNormalTex = tex; break;
		case MaterialMember::Metallic: mBaseMaterials[ material ].mMetallicTex = tex; break;
		case MaterialMember::Roughness: mBaseMaterials[ material ].mRoughnessTex = tex; break;
		}
	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void MMaterialManager::init( ) {

		mContext->mEvBus->SubscribePermanently<EComponentAdded<CMaterialInstance>>(
			[&]( const EComponentAdded<CMaterialInstance>& mat ) {

				std::optional<String> content = AssetLoader::ReadFile( RootID::External, mat.mComponent->mBasePath );
				if (!content) {
					LUM_LOG_ERROR( "Failed to load material %s: %s", mat.mComponent->mBasePath.data(), AssetLoader::GetErrorMessage( ) );
					*mat.mComponent = GetDefaultInstance();
					return;
				}

				fmt::Tokenizer tokenizer;
				tokenizer.Tokenize( content.value( ) );
				fmt::MaterialParser parser( tokenizer );
				FMaterialDescriptor data;
				parser.Parse( data );
				MaterialBaseHandle baseHandle = UploadBase( data );
				*mat.mComponent = CreateInstance( baseHandle );

			}
		);

		create_defaults( );
	
	}

	rhi::RTextureHandle MMaterialManager::validate_texture( rhi::RTextureHandle tex ) {

		if (!mContext->mRenderDev->IsValid( tex )) {
			LUM_LOG_DEBUG( "No texture given, setting default fallback" );
			return mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );
		}
		else
			return tex;

	}

	void MMaterialManager::create_defaults( ) {

		{ // Base material
			FMaterialBase base;
			base.mAlbedoTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );
			base.mNormalTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultNormal );
			base.mMetallicTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultMetallic );
			base.mRoughnessTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultRoughness );
			base.mAmbientTex = mContext->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );

			mDefaultMaterial = UploadBase( base );

		}

	}

} // namespace lum