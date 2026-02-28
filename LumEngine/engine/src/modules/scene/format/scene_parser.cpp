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
#include "entity/components/name.hpp"
#include "entity/components/light.hpp"

#include "render/texture_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/renderer.hpp"

namespace lum::fmt {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void SceneParser::Parse(FScene& scene) {

		FParseContext ctx{ scene };
		ctx.mContext = mContext;

		auto tokens = mTokenizer.GetTokens();

		for (int32 i = 0; i < tokens.size(); i++) {
			if (tokens[i].mType == ETokenType::Identifier) {
				auto it = sIdentifiersParseFunctions.find(HashStr(ToLower(tokens[i].mValue)));
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

		detail::ExpectOpeningBracket(tokens, i);

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Component) {

				detail::ExpectOpeningBracket(tokens, i);

				while (in_block(tokens, i)) {

					if (tokens[i].mType == ETokenType::Parameter) {

						if (ToLower(tokens[i].mValue) == "path") {
							detail::ExpectColon(tokens, i);
							ctx.mContext.mRenderer->SetEnvionmentTexture(
								ctx.mContext.mTextureMgr->LoadEquirectangularCubemap(tokens[i].mValue.c_str(), 1024));
						}

					}

					i++;

				}
			}

			i++;

		}
	}



	void SceneParser::parse_entity(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		Entity e;
		ctx.mScene.mEntities.push_back(e.mID);
		ctx.mEntity = e.mID;

		detail::ExpectOpeningBracket(tokens, i);

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Component) {
				auto it = sComponentsParseFunctions.find(HashStr(ToLower(tokens[i].mValue)));
				if (it != sComponentsParseFunctions.end()) {
					it->second(tokens, i, ctx);
				}

			}

			i++;

		}

	}
	void SceneParser::parse_transform(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);

		CTransform transform;

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Parameter) {
				if (ToLower(tokens[i].mValue) == "position") {

					transform.mPosition = detail::ReadVec3Parameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "rotation") {

					transform.mRotation = detail::ReadVec3Parameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "scale") {

					transform.mScale = detail::ReadVec3Parameter(tokens, i);

				}
				else LUM_LOG_ERROR("Invalid parameter");
			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, transform);

	}
	void SceneParser::parse_smesh(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);

		CStaticMesh mesh;

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (ToLower(tokens[i].mValue) == "path") {

					detail::ExpectColon(tokens, i);
					mesh.mMesh = ctx.mContext.mMeshMgr->CreateStatic(tokens[i].mValue.c_str());

				}
				else LUM_LOG_ERROR("Invalid parameter");

			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, mesh);

	}
	void SceneParser::parse_camera(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);

		CCamera camera;

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (ToLower(tokens[i].mValue) == "fov") {

					camera.mFov = detail::ReadFloatParameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "near") {

					camera.mNear = detail::ReadFloatParameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "far") {

					camera.mFar = detail::ReadFloatParameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "target") {

					camera.mTarget = detail::ReadVec3Parameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "up") {

					camera.mUp = detail::ReadVec3Parameter(tokens, i);

				}
				else LUM_LOG_ERROR("Invalid parameter");

			}

			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, camera);

	}
	void SceneParser::parse_render(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);

		CRender render;

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (ToLower(tokens[i].mValue) == "visible") {

					render.bVisible = detail::ReadBoolParameter(tokens, i);

				}
				else LUM_LOG_ERROR("Invalid parameter");

			}

			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, render);

	}
	void SceneParser::parse_material(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);
		CMaterial material;

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (ToLower(tokens[i].mValue) == "path") {

					std::optional<String> content = AssetLoader::ReadFile(ERootID::External, detail::ReadStringParameter(tokens, i));

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

					FMaterialDescriptor data;
					parser.Parse(data);

					auto& instance = material.mMat;

					MaterialBaseHandle handle = ctx.mContext.mMaterialMgr->UploadBase(data);

					material.mMat = ctx.mContext.mMaterialMgr->CreateInstance(handle);

				}
				else LUM_LOG_ERROR("Invalid parameter");

			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, material);


	}
	void SceneParser::parse_name(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);
		CName name;

		while (in_block(tokens, i)) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (ToLower(tokens[i].mValue) == "name") {

					name.mName = detail::ReadStringParameter(tokens, i).c_str();

				}
				else LUM_LOG_ERROR("Invalid parameter");

			}

			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, name);

	}
	void SceneParser::parse_directional_light(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);
		CDirectionalLight light;

		while (in_block(tokens, i)) {
			if (tokens[i].mType == ETokenType::Parameter) {

				if (ToLower(tokens[i].mValue) == "direction") {

					light.mDirection = detail::ReadVec3Parameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "intensity") {

					light.mIntensity = detail::ReadFloatParameter(tokens, i);

				}
				else if (ToLower(tokens[i].mValue) == "color") {

					light.mColor = detail::ReadVec3Parameter(tokens, i);

				}

			}

			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, light);

	}
	void SceneParser::parse_point_light(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		detail::ExpectOpeningBracket(tokens, i);
		CPointLight light;

		while (in_block(tokens, i)) {

			if (tokens[i].mValue == "intensity") {

				light.mIntensity = detail::ReadFloatParameter(tokens, i);

			}
			else if (tokens[i].mValue == "radius") {

				light.mRadius = detail::ReadFloatParameter(tokens, i);

			}
			else if (tokens[i].mValue == "color") {

				light.mColor = detail::ReadVec3Parameter(tokens, i);

			}


			i++;

		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, light);

	}

} // namespace lum::fmt