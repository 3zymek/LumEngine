//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parser for .lsc scene format files.
//
//=============================================================================//
#pragma once

#include "scene/format/scene_dep_manager.hpp"
#include "scene/format/scene_dep_manager.generated.hpp"
#include "scene/format/tokenizer.hpp"
#include "scene/scene_manager.hpp"

#include "audio/audio_manager.hpp"
#include "entity/components/audio_emitter.hpp"

#include "render/texture_manager.hpp"
#include "render/renderer.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneDependencyManager::Initialize( Tokenizer& tokenizer, SceneManagerContext& ctx ) {

		mTokenizer = &tokenizer;
		mContext = &ctx;

		detail::RegisterComponents( sComponentsParseFunctions );

	}


	void SceneDependencyManager::Parse( Scene& scene ) {

		FParseContext ctx{ scene };
		ctx.mContext = *mContext;

		auto tokens = mTokenizer->GetTokens( );

		for (int32 i = 0; i < tokens.size( ); i++) {
			if (tokens[ i ].mType == TokenType::Identifier) {
				auto it = sIdentifiersParseFunctions.find( HashStr( ToLower( tokens[ i ].mValue ) ) );
				if (it != sIdentifiersParseFunctions.end( )) {
					it->second( tokens, i, ctx );
				}
			}
		}

	}

	void SceneDependencyManager::Serialize( Scene& scene, StringView path ) {

		FParseContext ctx{ scene };
		ctx.mContext = *mContext;

		for (auto& [entityID, entity] : scene.mEntities) {
			
		}

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void SceneDependencyManager::parse_world( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {

				detail::ExpectOpeningBracket( tokens, i );

				while (detail::InBlock( tokens, i )) {

					if (tokens[ i ].mType == TokenType::Parameter) {

						if (detail::IsString( tokens, i, "path" )) {
							detail::ExpectColon( tokens, i );
							ctx.mContext.mRenderer->SetEnvironmentTexture(
								ctx.mContext.mTextureMgr->LoadEquirectangularCubemap( tokens[ i ].mValue.c_str( ) )
							);
						}

					}

					i++;

				}
			}

			i++;

		}
	}



	void SceneDependencyManager::parse_entity( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		Entity entity = ctx.mScene.CreateEntity( );
		EntityID id = entity.GetID( );
		ctx.mEntity = id;

		detail::ExpectOpeningBracket( tokens, i );

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {
				auto it = sComponentsParseFunctions.find( HashStr( ToLower( tokens[ i ].mValue ) ) );
				if (it != sComponentsParseFunctions.end( )) {
					it->second( tokens, i, ctx );
				}

			}

			i++;

		}

	}

} // namespace lum::fmt