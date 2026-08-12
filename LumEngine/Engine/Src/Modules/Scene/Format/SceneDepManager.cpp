//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once

#include "Scene/Format/SceneDepManager.hpp"
#include "Scene/Format/Tokenizer.hpp"
#include "Scene/SceneManager.hpp"

#include "Render/TextureManager.hpp"
#include "Render/Renderer.hpp"

#include "Core/Utils/StringBuilder.hpp"

#include "Scene/Format/DeserializeFunctions.gen.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneDependencyManager::Initialize( SceneManagerContext& ctx ) {
		mCtx = &ctx;

		RegisterSceneComponents( mComponentsInfos );
		categorize_component_infos( );

	}


	void SceneDependencyManager::Deserialize( Scene& scene, Tokenizer& tokenizer ) {

		DeserializeContext ctx{ scene };
		ctx.mCtx = *mCtx;

		auto tokens = tokenizer.GetTokens( );

		try {
			for (int32 i = 0; i < tokens.size( ); i++) {
				if (tokens[ i ].mType == TokenType::Identifier) {
					auto it = sIdentifiersDeserializeFunctions.find( HashString( ToLower( tokens[ i ].mValue ) ) );
					if (it != sIdentifiersDeserializeFunctions.end( )) {
						it->second( tokens, i, ctx );
					}
				}
			}
		}
		catch (const fmt::DeserializeException& e) {
			LUM_LOG_ERROR( e.what( ) );
		}

	}

	void SceneDependencyManager::Serialize( Scene& scene ) {

		StringBuilder sb{};

		for (auto& [entityId, entity] : scene.mEntities) {

			sb.Append( "entity " );
			sb.Append( entityId );
			sb.AppendLine( " {" );

			scene.mEntityMgr.ForEachComponent(
				entityId,
				[&]( ComponentBase* comp, ecs::ComponentBasePool* pool ) {
					auto it = mTypeIdInfoLookup.find( pool->GetTypeId( ) );
					if (it != mTypeIdInfoLookup.end( ) && (it->second && it->second->mSerializeFn)) {
						it->second->mSerializeFn( sb, comp );
					}
				});

			sb.AppendLine( "}" );

		}

		FileSystem::WriteAllText( scene.mScenePath, sb.ToString( ) );

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void SceneDependencyManager::deserialize_world( std::vector<Token>& tokens, int32& i, DeserializeContext& ctx ) {

		detail::ExpectOpeningBracketNext( tokens, i );

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {

				detail::ExpectOpeningBracketNext( tokens, i );

				while (detail::InBlock( tokens, i )) {

					if (tokens[ i ].mType == TokenType::Parameter) {

						if (detail::IsString( tokens, i, "path" )) {
							detail::ExceptColonNext( tokens, i );
							ctx.mCtx.mRenderer->SetEnvironmentTexture(
								ctx.mCtx.mTextureMgr->LoadEquirectangularCubemap( tokens[ i ].mValue.c_str( ) )
							);
						}

					}

					i++;

				}
			}

			i++;

		}
	}



	void SceneDependencyManager::deserialize_entity( std::vector<Token>& tokens, int32& i, DeserializeContext& ctx ) {

		Entity entity = ctx.mScene.CreateEntity( );
		EntityID id = entity.GetId( );
		ctx.mCurrentEntity = id;

		++i;
		EntityID persistentId = detail::ReadInt( tokens, i );

		detail::ExpectOpeningBracketNext( tokens, i );

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {
				auto it = mNameInfoLookup.find( HashString( ToLower( tokens[ i ].mValue ) ) );
				if (it != mNameInfoLookup.end( )) {
					it->second->mDeserializeFn( tokens, i, ctx );
				}

			}

			i++;

		}

	}

	void SceneDependencyManager::categorize_component_infos( ) {

		for (auto& info : mComponentsInfos) {
			mNameInfoLookup.emplace( HashString( info.mSerializationName ), &info );
			mTypeIdInfoLookup.emplace( info.mTypeId, &info );
		}

	}

} // namespace lum::fmt