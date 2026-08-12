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

		DeserializeContext ctx{
			.mScene = scene,
			.mCtx = *this->mCtx
		};

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

		for (auto& [entityId, children] : ctx.mPersistentChildren) {

			for (EntityID child : children) {

				scene.AttachChild(
					entityId,
					ctx.mPersistentToEntity[ child ]
				);

			}

		}

	}

	void SceneDependencyManager::Serialize( Scene& scene ) {

		StringBuilder sb{};

		for (auto& [entityId, entity] : scene.mEntities) {

			sb.Append( "entity " );
			sb.Append( entityId );
			sb.AppendLine( " {" );

			write_children( sb, scene, entityId );

			scene.mEntityMgr.ForEachComponent(
				entityId,
				[ & ]( ComponentBase* comp, ecs::ComponentBasePool* pool ) {
					auto it = sTypeIdInfoLookup.find( pool->GetTypeId( ) );
					if (it != sTypeIdInfoLookup.end( ) && (it->second && it->second->mSerializeFn)) {
						it->second->mSerializeFn( sb, comp );
					}
				} );

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
		ctx.mPersistentToEntity.emplace( persistentId, id );

		detail::ExpectOpeningBracketNext( tokens, i );

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {

				HashedString hashed = HashString( ToLower( tokens[ i ].mValue ) );

				auto it = sNameInfoLookup.find( hashed );

				if (it != sNameInfoLookup.end( )) {
					it->second->mDeserializeFn( tokens, i, ctx );
				}
				else {
					auto special = sEntityProperyHandlers.find( hashed );

					if (special != sEntityProperyHandlers.end( )) {
						special->second( tokens, i, ctx );
					}
					else {
						throw DeserializeException(
							"Unknown property at line %llu in file %s",
							tokens[ i ].mLine,
							tokens[ i ].mFilePath.ToString( ).c_str( )
						);
					}
				}
			}

			++i;

		}

	}

	void SceneDependencyManager::read_children( std::vector<Token>& tokens, int32& i, DeserializeContext& ctx ) {

		detail::ExceptColonNext( tokens, i );
		detail::ExceptOpeningSquareBracketInPlace( tokens, i );
		i++;

		std::vector<uint64> children{};

		while (detail::InSquareBlock( tokens, i )) {

			children.push_back( detail::ReadInt( tokens, i ) );
			++i;

		}

		ctx.mPersistentChildren.emplace(
			ctx.mCurrentEntity,
			std::move( children )
		);

	}

	void SceneDependencyManager::write_children( StringBuilder& sb, Scene& scene, EntityID entity ) {

		auto it = scene.mChildren.find( entity );
		if (it == scene.mChildren.end( )) 
			return;

		sb.Append( "\t@children: [ " );
		
		for (int32 i = 0; i < it->second.size( ); i++) {

			sb.Append( it->second[ i ] );

			if (i + 1 < it->second.size( ))
				sb.Append( ", " );

		}

		sb.AppendLine( " ]" );

	}

	void SceneDependencyManager::categorize_component_infos( ) {

		for (auto& info : mComponentsInfos) {
			sNameInfoLookup.emplace( HashString( info.mSerializationName ), &info );
			sTypeIdInfoLookup.emplace( info.mTypeId, &info );
		}

	}

} // namespace lum::fmt