#pragma once

#include "scene/format/scene_dependency_manager.hpp"
#include "scene/format/scene_dependency_manager.generated.hpp"
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

	void SceneDependencyManager::Initialize( Tokenizer& tokenizer, FSceneManagerContext& ctx ) {

		mTokenizer = &tokenizer;
		mContext = &ctx;

		detail::RegisterComponents( sComponentsParseFunctions );

	}


	void SceneDependencyManager::Parse( FScene& scene ) {

		FParseContext ctx{ scene };
		ctx.mContext = *mContext;

		auto tokens = mTokenizer->GetTokens( );

		for (int32 i = 0; i < tokens.size( ); i++) {
			if (tokens[ i ].mType == TokenType::Identifier) {
				LUM_LOG_INFO( "Parsing component: %s", tokens[i ].mValue.c_str() );
				auto it = sIdentifiersParseFunctions.find( HashStr( ToLower( tokens[ i ].mValue ) ) );
				if (it != sIdentifiersParseFunctions.end( )) {
					it->second( tokens, i, ctx );
				}
			}
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

		Entity e;
		ctx.mScene.mEntities.push_back( e.mID );
		ctx.mEntity = e.mID;

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

	void SceneDependencyManager::parse_audio_emitter( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );
		CAudioEmitter emitter;
		String path;
		String category;

		while (detail::InBlock( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "volume" )) {

					emitter.mVolume = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "pitch" )) {

					emitter.mPitch = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "min_distance" )) {

					emitter.mMinDistance = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "max_distance" )) {

					emitter.mMaxDistance = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "paused" )) {

					emitter.bPaused = detail::ReadBoolParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "looped" ) || detail::IsString( tokens, i, "loop" )) {

					emitter.bLooped = detail::ReadBoolParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "sound" )) {

					path = detail::ReadStringParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "category" )) {

					category = detail::ReadStringParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "group" )) {

					emitter.mGroup = ctx.mContext.mAudioMgr->GetGroup( detail::ReadStringParameter( tokens, i ) );

				}

			}

			i++;

		}

		emitter.mSound = ctx.mContext.mAudioMgr->GetSound( path, category );
		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, emitter );

	}

} // namespace lum::fmt