
#include "scene/scene_loader.hpp"
#include "scene/scene_manager.hpp"

#include "entity/components/transform.hpp"
#include "entity/components/material.hpp"
#include "entity/components/mesh.hpp"
#include "entity/components/render.hpp"
#include "entity/components/name.hpp"
#include "entity/components/camera.hpp"

#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/renderer.hpp"
#include "render/render_system.hpp"


/*
* Create own parser, tokenizer etc for engine format
* .lsc -> scene containing whole configuration
* .lmt -> material
* .lprefab ( if needed )
*/
//=====BACKEND======
#include <json.hpp>
//==================

namespace lum {

	using json = nlohmann::json;

	Scene SceneLoader::Load( ccharptr path, FSceneManagerContext& ctx ) {	
		
		Scene scene;
		auto& manager = scene.mEntityMgr;

		std::filesystem::path p;
		String spath = std::filesystem::path(AssetLoader::GetProjectRoot() / std::filesystem::path(path)).lexically_normal().string();

		if (!std::filesystem::exists(spath)) {
			LUM_LOG_ERROR("Scene file %s doesn't exist", spath);
			return {};
		}

		std::ifstream file(spath);
		if (!file.is_open()) {
			LUM_LOG_ERROR("Failed to open scene file: %s", spath);
			return {};
		}
		json parsed = json::parse(file);

		if (parsed.contains("skybox")) {
			auto handle = ctx.mTextureMgr->LoadEquirectangularCubemap(parsed["skybox"]["path"].get<String>().c_str(), parsed["skybox"]["faces"].get<int32>(), ERootID::External);
			ctx.mRenderer->SetEnvionmentTexture(handle);
		}

		for (auto& entity : parsed["entities"]) {
			
			Entity e;

			for (auto& [key, value] : entity["components"].items()) {
				
				if (key == "render") {
					if (value.get<bool>() == true)
						manager.AddComponent<CRender>(e);
				}

				else if (key == "transform") {
					CTransform t;
					auto& pos = value["position"];
					auto& rot = value["rotation"];
					auto& scale = value["scale"];
					t.mPosition = { pos[0].get<float32>(), pos[1].get<float32>(), pos[2].get<float32>() };
					t.mRotation = { rot[0].get<float32>(), rot[1].get<float32>(), rot[2].get<float32>() };
					t.mScale = { scale[0].get<float32>(), scale[1].get<float32>(), scale[2].get<float32>() };
					
					manager.AddComponent(e, t);
				}

				else if (key == "static_mesh") {
					auto mesh = ctx.mMeshMgr->CreateStatic(value["path"].get<String>().c_str(), ERootID::External);
					manager.AddComponent<CStaticMesh>(e, { .mMesh = mesh });
				}

				else if (key == "material") {
					FMaterialBase base;
					base.mBaseColor = { value["base_color"][0].get<float32>(), value["base_color"][1].get<float32>(), value["base_color"][2].get<float32>() };
					base.mRoughness = value["roughness"].get<float32>();
					base.mMetallic = value["metallic"].get<float32>();
					auto baseHandle = ctx.mMaterialMgr->UploadBase(base);
					auto instance = ctx.mMaterialMgr->CreateInstance(baseHandle);
					manager.AddComponent<CMaterial>(e, { .mMat = instance });
				}

				else if (key == "camera") {
					CCamera c;
					c.mFar = value["far"].get<float32>();
					c.mNear = value["near"].get<float32>();
					c.mFov = value["fov"].get<float32>();

					manager.AddComponent<CCamera>(e, c);
					
				}

				
				
			}

			scene.mEntities.push_back(e.mID);
			
		}
		
		return scene;

	}

}