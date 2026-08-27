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

//=======================================================//
	// Public
//=======================================================//

	void MaterialManager::Initialize( render::RendererContext& ctx ) {

		ctx.Validate( );

		m_Ctx = ctx;
		init( );

	}

	MaterialBaseHandle MaterialManager::UploadBase( const MaterialDescriptor& desc ) {

		MaterialBase base{};

		auto& texMgr = m_Ctx( ).m_TextureMgr( );

		if (desc.m_AlbedoTex) {
			base.m_AlbedoTex = texMgr.Load( desc.m_AlbedoTex.Value( ), TexturePreset::Albedo );
		}
		else base.m_AlbedoTex = texMgr.GetFallbackTexture( FallbackTexture::DefaultAlbedo );

		if (desc.m_NormalTex) {
			base.m_NormalTex = texMgr.Load( desc.m_NormalTex.Value( ), TexturePreset::Normal );
		}
		else base.m_NormalTex = texMgr.GetFallbackTexture( FallbackTexture::DefaultNormal );

		if (desc.m_RoughnessTex) {
			base.m_RoughnessTex = texMgr.Load( desc.m_RoughnessTex.Value( ), TexturePreset::Roughness );
		}
		else base.m_RoughnessTex = texMgr.GetFallbackTexture( FallbackTexture::DefaultRoughness );

		if (desc.m_MetallicTex) {
			base.m_MetallicTex = texMgr.Load( desc.m_MetallicTex.Value( ), TexturePreset::Metallic );
		}
		else base.m_MetallicTex = texMgr.GetFallbackTexture( FallbackTexture::DefaultMetallic );

		return m_BaseMaterials.Append( base );

	}


	void MaterialManager::ApplyInstance( CMaterialInstance& instance, MaterialBaseHandle base ) {

		if (!m_BaseMaterials.Contains( base ))
			base = m_DefaultMaterial;

		MaterialBase& matBase = m_BaseMaterials[ base ];

		instance.m_AlbedoTex = matBase.m_AlbedoTex;
		instance.m_AmbientTex = matBase.m_AmbientTex;
		instance.m_MetallicTex = matBase.m_MetallicTex;
		instance.m_NormalTex = matBase.m_NormalTex;
		instance.m_RoughnessTex = matBase.m_RoughnessTex;

	}

	CMaterialInstance MaterialManager::GetDefaultInstance( ) {
		static CMaterialInstance inst{};
		ApplyInstance( inst, m_DefaultMaterial );
		return inst;
	}

	void MaterialManager::SetBaseMap( MaterialBaseHandle material, MaterialMember mem, rhi::TextureHandle tex ) {
		switch (mem) {
		case MaterialMember::Albedo: m_BaseMaterials[ material ].m_AlbedoTex = tex; break;
		case MaterialMember::Normal: m_BaseMaterials[ material ].m_NormalTex = tex; break;
		case MaterialMember::Metallic: m_BaseMaterials[ material ].m_MetallicTex = tex; break;
		case MaterialMember::Roughness: m_BaseMaterials[ material ].m_RoughnessTex = tex; break;
		}
	}




//=======================================================//
	// Private
//=======================================================//

	void MaterialManager::init( ) {

		m_Ctx( ).m_EventBus().SubscribePermanently<EComponentAdded<CMaterialInstance>>(
			[&]( const EComponentAdded<CMaterialInstance>& mat ) {

				auto content = FileSystem::ReadAllText( 
					ResourceLoader::ResolveResourcePath( ResourceRoot::External, mat.m_Component->m_BasePath )
				);
				if (!content) {
					LUM_LOG_INFO( "{}", mat.m_Component->m_BasePath );
					LUM_LOG_ERROR( "Failed to load material {}: {}", mat.m_Component->m_BasePath, content.GetError() );
					*mat.m_Component = GetDefaultInstance();
					return;
				}

				fmt::Tokenizer tokenizer;
				tokenizer.Tokenize( content.ValueRef( ), mat.m_Component->m_BasePath );

				MaterialDescriptor data{};
				fmt::MaterialDeserializer::Deserialize( data, tokenizer );
				
				MaterialBaseHandle baseHandle = UploadBase( data );
				
				String basePath = mat.m_Component->m_BasePath;
				ApplyInstance( *mat.m_Component, baseHandle );
				mat.m_Component->m_BasePath = basePath;

			}
		);

		create_defaults( );
	
	}

	rhi::TextureHandle MaterialManager::validate_texture( rhi::TextureHandle tex ) {

		if (!m_Ctx( ).m_RenderDev().IsValid( tex )) {
			LUM_LOG_DEBUG( "No texture given, setting default fallback" );
			return m_Ctx( ).m_TextureMgr().GetFallbackTexture( FallbackTexture::DefaultAlbedo );
		}
		else
			return tex;

	}

	void MaterialManager::create_defaults( ) {

		{ // Base material

			auto& texMgr = m_Ctx( ).m_TextureMgr( );
			
			MaterialBase base{};
			base.m_AlbedoTex	= texMgr.GetFallbackTexture( FallbackTexture::DefaultAlbedo );
			base.m_NormalTex	= texMgr.GetFallbackTexture( FallbackTexture::DefaultNormal );
			base.m_MetallicTex	= texMgr.GetFallbackTexture( FallbackTexture::DefaultMetallic );
			base.m_RoughnessTex	= texMgr.GetFallbackTexture( FallbackTexture::DefaultRoughness );
			base.m_AmbientTex	= texMgr.GetFallbackTexture( FallbackTexture::DefaultAlbedo );

			m_DefaultMaterial = UploadBase( base );

		}

	}

} // namespace lum