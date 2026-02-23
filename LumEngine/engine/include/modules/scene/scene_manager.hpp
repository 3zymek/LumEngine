#pragma once
#include "scene/scene_loader.hpp"
#include "core/core_pch.hpp"
#include "entity/ecs_manager.hpp"

namespace lum {

	class MTextureManager;
	class MMaterialManager;
	class MMeshManager;
	class MShaderManager;

	namespace render { class Renderer; }

	struct Scene {
	
		std::vector<ecs::EntityID> mEntities;
		std::unique_ptr<ecs::MEntityManager> mEntityMgr;
		
	};

	struct FSceneManagerContext {

		MTextureManager*	mTextureMgr = nullptr;
		MMaterialManager*	mMaterialMgr = nullptr;
		MMeshManager*		mMeshMgr = nullptr;
		MShaderManager*		mShaderMgr = nullptr;
		render::Renderer*	mRenderer = nullptr;

	};

	class MSceneManager {
	public:

		void Initialize( FSceneManagerContext& ctx ) {
			
			mContext = ctx;

		}

		void SetScene( ccharptr scenePath ) {

			uint64 hash = HashStr(scenePath);

			if (!mScenes.contains(hash))
				mScenes[hash] = mLoader.Load( scenePath, mContext );

			mCurrentScene = &mScenes[hash];
		}

		Scene* GetCurrentScene() {
			return mCurrentScene;
		}

	private:

		FSceneManagerContext mContext;

		SceneLoader mLoader;
		Scene* mCurrentScene = nullptr;
		
		std::unordered_map<uint64, Scene> mScenes;
		
	};

}