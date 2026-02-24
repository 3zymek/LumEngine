#pragma once

#include "scene/format/scene_parser.hpp"
#include "scene/scene_manager.hpp"
#include "scene/scene_loader.hpp"
#include "scene/format/tokenizer.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/camera.hpp"

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

		expect_opening_bracket(tokens, i);

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Component) {
				
				expect_opening_bracket(tokens, i);

				if (tokens[i].mType == ETokenType::Parameter) {

					if (tokens[i].mValue == "path") {
						
						expect_colon(tokens, i);
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

		expect_opening_bracket(tokens, i);

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

		CTransform transform;
		expect_opening_bracket(tokens, i);

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {
				if (tokens[i].mValue == "position") {

					transform.mPosition = read_vec3_parameter(tokens, i);

				}

				else if (tokens[i].mValue == "rotation") {

					transform.mRotation = read_vec3_parameter(tokens, i);

				}

				else if (tokens[i].mValue == "scale") {

					transform.mScale = read_vec3_parameter(tokens, i);

				}
			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, transform);

	}
	void SceneParser::parse_smesh(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		CStaticMesh mesh;
		expect_opening_bracket(tokens, i);

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (tokens[i].mValue == "path") {
					expect_colon(tokens, i);
					mesh.mMesh = ctx.mContext.mMeshMgr->CreateStatic(tokens[i].mValue.c_str());
				}
				else LUM_LOG_ERROR("Invalid mesh parameter");

			}
			i++;
		}

		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, mesh);

	}
	void SceneParser::parse_camera(std::vector<FToken>& tokens, int32& i, FParseContext& ctx) {

		CCamera camera;
		expect_opening_bracket(tokens, i);

		while (i < tokens.size() && tokens[i].mType != ETokenType::RBracket) {

			if (tokens[i].mType == ETokenType::Parameter) {

				if (tokens[i].mValue == "fov") {

					camera.mFov = read_float_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "near") {

					camera.mNear = read_float_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "far") {

					camera.mFar = read_float_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "target") {

					camera.mTarget = read_vec3_parameter(tokens, i);

				}
				else if (tokens[i].mValue == "up") {

					camera.mUp = read_vec3_parameter(tokens, i);

				}

			}

			i++;

		}
		
		ctx.mScene.mEntityMgr.AddComponent(ctx.mEntity, camera);
		
	}




	//---------------------------------------------------------
	// Helpers
	//---------------------------------------------------------

	void SceneParser::expect_opening_bracket(std::vector<FToken>& tokens, int32& i) {
		++i;
		LUM_ASSERT(tokens[i].mType == ETokenType::LBracket, "Opening bracket expected");
		++i;
	}

	void SceneParser::expect_colon(std::vector<FToken>& tokens, int32& i) {
		++i;
		LUM_ASSERT(tokens[i].mType == ETokenType::Colon, "Colon expected");
		++i;
	}

	float32 SceneParser::read_float_parameter(std::vector<FToken>& tokens, int32& i) {

		expect_colon(tokens, i);

		float32 value = std::stof(tokens[i].mValue);

		LUM_ASSERT(tokens[i + 1].mType != ETokenType::Number, "Float expected");

		return value;

	}

	glm::vec3 SceneParser::read_vec3_parameter(std::vector<FToken>& tokens, int32& i) {

		expect_colon(tokens, i);

		float32 x = std::stof(tokens[i++].mValue);
		float32 y = std::stof(tokens[i++].mValue);
		float32 z = std::stof(tokens[i].mValue);

		LUM_ASSERT(tokens[i + 1].mType != ETokenType::Number, "Vec3 expected");

		return glm::vec3(x, y, z);

	}
	glm::vec2 SceneParser::read_vec2_parameter(std::vector<FToken>& tokens, int32& i) {

		LUM_ASSERT(tokens[i].mType == ETokenType::Colon, "Colon expected");
		++i;

		float32 x = std::stof(tokens[i++].mValue);
		float32 y = std::stof(tokens[i].mValue);

		LUM_ASSERT(tokens[i + 1].mType != ETokenType::Number, "Vec2 expected");

		return glm::vec2(x, y);

	}


} // namespace lum::fmt