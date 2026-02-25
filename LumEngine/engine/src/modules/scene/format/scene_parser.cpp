#pragma once

#include "scene/format/scene_parser.hpp"
#include "scene/format/tokenizer.hpp"
#include "scene/format/material_parser.hpp"

#include "scene/scene_manager.hpp"
#include "scene/scene_loader.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/camera.hpp"
#include "entity/components/render.hpp"
#include "entity/components/material.hpp"

#include "render/texture_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/renderer.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneParser::Parse(Scene& scene) {

		FParseContext ctx{ scene };
		ctx.mContext = mContext;

		auto tokens = mTokenizer.GetTokens();

		for (int32 i = 0; i < tokens.size(); i++) {

			if (tokens[i].mType == ETokenType::Identifier) {
				auto it = sIdentifiersParseFunctions.find(HashStr(tokens[i].mValue));
				if (it != sIdentifiersParseFunctions.end()) {
					it->second(tokens, i, ctx);
				}
			}
		}

	}

	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void SceneParser::parse_world(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::expect_opening_bracket(tokens, i);

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Component) {
				
				detail::expect_opening_bracket(tokens, i);

				if (tokens[i].mType == ETokenType::Parameter) {

					if (tokens[i].mValue == "path") {
						
						detail::expect_colon(tokens, i);
						ctx.mContext.mRenderer->SetEnvionmentTexture(ctx.mContext.mTextureMgr->LoadEquirectangularCubemap(tokens[i].mValue.c_str(), 1024));

					}
					else LUM_LOG_ERROR("Invalid parameter");

				}
				

			}

			i++;
		}

	}

	void SceneParser::parse_entity(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		Entity e;
		ctx.mScene.mEntities.push_back(e.mID);
		ctx.mEntity = e.mID;

		detail::expect_opening_bracket(tokens, i);

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Component) {
				auto it = sComponentsParseFunctions.find(HashStr(tokens[i].mValue));
				if (it != sComponentsParseFunctions.end()) {
					it->second(tokens, i, ctx);
				}

			}
			i++;
		}

	}

	void SceneParser::parse_transform(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::expect_opening_bracket(tokens, i);

		CTransform transform;

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {
				if (tokens[i].mValue == "position") {

					transform.mPosition = detail::read_vec3_parameter(tokens, i);

				}

				else if (tokens[i].mValue == "rotation") {

					transform.mRotation = detail::read_vec3_parameter(tokens, i);

				}

				else if (tokens[i].mValue == "scale") {

					transform.mScale = detail::read_vec3_parameter(tokens, i);

				}
			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, transform);

	}
	void SceneParser::parse_smesh(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::expect_opening_bracket(tokens, i);

		CStaticMesh mesh;

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (tokens[i].mValue == "path") {

					detail::expect_colon(tokens, i);
					mesh.mMesh = ctx.mContext.mMeshMgr->CreateStatic(tokens[i].mValue.c_str());

				}
				else LUM_LOG_ERROR("Invalid mesh parameter");

			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, mesh);

	}
	void SceneParser::parse_camera(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::expect_opening_bracket(tokens, i);

		CCamera camera;

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (tokens[i].mValue == "fov") {

					camera.mFov = detail::read_float_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "near") {

					camera.mNear = detail::read_float_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "far") {

					camera.mFar = detail::read_float_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "target") {

					camera.mTarget = detail::read_vec3_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "up") {

					camera.mUp = detail::read_vec3_parameter(tokens, i);

				}

			}

			i++;

		}
		
		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, camera);
		
	}

	void SceneParser::parse_render(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::expect_opening_bracket(tokens, i);

		CRender render;

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (tokens[i].mValue == "visible") {

					render.bVisible = detail::read_bool_parameter(tokens, i);

				}

			}

			i++;

		}
		
		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, render);

	}
	void SceneParser::parse_material(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::expect_opening_bracket(tokens, i);
		CMaterial material;

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (tokens[i].mValue == "path") {

					detail::expect_colon(tokens, i);

					std::optional<String> content = AssetLoader::ReadFile(ERootID::External, tokens[i].mValue);

					if (!content) {
						LUM_LOG_ERROR("Failed to load material %s: %s", tokens[i].mValue.c_str(), AssetLoader::GetErrorMessage());
						material.mMat = ctx.mContext.mMaterialMgr->GetDefaultInstance();
						ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, material);
						i++;
						continue;
					}

					
				  
					Tokenizer tokenizer;
					tokenizer.Tokenize(content.value());
					MaterialParser parser(tokenizer);

					MaterialData data;
					parser.Parse(data);

					auto& instance = material.mMat;

					if (data.mAlbedoTex) {
						instance.mAlbedoTex = ctx.mContext.mTextureMgr->Load(data.mAlbedoTex.value(), ETexturePreset::Albedo);
					}
					if (data.mNormalTex) {
						instance.mNormalTex = ctx.mContext.mTextureMgr->Load(data.mNormalTex.value(), ETexturePreset::Normal);
					}
					if (data.mRoughnessTex) {
						instance.mRoughnessTex = ctx.mContext.mTextureMgr->Load(data.mRoughnessTex.value(), ETexturePreset::Roughness);
					}
					if (data.mMetallicTex) {
						instance.mMetallicTex = ctx.mContext.mTextureMgr->Load(data.mMetallicTex.value(), ETexturePreset::Metallic);
					}
					if (data.mBaseColor) {
						instance.mBaseColor = data.mBaseColor.value();
					}
					if (data.mRoughnessValue) {
						instance.mRoughnessValue = data.mRoughnessValue.value();
					}
					if (data.mMetallicValue) {
						instance.mMetallicValue = data.mMetallicValue.value();
					}



				}

			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, material);


	}
	void SceneParser::parse_name(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

	}


} // namespace lum::fmt