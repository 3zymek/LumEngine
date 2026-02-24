#include "scene/scene_manager.hpp"
#include "entity/ecs_manager.hpp"
#include "core/utils/asset_loader.hpp"
#include "scene/format/scene_parser.hpp"
#include "scene/format/tokenizer.hpp"
#include "scene/scene_loader.hpp"
#include "entity/components/transform.hpp"

namespace lum {

	void MSceneManager::Initialize(FSceneManagerContext& ctx) {

		mContext = ctx;

	}

	void MSceneManager::SetScene(ccharptr scenePath) {

		uint64 hash = HashStr(scenePath);

		if (!mScenes.contains(hash)) {

			std::optional<String> content = AssetLoader::ReadFile(ERootID::External, scenePath);

			if (!content) {
				LUM_LOG_ERROR("Failed to load scene %s: %s", scenePath, AssetLoader::GetErrorMessage());
				return;
			}
			if (!fmt::IsValidFormat(scenePath)) {
				LUM_LOG_ERROR("Invalid scene format: %s", scenePath);
				return;
			}

			fmt::Tokenizer tokenizer;
			tokenizer.Tokenize(content.value());

			fmt::SceneParser parser(tokenizer, mContext);
			Scene scene;
			parser.Parse(scene);

			scene.mEntityMgr.EachWithID<CTransform>(
				[&](ecs::EntityID id, CTransform& c) {
					std::cout << id << '\n';
					std::cout << "pos: " << c.mPosition.x << " " << c.mPosition.y << " " << c.mPosition.z << '\n';
				});

			mScenes.emplace(hash, std::move(scene));
			
		}

		mCurrentScene = &mScenes[hash];

	}

	Scene* MSceneManager::GetCurrentScene() {
		if(mCurrentScene != nullptr)
			return mCurrentScene;
		LUM_LOG_ERROR("No scene set");
		return nullptr;
	}

}