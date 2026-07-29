//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once

#include "Scene/Format/SceneDepManager.hpp"
#include "Scene/Format/Tokenizer.hpp"
#include "Scene/SceneManager.hpp"

#include "Audio/AudioManager.hpp"
#include "Entity/Components/AudioEmitter.hpp"

#include "Render/TextureManager.hpp"
#include "Render/Renderer.hpp"

#include "Scene/Format/DeserializeFunctions.gen.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneDependencyManager::Initialize( Tokenizer& tokenizer, SceneManagerContext& ctx ) {

		mTokenizer = &tokenizer;
		mCtx = &ctx;

		RegisterSceneComponents( sComponentsInfos );

	}


	void SceneDependencyManager::Deserialize( Scene& scene ) {

		ParseContext ctx{ scene };
		ctx.mCtx = *mCtx;

		auto tokens = mTokenizer->GetTokens( );

		for (int32 i = 0; i < tokens.size( ); i++) {
			if (tokens[ i ].mType == TokenType::Identifier) {
				auto it = sIdentifiersParseFunctions.find( HashString( ToLower( tokens[ i ].mValue ) ) );
				if (it != sIdentifiersParseFunctions.end( )) {
					it->second( tokens, i, ctx );
				}
			}
		}

	}

	void SceneDependencyManager::Serialize( Scene& scene, StringView path ) {

		ParseContext ctx{ scene };
		ctx.mCtx = *mCtx;

		for (auto& [entityID, entity] : scene.mEntities) {

		}

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void SceneDependencyManager::parse_world( std::vector<Token>& tokens, int32& i, ParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {

				detail::ExpectOpeningBracket( tokens, i );

				while (detail::InBlock( tokens, i )) {

					if (tokens[ i ].mType == TokenType::Parameter) {

						if (detail::IsString( tokens, i, "path" )) {
							detail::ExpectColon( tokens, i );
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



	void SceneDependencyManager::parse_entity( std::vector<Token>& tokens, int32& i, ParseContext& ctx ) {

		Entity entity = ctx.mScene.CreateEntity( );
		EntityID id = entity.GetID( );
		ctx.mEntity = id;

		detail::ExpectOpeningBracket( tokens, i );

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {
				auto it = sComponentsInfos.find( HashString( ToLower( tokens[ i ].mValue ) ) );
				if (it != sComponentsInfos.end( )) {
					it->second.mDeserializeFn( tokens, i, ctx );
				}

			}

			i++;

		}

	}

} // namespace lum::fmt