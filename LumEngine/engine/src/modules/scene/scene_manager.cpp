#include "scene/scene_manager.hpp"
#include "entity/ecs_manager.hpp"
#include "core/utils/asset_loader.hpp"
#include "scene/format/scene_parser.hpp"
#include "scene/format/tokenizer.hpp"
#include "scene/scene_loader.hpp"
#include "entity/components/transform.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MSceneManager::Initialize( FSceneManagerContext& ctx ) {

		mContext = ctx;

	}

	void MSceneManager::SetScene( StringView scenePath ) {

		uint64 hash = HashStr(scenePath);

		if (!mScenes.contains(hash)) {

			std::optional<String> content = AssetLoader::ReadFile(ERootID::External, scenePath);

			if (!content) {
				LUM_LOG_ERROR("Failed to load scene %s: %s", scenePath, AssetLoader::GetErrorMessage());
				return;
			}
			if (!fmt::IsValidFormat(scenePath, fmt::EFormat::Scene)) {
				LUM_LOG_ERROR("Invalid scene format: %s", scenePath);
				return;
			}

			fmt::Tokenizer tokenizer;
			tokenizer.Tokenize(content.value());

			fmt::SceneParser parser(tokenizer, mContext);
			FScene scene;
			parser.Parse(scene);

			mScenes.emplace(hash, std::move(scene));
			
		}

		mCurrentScene = &mScenes[hash];

	}

	void MSceneManager::LoadScene( StringView scenePath ) {

		uint64 hash = HashStr(scenePath);

		std::optional<String> content = AssetLoader::ReadFile(ERootID::External, scenePath);

		if (!content) {
			LUM_LOG_ERROR("Failed to load scene %s: %s", scenePath, AssetLoader::GetErrorMessage());
			return;
		}
		if (!fmt::IsValidFormat(scenePath, fmt::EFormat::Scene)) {
			LUM_LOG_ERROR("Invalid scene format: %s", scenePath);
			return;
		}

		fmt::Tokenizer tokenizer;
		tokenizer.Tokenize(content.value());

		fmt::SceneParser parser(tokenizer, mContext);
		FScene scene;
		parser.Parse(scene);

		mScenes.emplace(hash, std::move(scene));

	}

	FScene* MSceneManager::GetCurrentScene( ) {
		if(mCurrentScene != nullptr)
			return mCurrentScene;
		LUM_LOG_ERROR("No scene set");
		return nullptr;
	}

}