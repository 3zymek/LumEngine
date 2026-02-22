#pragma once
#include "core/core_pch.hpp"

namespace lum {

	namespace ecs { class MEntityManager; }
	class MTextureManager;
	class MMaterialManager;
	class MMeshManager;
	class MShaderManager;

	struct Scene {
	
		ecs::MEntityManager& mEntityMgr;
		
	};

	struct FSceneManagerContext {

		MTextureManager*	mTextureMgr = nullptr;
		MMaterialManager*	mMaterialMgr = nullptr;
		MMeshManager*		mMeshMgr = nullptr;
		MShaderManager*		mShaderMgr = nullptr;

	};

	class MSceneManager {
	public:

		void Initialize( FSceneManagerContext& ctx ) {
			
			mContext = ctx;

		}

		void SetScene(ccharptr scenePath) {

			uint64 hash = HashStr(scenePath);

			if (mScenes.contains(hash))
				mCurrentScene = &mScenes[hash];

		}

	private:

		FSceneManagerContext mContext;

		Scene* mCurrentScene = nullptr;
		
		std::unordered_map<uint64, Scene> mScenes;
		
	};

}