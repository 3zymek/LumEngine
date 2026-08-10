//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages shared material bases and per-entity instances.
//
//=============================================================================//

#include "Render/MaterialManager.hpp"
#include "Render/RenderCommon.hpp"
#include "Rhi/Core/RhiDevice.hpp"
#include "Render/TextureManager.hpp"
#include "Event/EventBus.hpp"
#include "Event/Events/EntityEvents.hpp"
#include "Scene/Format/MaterialDeserializer.hpp"
#include "Scene/Format/Tokenizer.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MaterialManager::Initialize( render::RendererContext* ctx ) {

		mCtx = ctx;
		init( );

	}

	MaterialBaseHandle MaterialManager::UploadBase( const MaterialDescriptor& desc ) {

		MaterialBase base{};

		if (desc.mAlbedoTex) {
			base.mAlbedoTex = mCtx->mTextureMgr->Load( desc.mAlbedoTex.Value( ), TexturePreset::Albedo );
		}
		else base.mAlbedoTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );

		if (desc.mNormalTex) {
			base.mNormalTex = mCtx->mTextureMgr->Load( desc.mNormalTex.Value( ), TexturePreset::Normal );
		}
		else base.mNormalTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultNormal );

		if (desc.mRoughnessTex) {
			base.mRoughnessTex = mCtx->mTextureMgr->Load( desc.mRoughnessTex.Value( ), TexturePreset::Roughness );
		}
		else base.mRoughnessTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultRoughness );

		if (desc.mMetallicTex) {
			base.mMetallicTex = mCtx->mTextureMgr->Load( desc.mMetallicTex.Value( ), TexturePreset::Metallic );
		}
		else base.mMetallicTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultMetallic );

		return mBaseMaterials.Append( base );

	}


	void MaterialManager::ApplyInstance( CMaterialInstance& instance, MaterialBaseHandle base ) {

		if (!mBaseMaterials.Contains( base ))
			base = mDefaultMaterial;

		MaterialBase& matBase = mBaseMaterials[ base ];

		instance.mAlbedoTex = matBase.mAlbedoTex;
		instance.mAmbientTex = matBase.mAmbientTex;
		instance.mMetallicTex = matBase.mMetallicTex;
		instance.mNormalTex = matBase.mNormalTex;
		instance.mRoughnessTex = matBase.mRoughnessTex;

	}

	CMaterialInstance MaterialManager::GetDefaultInstance( ) {
		static CMaterialInstance inst{};
		ApplyInstance( inst, mDefaultMaterial );
		return inst;
	}

	void MaterialManager::SetBaseMap( MaterialBaseHandle material, MaterialMember mem, rhi::TextureHandle tex ) {
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

	void MaterialManager::init( ) {

		mCtx->mEvBus->SubscribePermanently<EComponentAdded<CMaterialInstance>>(
			[&]( const EComponentAdded<CMaterialInstance>& mat ) {

				auto content = FileSystem::ReadAllText( 
					ResourceLoader::ResolveResourcePath( ResourceRoot::External, mat.mComponent->mBasePath )
				);
				if (!content) {
					LUM_LOG_ERROR( "Failed to load material %s: %s", mat.mComponent->mBasePath.data(), content.GetError() );
					*mat.mComponent = GetDefaultInstance();
					return;
				}

				fmt::Tokenizer tokenizer;
				tokenizer.Tokenize( content.ValueRef( ) );

				MaterialDescriptor data{};
				fmt::MaterialDeserializer::Deserialize( data, tokenizer );
				
				MaterialBaseHandle baseHandle = UploadBase( data );
				
				String basePath = mat.mComponent->mBasePath;
				ApplyInstance( *mat.mComponent, baseHandle );
				mat.mComponent->mBasePath = basePath;

			}
		);

		create_defaults( );
	
	}

	rhi::TextureHandle MaterialManager::validate_texture( rhi::TextureHandle tex ) {

		if (!mCtx->mRenderDev->IsValid( tex )) {
			LUM_LOG_DEBUG( "No texture given, setting default fallback" );
			return mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );
		}
		else
			return tex;

	}

	void MaterialManager::create_defaults( ) {

		{ // Base material
			MaterialBase base;
			base.mAlbedoTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );
			base.mNormalTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultNormal );
			base.mMetallicTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultMetallic );
			base.mRoughnessTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultRoughness );
			base.mAmbientTex = mCtx->mTextureMgr->GetFallbackTexture( FallbackTexture::DefaultAlbedo );

			mDefaultMaterial = UploadBase( base );
		}

	}

} // namespace lum