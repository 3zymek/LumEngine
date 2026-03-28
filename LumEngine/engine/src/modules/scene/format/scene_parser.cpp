#pragma once

#include "scene/format/scene_parser.hpp"
#include "scene/format/tokenizer.hpp"
#include "scene/format/material_parser.hpp"
#include "scene/scene_manager.hpp"

#include "audio/audio_manager.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/camera.hpp"
#include "entity/components/render.hpp"
#include "entity/components/material.hpp"
#include "entity/components/name.hpp"
#include "entity/components/light.hpp"
#include "entity/components/audio_emitter.hpp"

#include "render/texture_manager.hpp"
#include "render/material_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/renderer.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneParser::Parse( FScene& scene ) {

		FParseContext ctx{ scene };
		ctx.mContext = mContext;

		auto tokens = mTokenizer.GetTokens( );

		for (int32 i = 0; i < tokens.size( ); i++) {
			if (tokens[ i ].mType == TokenType::Identifier) {
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

	void SceneParser::parse_world( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {

				detail::ExpectOpeningBracket( tokens, i );

				while (in_block( tokens, i )) {

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



	void SceneParser::parse_entity( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		Entity e;
		ctx.mScene.mEntities.push_back( e.mID );
		ctx.mEntity = e.mID;

		detail::ExpectOpeningBracket( tokens, i );

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Component) {
				auto it = sComponentsParseFunctions.find( HashStr( ToLower( tokens[ i ].mValue ) ) );
				if (it != sComponentsParseFunctions.end( )) {
					it->second( tokens, i, ctx );
				}

			}

			i++;

		}

	}
	void SceneParser::parse_transform( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );

		CTransform transform;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {
				if (detail::IsString( tokens, i, "position" )) {

					transform.mPosition = detail::ReadVec3Parameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "rotation" )) {

					transform.mRotation = detail::ReadVec3Parameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "scale" )) {

					transform.mScale = detail::ReadVec3Parameter( tokens, i );

				}
				else LUM_LOG_ERROR( "Invalid parameter" );
			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, transform );

	}
	void SceneParser::parse_smesh( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );

		CStaticMesh mesh;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "path" )) {

					detail::ExpectColon( tokens, i );
					mesh.mMesh = ctx.mContext.mMeshMgr->CreateStatic( tokens[ i ].mValue.c_str( ) );

				}
				else LUM_LOG_ERROR( "Invalid parameter" );

			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, mesh );

	}
	void SceneParser::parse_camera( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );

		CCamera camera;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "fov" )) {

					camera.mFov = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "near" )) {

					camera.mNear = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "far" )) {

					camera.mFar = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "target" )) {

					camera.mTarget = detail::ReadVec3Parameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "up" )) {

					camera.mUp = detail::ReadVec3Parameter( tokens, i );

				}
				else LUM_LOG_ERROR( "Invalid parameter" );

			}

			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, camera );

	}
	void SceneParser::parse_render( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );

		CRender render;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "visible" )) {

					render.bVisible = detail::ReadBoolParameter( tokens, i );

				}
				else LUM_LOG_ERROR( "Invalid parameter" );

			}

			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, render );

	}
	void SceneParser::parse_material( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );
		CMaterial material;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "path" )) {

					std::optional<String> content = AssetLoader::ReadFile( RootID::External, detail::ReadStringParameter( tokens, i ) );

					if (!content) {
						LUM_LOG_ERROR( "Failed to load material %s: %s", tokens[ i ].mValue.c_str( ), AssetLoader::GetErrorMessage( ) );
						material.mMat = ctx.mContext.mMaterialMgr->GetDefaultInstance( );
						ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, material );
						i++;
						continue;
					}

					Tokenizer tokenizer;
					tokenizer.Tokenize( content.value( ) );
					MaterialParser parser( tokenizer );

					FMaterialDescriptor data;
					parser.Parse( data );

					auto& instance = material.mMat;

					MaterialBaseHandle handle = ctx.mContext.mMaterialMgr->UploadBase( data );

					material.mMat = ctx.mContext.mMaterialMgr->CreateInstance( handle );

				}
				else LUM_LOG_ERROR( "Invalid parameter" );

			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, material );


	}
	void SceneParser::parse_name( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );
		CName name;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "name" )) {

					name.mName = detail::ReadStringParameter( tokens, i ).c_str( );

				}
				else LUM_LOG_ERROR( "Invalid parameter" );

			}

			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, name );

	}
	void SceneParser::parse_point_light( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );
		CPointLight light;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "intensity" )) {

					light.mIntensity = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "radius" )) {

					light.mRadius = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "color" )) {

					light.mColor = detail::ReadVec3Parameter( tokens, i );

				}

			}


			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, light );

	}

	void SceneParser::parse_spot_light( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );
		CSpotLight light;

		while (in_block( tokens, i )) {

			if (tokens[ i ].mType == TokenType::Parameter) {

				if (detail::IsString( tokens, i, "intensity" )) {

					light.mIntensity = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "radius" )) {

					light.mRadius = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "color" )) {

					light.mColor = detail::ReadVec3Parameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "direction" )) {

					light.mDirection = detail::ReadVec3Parameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "inner_cone" )) {

					light.mInnerConeAngle = detail::ReadFloatParameter( tokens, i );

				}
				else if (detail::IsString( tokens, i, "outer_cone" )) {

					light.mOuterConeAngle = detail::ReadFloatParameter( tokens, i );

				}

			}

			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, light );

	}

	void SceneParser::parse_audio_emitter( std::vector<FToken>& tokens, int32& i, FParseContext& ctx ) {

		detail::ExpectOpeningBracket( tokens, i );
		CAudioEmitter emitter;
		String path;
		String category;

		while (in_block( tokens, i )) {

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
		ctx.mContext.mAudioMgr->PlayOneShot( path, {} );
		ctx.mScene.mEntityMgr.AddComponent( ctx.mEntity, emitter );

	}

} // namespace lum::fmt